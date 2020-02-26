#pragma once
#include "Scene.hpp"
#include "Camera.hpp"
#include <Eigen/Core>

class RayTracer{
public:
    Scene * scene;
    Camera * camera;
    RayTracer();
    RayTracer(Scene * scene, Camera * camera);
    Eigen::Vector3d ray_tracing(const Ray & ray);
    Eigen::Vector3d mix_color(Eigen::Vector3d & main_color, Eigen::Vector3d emission_color, Eigen::Vector3d & reflect_color, Eigen::Vector3d & refract_color);
}