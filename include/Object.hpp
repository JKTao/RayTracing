#pragma once
#include "Ray.hpp"
#include <Eigen/Core>
#include <optional>
class Intersection;
class Ray;
class Object{
public:
    Eigen::Vector3d emission;
    Eigen::Vector3d color;
    double reflect;
    double refract;
    Object();
    Object(const Eigen::Vector3d  & emission, const Eigen::Vector3d & color, double reflect, double refract);
    virtual std::optional<Intersection> find_intersection(const Ray & ray) = 0;
    virtual ~Object() = default;
};

class Sphere:public Object{
public:
    double radius;
    Eigen::Vector3d center;
    Sphere();
    Sphere(double radius, const Eigen::Vector3d & center, const Eigen::Vector3d & emission, const Eigen::Vector3d & color, double reflect, double refract);
    std::optional<Intersection> find_intersection(const Ray & ray);
};