#pragma once
#include "Ray.hpp"
#include "Model.hpp"
#include <vector>

class Scene{
public:
    std::vector<Triangle*> objects;
    Scene();
    Scene(std::vector<Triangle*> & objects);
    std::vector<Intersection> find_intersections(const Ray & ray);
};