#include "RayTracer.hpp"
#include "Ray.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

RayTracer::RayTracer(Scene * scene, Camera * camera):scene(scene), camera(camera){

}

Eigen::Vector3d RayTracer::ray_tracing(const Ray & ray, int depth){
    std::vector<Intersection> intersections = scene->find_intersections(ray);
    std::vector<Intersection>::iterator nearest_intersection = intersections.end();
    if(!intersections.empty()){
        nearest_intersection = std::min_element(intersections.begin(), intersections.end(), [](const Intersection & A, const Intersection & B){return A.t < B.t;});
    }
    else{
        return Eigen::Vector3d(0, 0, 0);
    }
    //for test
    if(depth == 0){
        return nearest_intersection->normal;
    }
    // std::cout << "test " << std::endl;
    if(depth > 5){
        return nearest_intersection->object->color;
    }
    auto reflect_ray = nearest_intersection->caculate_reflect_ray(ray);
    auto refract_ray = nearest_intersection->caculate_refract_ray(ray);//reflect ray may return null is refract angle > 90.
    double reflect_scale = nearest_intersection->object->reflect, refract_scale = nearest_intersection->object->refract;
    Eigen::Vector3d main_color = nearest_intersection->object->color;
    Eigen::Vector3d emission_color = nearest_intersection->object->emission;
    Eigen::Vector3d reflect_color = ray_tracing(reflect_ray, depth + 1);
    
    if(!refract_ray){
        reflect_color = main_color.cwiseProduct(reflect_color) * reflect_scale;
        return reflect_color + emission_color;  // mix reflect color and emission color.
    }else{
        Eigen::Vector3d refract_color = ray_tracing(refract_ray.value(), depth + 1);
        reflect_color = main_color.cwiseProduct(reflect_color) * reflect_scale;
        refract_color = main_color.cwiseProduct(refract_color) * refract_scale;
        return reflect_color + refract_color + emission_color; //mix reflect color, refract color and emission color
    }
}

// useless in this model.
Eigen::Vector3d mix_color(Eigen::Vector3d & main_color, Eigen::Vector3d emission_color, Eigen::Vector3d & reflect_color, Eigen::Vector3d & refract_color){
    return main_color;
}
