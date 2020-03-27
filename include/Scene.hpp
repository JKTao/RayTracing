#pragma once
#include "Ray.hpp"
#include "Object.hpp"
#include "KdTree.hpp"
#include <vector>

class Scene{
public:
    std::vector<Object*> objects;
    Object *kdtree;
    Scene();
    Scene(std::vector<Object*> & objects);
    bool find_intersection(const Ray & ray, Intersection & intersection);
};