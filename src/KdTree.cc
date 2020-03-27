#include <KdTree.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

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
    // cout << "yes, once succeed!" << endl;
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
    //TODO: improve the axis selection and object division stragety.
    //Firstly find the longest axis index
    AABB boundingbox_union = objects[0]->get_boundingbox();
    for(auto object:objects){
        AABB boundingbox_ = object->get_boundingbox();
        boundingbox_union = boundingbox_union.union_boundingbox(boundingbox_union, boundingbox_);
    }
    Eigen::Vector3d distance = boundingbox_union.second_node - boundingbox_union.first_node;
    vector<double> distance_ = {distance[0], distance[1], distance[2]};
    int index = max_element(distance_.begin(), distance_.end()) - distance_.begin();

    //Secondly, divide the objects by the axis component value.
    KdTree *node;
    Object *left, *right;
    int n = objects.size();
    if(n == 1){
        //In this case, the leaf nodes is the triangle.
        return objects[0];
    }else if(n == 2){
        AABB boundingbox1 = objects[0]->get_boundingbox(), boundingbox2 = objects[1]->get_boundingbox();
        left = objects[0];
        right = objects[1];
        if(boundingbox1.center_node[index] > boundingbox2.center_node[index]){
            swap(left, right);
        }
    }else{
        int position = n/2;
        auto less_than_for_object = [index](const Object *A, const Object *B){return A->center(index) < B->center(index); };
        nth_element(objects.begin(), objects.begin() + position, objects.end(), less_than_for_object); // this is not a good idea in some special case.
        vector<Object*> left_objects(objects.begin(), objects.begin() + position);
        vector<Object*> right_objects(objects.begin() + position, objects.end());
        // double middle = boundingbox_union.center_node[index];
        // int flag = -1;
        // for(auto object:objects){
        //     double curr = object->get_boundingbox().center_node[index];
        //     if(curr < middle){
        //         left_objects.push_back(object);
        //     }else if(curr > middle){
        //         right_objects.push_back(object);
        //     }else{
        //         middle_objects.push_back(object);
        //     }
        // }

        //         //In this case, which seems badly for us, the stragety degenerate. but division is still neccessary to guranttee terminal of recursion.
        //         //A flag is set to assign object at cut point to both side equaprobaly.
        //         if(left_objects.empty()){
        //             left_objects.push_back(object);
        //             flag = 0;
        //         }else{
        //             right_objects.push_back(object);
        //             flag = 1;
        //         }
        //     }else if(flag == 0){
        //         right_objects.push_back(object);
        //         flag = 1;
        //     }else{
        //         left_objects.push_back(object);
        //         flag = 0;
        //     }
        // }
        if(left_objects.empty() || right_objects.empty()){
            cerr << "strange error happens!" << endl;
            // string  empty_message = (left_objects.empty())?"left":"right";
            // cerr << empty_message << endl;
            // cout << boundingbox_union.first_node[0] << " " << boundingbox_union.first_node[1] << " " <<  boundingbox_union.first_node[2] << " " << boundingbox_union.second_node[0] << " " << boundingbox_union.second_node[1] << " " <<  boundingbox_union.second_node[2] << endl;
            // cout << index << " " << " " << " " << objects.size() << endl;
            // for(auto object:objects){
            //     auto boundingbox_ = object->get_boundingbox();
            //     cout << boundingbox_.center_node[index] << "    ";
            //     cout << boundingbox_.first_node[0] << " " << boundingbox_.first_node[1] << " " <<  boundingbox_.first_node[2] << " " << boundingbox_.second_node[0] << " " << boundingbox_.second_node[1] << " " <<  boundingbox_.second_node[2] << endl;
            // }
            exit(1);
        }
        left = KdTree::build_kdtree(left_objects);
        right = KdTree::build_kdtree(right_objects);
    }
    node = new KdTree(left, right, index);
    node->boundingbox = boundingbox_union;
    return node;
}