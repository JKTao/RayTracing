#pragma once
#include "Ray.hpp"
#include "Object.hpp"
#include <vector>

class Scene{
public:
    std::vector<Object*> objects;
    Scene();
    Scene(std::vector<Object*> objects);
    std::vector<Intersection> find_intersections(const Ray & ray);
};