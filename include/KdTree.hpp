#pragma once
#include <Object.hpp>

class KdTree;
class KdTree:public Object{
public:
    int divide_axis;
    AABB boundingbox;
    Object *left;
    Object *right;
    KdTree();
    KdTree(Object *left, Object *right, int index);
    bool find_intersection(const Ray & ray, Intersection & intersection);
    AABB get_boundingbox();
    static Object* build_kdtree(std::vector<Object*> & objects);
};