#include "Model.hpp"
#include "Scene.hpp"
#include <vector>
#include <iostream>

Scene::Scene(){
}

Scene::Scene(std::vector<Triangle*> & objects):objects(std::move(objects)){
}

std::vector<Intersection> Scene::find_intersections(const Ray & ray){
    std::vector<Intersection> intersections;
    for(auto & object:objects){
        Intersection intersection;
        bool intersected = object->find_intersection(ray, intersection);
        if(intersected){
            intersections.push_back(intersection);
        }
    }
    return intersections;
}