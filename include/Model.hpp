#pragma once
#include <string>
#include <map>
#include <Eigen/Core>

struct Material{
    Eigen::Vector3d Kd{0, 0, 0};
    Eigen::Vector3d Ka{0, 0, 0};
    Eigen::Vector3d Ks{0, 0, 0};
    Eigen::Vector3d Tf{0, 0, 0};
    double Ni = 1;
    double Ns = 60;
};

class Model{
public:
    void read_object_file(const char *object_file_path);
    void read_material_lib(const char *material_lib_path);
    std::map<std::string, Material*> material_table;
};