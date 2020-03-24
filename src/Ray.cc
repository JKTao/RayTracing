#include "Ray.hpp"
#include <vector>
#include <Eigen/Core>
#include <iostream>

using Eigen::Vector3d;
Ray::Ray(){

}

Ray::Ray(const Vector3d &origin, const Vector3d & direction):origin(origin), direction(direction){

}

Intersection::Intersection(){

}

Intersection::Intersection(Object *object, const Eigen::Vector3d & normal, const Eigen::Vector3d & position, double t):object(object), normal(normal), position(position), t(t){
    std::cout << "intersection " << normal[0] << normal[1] << normal[2] << std::endl;
}

Ray Intersection::caculate_reflect_ray(const Ray & ray){
    double projection_into_normal = ray.direction.dot(normal);//How to caculate normal in other 3d shape?
    into = (projection_into_normal < 0)?1:-1;
    n = (into == 1)?n:1/n; // reflection rate
    cos_alpha = projection_into_normal * into; // define cos_alpha and sin_alpha as member variable to avoid recaculate in refract ray 
    //TODO: sqrt may be bottleneck?
    sin_alpha = sqrt(1 - cos_alpha * 2);
    Eigen::Vector3d origin = position;
    Eigen::Vector3d direction = ray.direction + 2 * cos_alpha * normal;
    return Ray(origin, direction);
}

std::optional<Ray> Intersection::caculate_refract_ray(const Ray & ray){
    sin_theta = sin_alpha/n;
    if(sin_theta >= 1){
        return std::nullopt;
    }else{
        double tan_theta = sin_theta / sqrt(1 - sin_theta * sin_theta);
        //TODO: sqrt may be bottleneck?
        Eigen::Vector3d origin = position;
        Eigen::Vector3d direction = (ray.direction + cos_alpha * normal).normalized() * tan_theta - normal * into;
        return Ray(origin, direction);
    }
}