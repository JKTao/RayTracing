#include "Scene.hpp"
#include <vector>
#include <iostream>

Scene::Scene(){

}

Scene::Scene(std::vector<Object*> objects):objects(std::move(objects)){

}

std::vector<Intersection> Scene::find_intersections(const Ray & ray){
    std::vector<Intersection> intersections;
    for(auto object:objects){
        std::optional<Intersection> op_intersection = object->find_intersection(ray);
        if(op_intersection){
            intersections.push_back(*op_intersection);
            auto it = *op_intersection;
            std::cout << it.normal << std::endl;
        }
    }
    return intersections;
}