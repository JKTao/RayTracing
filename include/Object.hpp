#pragma once
#include "Ray.hpp"
#include <Eigen/Core>
#include <optional>
class Intersection;
class Ray;
class Object{
public:
    Object();
    virtual std::optional<Intersection> find_intersection(const Ray & ray) = 0;

};

class Sphere:public Object{
public:
    double radius;
    Eigen::Vector3d center;
    Eigen::Vector3d emission;
    Eigen::Vector3d color;
    double reflect;
    Sphere();
    Sphere(double radius, const Eigen::Vector3d & center, const Eigen::Vector3d & emission, const Eigen::Vector3d & color, double reflect);
    std::optional<Intersection> find_intersection(const Ray & ray);
};