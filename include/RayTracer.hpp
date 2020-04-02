#pragma once
#include "Scene.hpp"
#include "Camera.hpp"
#include "Ultility.hpp"
#include <Eigen/Core>

class RayTracer{
public:
    Scene * scene;
    Camera * camera;
    int max_depth;
    Random random_generator;
    RayTracer();
    RayTracer(Scene * scene, Camera * camera, int max_depth);
    Eigen::Vector3d ray_tracing(Ray & ray, int depth);
    //useless in this model
    Ray next_ray_sample(Ray & ray, Intersection & intersection);
    Ray diffuse_sample(Eigen::Vector3d & x_axis, Eigen::Vector3d & position);
    Ray specular_sample(Eigen::Vector3d & x_axis, Eigen::Vector3d & position, double Ns);
};