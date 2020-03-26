#pragma once
#include <string>
#include <map>
#include <Ray.hpp>
#include <Object.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>



class Model{
public:
    std::vector<Triangle*> triangles;
    void read_object_file(const char *object_file_path);
    void read_material_lib(const char *material_lib_path);
    std::map<std::string, Material*> material_table;
};