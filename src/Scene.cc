#include "Object.hpp"
#include "KdTree.hpp"
#include "Scene.hpp"
#include <vector>
#include <iostream>

Scene::Scene(){
}

Scene::Scene(std::vector<Object*> & objects):objects(objects.begin(), objects.end()){
    kdtree = KdTree::build_kdtree(objects);
}

bool Scene::find_intersection(const Ray & ray, Intersection & intersection){
    return kdtree->find_intersection(ray, intersection);
}