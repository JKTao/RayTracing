#include "Camera.hpp"
#include "cmath"
#include <iostream>
Camera::Camera(){
}

Camera::Camera(double fov, const Eigen::Vector3d & eye, const Eigen::Vector3d & front, const Eigen::Vector3d & up, int height, int width):fov(fov), eye(eye), front(front.normalized()), up(up.normalized()), height(height), width(width){
    double tan_half_fov = tan(fov/2 * M_PI / 180);
    // std::cout << "tan_half_fov = " << tan_half_fov << std::endl;
    scale_y = tan_half_fov / height * 2;
    scale_x = scale_y;
    this->right = front.cross(up).normalized();
    this->up = right.cross(front).normalized();
    this->right = this->right * scale_x;
    this->up = this->up * scale_y;
    initial_direction = front - this->right * (width/2 - 0.5) + this->up * (height/2 - 0.5); //direction to the left-up pixel.
    // std::cerr << "front" << this->front << std::endl;
    // std::cerr << "up" << this->up << std::endl;
    // std::cerr << "right" << this->right << std::endl;
    //if found inverse image, change + up to - up
}

Ray Camera::generate_ray(int x, int y){
    Eigen::Vector3d origin = eye;
    Eigen::Vector3d direction = initial_direction + x * right - y * up;
    direction.normalize();
    return Ray(origin, direction);
}