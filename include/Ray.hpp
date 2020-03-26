#pragma once
#include <Eigen/Core>
class Triangle;
class Ray{
public:
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
    Eigen::Vector3d inv_direction;
    Ray();
    Ray(const Eigen::Vector3d & origin, const Eigen::Vector3d & direction);
};

class Intersection{
public:
    Triangle *object;
    Eigen::Vector3d normal;
    Eigen::Vector3d position;
    double t;
    double n;
    int into;
    double sin_alpha;
    double cos_alpha;
    double sin_theta;
    Intersection();
    Intersection(Triangle *object, const Eigen::Vector3d & normal, const Eigen::Vector3d & position, double t);
    Ray caculate_reflect_ray(const Ray & ray);
    std::optional<Ray> caculate_refract_ray(const Ray & ray);
};