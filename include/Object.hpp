#pragma once
#include "Ray.hpp"
#include <Eigen/Core>
#include <optional>
class Intersection;
class Ray;
class AABB;

struct Material{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Vector3d Kd{0, 0, 0};
    Eigen::Vector3d Ka{0, 0, 0};
    Eigen::Vector3d Ks{0, 0, 0};
    Eigen::Vector3d Tf{0, 0, 0};
    double Ni = 1;
    double Ns = 60;
};


class Object{
public:
    Material *mtl;
    Object();
    Object(Material *mtl);
    virtual bool find_intersection(const Ray & ray, Intersection & intersection) = 0;
    virtual ~Object() = default;
    virtual AABB get_boundingbox() = 0;
};

class AABB:public Object{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Vector3d first_node;
    Eigen::Vector3d second_node;
    Eigen::Vector3d center_node;
    bool find_intersection(const Ray & ray, Intersection & intersection);
    AABB get_boundingbox();
    AABB union_boundingbox(AABB & boundingbox1, AABB &boundingbox2);
    AABB();
    AABB(Eigen::Vector3d &first_node, Eigen::Vector3d &second_node);
};

class Triangle:public Object{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Matrix3d vertices;
    Eigen::Matrix3d normals;
    Eigen::Matrix3d edges;
    AABB boundingbox;
    Triangle();
    Triangle(Eigen::Vector3d &v1, Eigen::Vector3d &v2, Eigen::Vector3d &v3, Eigen::Vector3d & vn1, Eigen::Vector3d &vn2, Eigen::Vector3d &vn3, Material *mtl);
    bool find_intersection(const Ray & ray, Intersection & intersection);
    AABB get_boundingbox();
};
