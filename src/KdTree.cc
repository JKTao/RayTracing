#include <KdTree.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

KdTree::KdTree(){
}

KdTree::KdTree(Object *left, Object *right, int index):left(left), right(right), divide_axis(index){
}

AABB KdTree::get_boundingbox(){
    return boundingbox;
}

bool KdTree::find_intersection(const Ray &ray, Intersection & intersection){
    if(!boundingbox.find_intersection(ray, intersection)){
        return false;
    }
    bool intersected = false;
    if(ray.direction[divide_axis] > 0){
        if(left != nullptr){
            intersected |= left->find_intersection(ray, intersection);
        }
        if(right != nullptr){
            intersected |= right->find_intersection(ray, intersection);
        }
    }else{
        if(right != nullptr){
            intersected |= right->find_intersection(ray, intersection);
        }
        if(left != nullptr){
            intersected |= left->find_intersection(ray, intersection);
        }
    }
    return intersected;
}

Object* KdTree::build_kdtree(vector<Object*> & objects){
    //Is this a good stragety?
    AABB boundingbox_union = objects[0]->get_boundingbox();
    for(auto object:objects){
        AABB boundingbox_ = object->get_boundingbox();
        boundingbox_union = boundingbox_union.union_boundingbox(boundingbox_union, boundingbox_);
    }
    Eigen::Vector3d distance = boundingbox_union.second_node - boundingbox_union.first_node;
    vector<double> distance_ = {distance[0], distance[1], distance[2]};
    int index = max_element(distance_.begin(), distance_.end()) - distance_.begin();

    KdTree *node;
    Object *left, *right;
    if(objects.size() == 1){
        return objects[0];
    }else if(objects.size() == 2){
        AABB boundingbox1 = objects[0]->get_boundingbox(), boundingbox2 = objects[1]->get_boundingbox();
        left = objects[0];
        right = objects[1];
        if(boundingbox1.center_node[index] > boundingbox2.center_node[index]){
            swap(left, right);
        }
        node->boundingbox = boundingbox_union;
    }else{
        vector<Object*> left_objects, right_objects;
        double middle = boundingbox_union.center_node[index];
        int flag = 1;
        for(auto object:objects){
            double curr = object->get_boundingbox().center_node[index];
            if(curr < middle){
                left_objects.push_back(object);
            }else if(curr > middle){
                right_objects.push_back(object);
            }else if(flag){
                left_objects.push_back(object);
                flag = 0;
            }else{
                right_objects.push_back(object);
                flag = 1;
            }
        }
        if(left_objects.empty() || right_objects.empty()){
            cerr << "strange error happens!" << endl;
            exit(1);
        }
        left = KdTree::build_kdtree(left_objects);
        right = KdTree::build_kdtree(right_objects);
    }
    node = new KdTree(left, right, index);
    node->boundingbox = boundingbox_union;
    return node;
}