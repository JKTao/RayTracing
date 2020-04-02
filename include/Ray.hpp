#pragma once
#include <Eigen/Core>
class Triangle;
class Ray{
public:
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
    Eigen::Vector3d inv_direction;
    enum RAY_TYPE{
        DIFFUSE,
        SPECULAR,
        REFRACT
    };
    int ray_type;
    Ray();
    Ray(const Eigen::Vector3d & origin, const Eigen::Vector3d & direction);
    Ray(const Eigen::Vector3d &origin, const Eigen::Vector3d & direction, int ray_type);
    Ray caculate_refract_ray(Eigen::Vector3d position, Eigen::Vector3d normal, double relative_n, double cos_alpha);
    Eigen::Vector3d caculate_reflect_direction(Eigen::Vector3d normal, double cos_alpha);
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
};