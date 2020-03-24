#pragma once
#include "Ray.hpp"
#include <Eigen/Core>
#include <Eigen/Geometry>


class Camera{
public:
    double fov;
    Eigen::Vector3d eye;
    Eigen::Vector3d front;
    Eigen::Vector3d right;
    Eigen::Vector3d up;
    int width;
    int height;
    double scale_x;
    double scale_y;
    Eigen::Vector3d initial_direction;
    Camera();
    Camera(double fov, const Eigen::Vector3d & eye, const Eigen::Vector3d & front, const Eigen::Vector3d & up, int height, int width);
    Ray generate_ray(int x, int y);

};
