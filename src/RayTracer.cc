#include "RayTracer.hpp"
#include "Ray.hpp"
#include <vector>
#include <algorithm>

RayTracer::RayTracer(Scene * scene, Camera * camera):scene(scene), camera(camera){

}

Eigen::Vector3d RayTracer::ray_tracing(const Ray & ray){
    std::vector<Intersection> intersections = scene->find_intersections(ray);
    std::vector<Intersection>::iterator nearest_intersection;
    if(!intersections.empty()){
        nearest_intersection = std::min_element(intersections.begin(), intersections.end(), [](const Intersection & A, const Intersection & B){return A.t < B.t});
    }
    else{
        return Eigen::Vector3d(1, 1, 1);
    }
    auto reflect_ray = nearest_intersection->caculate_reflect_ray(ray);
    auto refract_ray = nearest_intersection->caculate_refract_ray(ray);
    Eigen::Vector3d main_color = nearest_intersection->obj.radiance;
    Eigen::Vector3d emission_color = nearest_intersection->obj.emission;
    Eigen::Vector3d reflect_color = ray_tracing(reflect_ray);
    Eigen::Vector3d refract_color = ray_tracing(refract_ray);
    return mix_color(main_color, emission_color, reflect_color, refract_color);
}

Eigen::Vector3d mix_color(Eigen::Vector3d & main_color, Eigen::Vector3d emission_color, Eigen::Vector3d & reflect_color, Eigen::Vector3d & refract_color){
    return main_color;
}
