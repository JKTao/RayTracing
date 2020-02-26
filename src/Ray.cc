#include "Ray.hpp"
#include <vector>
#include <Eigen/Core>


using Eigen::Vector3d;
Ray::Ray(){

}

Ray::Ray(const Vector3d &origin, const Vector3d & direction):origin(origin), direction(direction){

}

Intersection::Intersection(){

}

Intersection::Intersection(Object *object, const Eigen::Vector3d & position, const Eigen::Vector3d & normal, double t):object(object), normal(normal), position(position), t(t){
}

Ray Intersection::caculate_reflect_ray(const Ray & ray){
    double projection_into_normal = ray.direction.dot(normal);
    into = (projection_into_normal < 0)?1:-1;
    cos_theta = projection_into_normal * into;
    Eigen::Vector3d origin = ray.origin;
    Eigen::Vector3d direction = ray.direction + 2 * cos_theta * normal;
    return Ray(origin, direction);
}

Ray Intersection::caculate_refract_ray(const Ray & ray){
    return ray;
}