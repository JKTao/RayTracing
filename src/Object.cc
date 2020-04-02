#include <Object.hpp>
#include <Model.hpp>
#include <iostream>

using namespace std;
Object::Object(){
}

Object::Object(Material *mtl):mtl(mtl){
}

double Object::get_center(int index){
    return center[index];
}

Triangle::Triangle(){
}

Triangle::Triangle(Eigen::Vector3d &v1, Eigen::Vector3d &v2, Eigen::Vector3d &v3, Eigen::Vector3d & vn1, Eigen::Vector3d &vn2, Eigen::Vector3d &vn3, Material *mtl):Object(mtl){
    vertices << v1, v2, v3;
    normals << vn1.normalized(), vn2.normalized(), vn3.normalized();
    edges.col(1) = vertices.col(1) - vertices.col(0);
    edges.col(2) = vertices.col(2) - vertices.col(0);
    Eigen::Vector3d first_node = vertices.rowwise().minCoeff();
    Eigen::Vector3d second_node = vertices.rowwise().maxCoeff();
    boundingbox = AABB(first_node, second_node);
    center = boundingbox.center_node;
    //rows
}

AABB Triangle::get_boundingbox(){
    return boundingbox;
}

bool Triangle::find_intersection(const Ray & ray, Intersection &intersection){
    // https://huangwang.github.io/2019/06/04/%E6%B1%82%E8%A7%A3%E5%B0%84%E7%BA%BF%E4%B8%8E%E4%B8%89%E8%A7%92%E5%BD%A2%E4%BA%A4%E7%82%B9%E7%9A%84%E7%AE%97%E6%B3%95/
    //find_intersection
    Eigen::Vector3d T = ray.origin - vertices.col(0);
    Eigen::Vector3d P = ray.direction.cross(edges.col(2));
    Eigen::Vector3d Q = T.cross(edges.col(1));
    double b = P.dot(edges.col(1));
    if(b > 1e-15 || b < -1e-15){
        double b_inv = 1/b;
        double t = Q.dot(edges.col(2)) * b_inv;
        if(t < 1e-8 || t > intersection.t){
            return false;
        }
        double u = P.dot(T) * b_inv;
        double v = Q.dot(ray.direction) * b_inv;
        if(0 <= u  && 0 <= v && u + v <= 1){
            double w = 1 - u - v;
            Eigen::Vector3d normal = (normals.col(0) * w + normals.col(1) * u + normals.col(2) * v).normalized();
            Eigen::Vector3d position = ray.direction * t + ray.origin;
            intersection = move(Intersection(this, normal, position, t));
            return true;
        }
        return false;
    }
    return false;
}

AABB::AABB(){

}

AABB::AABB(Eigen::Vector3d & first_node, Eigen::Vector3d & second_node):first_node(move(first_node)), second_node(move(second_node)){
    center_node = (this->first_node + this->second_node) * 0.5;
}

AABB AABB::get_boundingbox(){
}

bool AABB::find_intersection(const Ray & ray, Intersection &intersection){
    // bouding box intersection!
    Eigen::Matrix<double, 3, 2> t;
    t.col(0) = (first_node - ray.origin).cwiseProduct(ray.inv_direction);
    t.col(1) = (second_node - ray.origin).cwiseProduct(ray.inv_direction);
    Eigen::Vector3d t1 = t.rowwise().minCoeff();
    Eigen::Vector3d t2 = t.rowwise().maxCoeff();
    double t_near = t1.maxCoeff();
    double t_far = t2.minCoeff();
    // cout << t_near << t_far << endl;
    // cout << ray.direction[0] << " " << ray.direction[1] << " " << ray.direction[2] << endl;
    // cout << first_node[0] << " " << first_node[1] << " " << first_node[2] << endl;
    // cout << second_node[0] << " " << second_node[1] << " " << second_node[2] << endl;
    // if t_near == t_far, one dimension of boundingbox is zero, which indicated the object is parrallel to corresponding axis, so == is important.
    return (t_near <= t_far && t_near < 1e10 && t_far > 1e-10);
}

AABB AABB::union_boundingbox(AABB & boundingbox1, AABB & boundingbox2){
    Eigen::Vector3d first_node = boundingbox1.first_node.cwiseMin(boundingbox2.first_node);
    Eigen::Vector3d second_node = boundingbox1.second_node.cwiseMax(boundingbox2.second_node);
    return AABB(first_node, second_node);
}