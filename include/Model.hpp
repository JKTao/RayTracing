#pragma once
#include <string>
#include <map>
#include <Ray.hpp>
#include <Object.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>



class Model{
public:
    std::vector<Object*> triangles;
    std::string object_file_path;
    std::string material_lib_path;
    Eigen::Vector3d eye;
    Eigen::Vector3d up;
    Eigen::Vector3d lookat;
    Eigen::Vector3d front;
    double fov;
    int width;
    int height;
    Model();
    Model(std::string config_file_path);
    void read_config(const char *config_file_path);
    void read_object_file(const char *object_file_path);
    void read_material_lib(const char *material_lib_path);
    std::map<std::string, Material*> material_table;
};