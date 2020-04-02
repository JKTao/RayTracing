#include "Ray.hpp"
#include <vector>
#include <Eigen/Core>
#include <iostream>

using Eigen::Vector3d;
using namespace std;
Ray::Ray(){
}

Ray::Ray(const Vector3d &origin, const Vector3d & direction):origin(origin), direction(direction){
    inv_direction = direction.cwiseInverse();
}

Ray::Ray(const Vector3d &origin, const Vector3d & direction, int ray_type):origin(origin), direction(direction), ray_type(ray_type){
    inv_direction = direction.cwiseInverse();
}

Intersection::Intersection():t(1e10){

}

Intersection::Intersection(Triangle *object, const Eigen::Vector3d & normal, const Eigen::Vector3d & position, double t):object(object), normal(normal), position(position), t(t){
    // std::cout << "intersection " << normal[0] << normal[1] << normal[2] << std::endl;
}

Eigen::Vector3d Ray::caculate_reflect_direction(Eigen::Vector3d normal, double cos_alpha){
    Eigen::Vector3d direction = this->direction + 2 * cos_alpha * normal;
    return direction;
}

Ray Ray::caculate_refract_ray(Eigen::Vector3d position, Eigen::Vector3d normal, double relative_n, double cos_alpha){
    double cos_beta = 1 - relative_n * relative_n * (1 - cos_alpha * cos_alpha);
    if(cos_beta < 0){
        return Ray(position, position, -1);
    }
    cos_beta = sqrt(cos_beta);
    Eigen::Vector3d new_direction = relative_n * direction + (relative_n * cos_alpha - cos_beta) * normal;
    return Ray(position, new_direction, Ray::REFRACT);
}