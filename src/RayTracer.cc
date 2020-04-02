#include "RayTracer.hpp"
#include "Ray.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
using namespace std;

extern int logging;
RayTracer::RayTracer(Scene * scene, Camera * camera, int max_depth):scene(scene), camera(camera), max_depth(max_depth){

}

Ray RayTracer::diffuse_sample(Eigen::Vector3d & z_axis, Eigen::Vector3d & position){
    double phi = random_generator.double_rand() * 2 * M_PI;
    double sin_theta_pow2 = random_generator.double_rand();
    double cos_phi = cos(phi);
    double sin_phi = sin(phi);
    double cos_theta = sqrt(1 - sin_theta_pow2);
    double sin_theta = sqrt(sin_theta_pow2);
    Eigen::Vector3d x_axis;
    Eigen::Vector3d diff_direction(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
    if(fabs(z_axis[0]) > fabs(z_axis[1])){
        x_axis = Eigen::Vector3d(z_axis[2], 0, -z_axis[0]).normalized();
    }else{
        x_axis = Eigen::Vector3d(0, z_axis[2], -z_axis[1]).normalized();
    }
    Eigen::Vector3d y_axis = z_axis.cross(x_axis).normalized();
    Eigen::Matrix3d axis;
    axis << x_axis, y_axis, z_axis;
    Eigen::Vector3d direction = axis * diff_direction;
    return Ray(position, direction, Ray::DIFFUSE);
}

Ray RayTracer::specular_sample(Eigen::Vector3d & z_axis, Eigen::Vector3d & position, double Ns){
    double phi = random_generator.double_rand() * 2 * M_PI;
    double cos_theta = pow(random_generator.double_rand(), 1 / (Ns+1));
    if(logging == 2){
        cout << "    " << "cos_theta: " << cos_theta << endl;
    }
    double cos_phi = cos(phi);
    double sin_phi = sin(phi);
    double sin_theta = sqrt(1 - cos_theta * cos_theta);
    Eigen::Vector3d x_axis;
    Eigen::Vector3d diff_direction(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
    if(fabs(z_axis[0]) > fabs(z_axis[1])){
        x_axis = Eigen::Vector3d(z_axis[2], 0, -z_axis[0]).normalized();
    }else{
        x_axis = Eigen::Vector3d(0, z_axis[2], -z_axis[1]).normalized();
    }
    Eigen::Vector3d y_axis = z_axis.cross(x_axis).normalized();
    Eigen::Matrix3d axis;
    axis << x_axis, y_axis, z_axis;
    Eigen::Vector3d direction = axis * diff_direction;
    // cerr << diff_direction[0] << " " << diff_direction[1] << " " << diff_direction[2] << endl;
    // cerr << diff_direction.norm() << endl;
    // cerr << sin_theta << endl;
    // cerr << axis << endl;
    // cerr << Ns << endl;
    // cerr << direction << endl;
    // cerr << direction.norm() << endl;
    // exit(1);
    // if(logging == 2){
    //     cout << "    DIFF_DIRECTION " <<  diff_direction[0] << " " <<  diff_direction[1] << " " << diff_direction[2] << endl;
    //     cout << "    DIRECTION " <<  direction[0] << " " <<  direction[1] << " " << direction[2] << endl;
    //     cout << "    DIRECTION_NORM " <<  direction.norm() << endl;
    //     cout << "    COS_THETA " << z_axis.dot(direction) << endl;
    //     cout << "    XAXIS ZAXIS " << z_axis.dot(x_axis) << endl;
    // }
    return Ray(position, direction, Ray::SPECULAR);
}

Ray RayTracer::next_ray_sample(Ray & ray, Intersection & intersection){
    //reflection, refraction, diffuse.
    //first sample: for  transparent material, sample between refract ray and reflect ray, the weight is fresnel factor by Schlick's approximation.
    //refrence: https://en.wikipedia.org/wiki/Schlick%27s_approximation
    //second sample: sample between diffuse and specular ray.
    Triangle *object = intersection.object;
    Eigen::Vector3d normal;
    double cos_alpha = ray.direction.dot(intersection.normal);
    Eigen::Vector3d reflect_direction;
    double n1, n2;
    if(cos_alpha > 0){
        // out case
        normal = -intersection.normal;
        n1 = object->mtl->Ni;
        n2 = 1;
    }else{
        // in case
        normal = intersection.normal;
        n1 = 1;
        n2 = object->mtl->Ni;
        cos_alpha = -cos_alpha;
    }
    reflect_direction = ray.caculate_reflect_direction(normal, cos_alpha);
    if(object->mtl->Ni > 1){
        double res_cos_alpha = 1 - cos_alpha;
        double R0 = (n1 - n2)/(n1 + n2);
        R0 = R0 * R0;
        double res_cos_alpha_pow5 = res_cos_alpha * res_cos_alpha;
        res_cos_alpha_pow5 = res_cos_alpha * res_cos_alpha_pow5 * res_cos_alpha_pow5;
        double R = R0 + (1 - R0) * res_cos_alpha_pow5;
        // refract case.
        if(R < random_generator.double_rand()){
            Ray refract_ray =  ray.caculate_refract_ray(intersection.position, normal, n1/n2, cos_alpha);
            if(refract_ray.ray_type != -1){
                return refract_ray;
            }
        }
        if(object->mtl->diffuse == 0){
            return Ray(intersection.position, reflect_direction, Ray::SPECULAR);
        }
    }
    //reflect and diffuse case
    //TODO: mtl, diffuse
        //light case
    //glossy case
    // cerr << "reflect_direction " << reflect_direction << endl;
    // cerr << "normal" << normal << endl;
    // cerr << "direction" << ray.direction << endl;
    // exit(1);
    // if(intersection.object->mtl->diffuse < drand48()){
    if(0.5 < random_generator.double_rand()){
    // if(intersection.object->mtl->Ks[0] > 0.5){
        // if(logging == 1){
        //     cout << "glossy branch, specular sample\n";
        // }
        // return Ray(intersection.position, reflect_direction, Ray::SPECULAR);
        if(logging == 2){
            cout << "    REFLECTRAY_DIRECTION " << reflect_direction[0] << " " << reflect_direction[1] << " " << reflect_direction[2] << endl;
        }
        return specular_sample(reflect_direction, intersection.position, intersection.object->mtl->Ns);
    }else{
        // if(logging == 1){
        //     cout << "glossy branch, diffuse sample\n";
        // }
        return diffuse_sample(normal, intersection.position);
    }
}

Eigen::Vector3d RayTracer::ray_tracing(Ray & ray, int depth){
    Intersection intersection;
    if(logging == 2){
        cout << "DEPTH " << depth << endl;
        cout << "  RAY_ORIGIN " << ray.origin[0] << " " << ray.origin[1] << " " << ray.origin[2] << endl;
        cout << "  RAY_DIRECTION " << ray.direction[0] << " " << ray.direction[1] << " " << ray.direction[2] << endl;
    }
    if(!scene->find_intersection(ray, intersection)){
        if(logging == 2){
            cout << "  No intersection" << endl;
        }
        return Eigen::Vector3d(0, 0, 0); //background color black
    }
    if(depth > max_depth){
        if(logging == 2){
            cout << "  MTL Ka: " << intersection.object->mtl->Ka[0] << endl;
            cout << "  MTL Kd: " << intersection.object->mtl->Kd[0] << endl;
            cout << "  INTERSECTION: " << intersection.position[0] << " " << intersection.position[1] << " " << intersection.position[2] << endl;
        }
        return intersection.object->mtl->Ka;
    }
    Ray next_ray = next_ray_sample(ray, intersection);
    Eigen::Vector3d color = ray_tracing(next_ray, depth + 1);
    if(logging == 1 && depth < 5){
        cout << "color" << " " << color[0] << " " << color[1] << " " << color[2]<< endl;
    }
    if(logging == 2){
        cout << "  INDIRECT_COLOR: " << color[0] << " " << color[1] << " " << color[2] << endl;
    }
    if(next_ray.ray_type == next_ray.DIFFUSE){
        // diffuse case
        if(logging == 1 && depth < 5){
            cout << "Kd " << intersection.object->mtl->Kd[0] << " " << intersection.object->mtl->Kd[1] << " "  << intersection.object->mtl->Kd[2] << endl;
        }
        color =  2 * intersection.object->mtl->Kd.cwiseProduct(color);
    }else if(next_ray.ray_type == next_ray.SPECULAR){
        // specular case
        if(logging == 1 && depth < 5){
            cout << "Ks " << intersection.object->mtl->Ks[0] << " " << intersection.object->mtl->Ks[1] << " "  << intersection.object->mtl->Ks[2] << endl;
        }
        color = intersection.object->mtl->Ks.cwiseProduct(color);
    }else if(next_ray.ray_type == next_ray.REFRACT){
        // refract case
        if(logging == 1 && depth < 5){
            cout << "Tf " << intersection.object->mtl->Tf[0] << " " << intersection.object->mtl->Tf[1]  << " " << intersection.object->mtl->Tf[2] << endl;
        }
        color = intersection.object->mtl->Tf.cwiseProduct(color);
    }else{
        cerr << "bad things happened!" << endl;
        exit(1);
    }
    if(logging == 1 && depth < 5){
        cout << "Ka " << intersection.object->mtl->Ka[0] << " " << intersection.object->mtl->Ka[1] << " "  << intersection.object->mtl->Ka[2] << endl;
    }
    return intersection.object->mtl->Ka + color;
}

