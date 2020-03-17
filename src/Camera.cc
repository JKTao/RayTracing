#include "Camera.hpp"
#include "cmath"
Camera::Camera(){

}

Camera::Camera(double fov, const Eigen::Vector3d & eye, const Eigen::Vector3d & front, const Eigen::Vector3d & right, int height, int width):fov(fov), eye(eye), front(front.normalized()), right(right.normalized()), height(height), width(width){
    double tan_half_fov = tan(fov/2 * M_PI / 180);
    scale = tan_half_fov / width * 2;
    up = front.cross(right);
    up.normalize();
    initial_direction = front - right * scale * (width/2 - 0.5) + up * scale * (height/2 - 0.5);
}

Ray Camera::generate_ray(int x, int y){
    Eigen::Vector3d origin = eye;
    Eigen::Vector3d direction = initial_direction + x * right - y * up;
    direction.normalize();
    return Ray(origin, direction);
}