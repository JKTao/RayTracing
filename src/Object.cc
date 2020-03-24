#include "Object.hpp"
#include <cmath>
#include <Eigen/Core>
#include <iostream>


using Eigen::Vector3d;

Object::Object(){}

Object::Object(const Eigen::Vector3d  & emission, const Eigen::Vector3d & color, double reflect, double refract):emission(emission), color(color), reflect(reflect), refract(refract){

}

Sphere::Sphere(){}

Sphere::Sphere(double radius, const Vector3d & center, const Eigen::Vector3d  & emission, const Eigen::Vector3d & color, double reflect, double refract):Object(emission, color, reflect, refract), radius(radius), center(center){
    
}

std::optional<Intersection> Sphere::find_intersection(const Ray & ray){
    Vector3d p = ray.origin - center;
    double A = ray.direction.squaredNorm();
    double B = p.dot(ray.direction);
    double C = p.squaredNorm() - radius * radius;
    double determinator = B * B - A * C;
    double t;
    if(determinator <= 0){
        return std::nullopt;
    }else{
        double sqrt_determinator = sqrt(determinator);
        t = (-B - sqrt_determinator) / A;
        if(t < 0){
            t = (-B + sqrt_determinator) / A;
            if(t < 0){
                return std::nullopt;
            }
        }
    }
    Eigen::Vector3d position = ray.direction * t + ray.origin;
    Eigen::Vector3d normal = (position - center).normalized();
    // std::cout << "sphere " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
    return Intersection(static_cast<Object*>(this), normal, position, t);
}


