#include "Ray.hpp"
#include "Object.hpp"
#include "RayTracer.hpp"
#include "Scene.hpp"
#include <iostream>
#include <vector>
#include <Eigen/Core>

void write_colors(std::vector<std::vector<Eigen::Vector3d>> & colors){

    
}
int main(){
    using Vec = Eigen::Vector3d;
    Sphere *light = new Sphere(600, Vec(50,681.6-0.27,81.6),Vec(12,12,12),  Vec(0, 0, 0), 1, 1);
    Sphere *sphere = new Sphere(16.5,Vec(27,16.5,47),  Vec(0, 0, 0),Vec(1,1,1)*.999, 1, 1);
    std::vector<Sphere*> objects = {light, sphere};
    Scene *scene = new Scene(std::vector<Object*>(objects.begin(), objects.end()));
    Camera *camera = new Camera();//TODO set camera parameter;
    RayTracer *raytracer = new RayTracer(scene, camera);
    std::vector<std::vector<Eigen::Vector3d>> colors(camera->height);
    for(int i = 0; i < camera->height; i++){
        for(int j = 0; j < camera->width; j++){
            colors[i].emplace_back(raytracer->ray_tracing(camera->generate_ray(j, i), 0));
        }
    }
    write_colors(colors);
    
}