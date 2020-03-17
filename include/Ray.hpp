#pragma once
#include <Eigen/Core>
#include "Object.hpp"
class Object;
class Ray{
public:
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
    Ray();
    Ray(const Eigen::Vector3d & origin, const Eigen::Vector3d & direction);
};

class Intersection{
public:
    Object *object;
    Eigen::Vector3d normal;
    Eigen::Vector3d position;
    double t;
    double n;
    int into;
    double sin_alpha;
    double cos_alpha;
    double sin_theta;
    Intersection();
    Intersection(Object *object, const Eigen::Vector3d & position, const Eigen::Vector3d & normal, double t);
    Ray caculate_reflect_ray(const Ray & ray);
    std::optional<Ray> caculate_refract_ray(const Ray & ray);
};