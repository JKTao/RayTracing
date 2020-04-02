#include <Model.hpp>
#include <Ray.hpp>
#include <Object.hpp>
#include <iostream>
#include <cstdio>
#include <yaml-cpp/yaml.h>

using namespace std;
bool starts_with(char *str, const char *pattern){
    int i = 0;
    while(str[i] == pattern[i] && str[i] != '\0'){
        i++;
    }
    return (pattern[i] == '\0');
}


void read_config_vector(YAML::Node config, string key, Eigen::Vector3d & vector){
    YAML::Node node = config[key];
    for(int i = 0; i < node.size(); i++){
        vector[i] = node[i].as<double>();
    }
}

Model::Model(){
}

Model::Model(string config_file_path){
    read_config(config_file_path.c_str());
    read_material_lib(this->material_lib_path.c_str());
    read_object_file(this->object_file_path.c_str());
}

void Model::read_config(const char *config_file_path){
    YAML::Node config = YAML::LoadFile(config_file_path);
    object_file_path = config["object_file_path"].as<string>();
    material_lib_path = config["material_lib_path"].as<string>();
    save_image_path = config["save_image_path"].as<string>();
    iteration_times = config["max_iteration"].as<int>();
    max_depth = config["max_depth"].as<int>();
    fov = config["fov"].as<double>();
    width = config["width"].as<int>();
    height = config["height"].as<int>();
    read_config_vector(config, "position", this->eye);
    read_config_vector(config, "lookat", this->lookat);
    read_config_vector(config, "up", this->up);
    front = lookat - eye;
}

void Model::read_object_file(const char *object_file_path){
    char buffer[200];
    string mtllib_name, mtl_name;
    Material *mtl;
    double x, y, z;
    int v1, v2, v3, v4, vn1, vn2, vn3, vn4;
    vector<Eigen::Vector3d> vertices;
    vector<Eigen::Vector3d> normals;
    FILE *object_file  = fopen(object_file_path, "r");
    if(object_file == nullptr){
        cerr << "Unable to open object file . " << object_file_path << endl;
        exit(1);
    }
    int line_count = 0;
    while(fscanf(object_file, "%[^\n]\n", buffer) != -1){
        line_count++;
        if(starts_with(buffer, "mtllib")){
            mtllib_name = move(string(buffer + 7));
        }else if(starts_with(buffer, "usemtl")){
            mtl_name = move(string(buffer + 7));
            auto it = material_table.find(mtl_name);
            if(it == material_table.end()){
                cerr << "can't find material " << mtl_name;
                exit(1);
            }
            mtl = it->second;
        }else if(starts_with(buffer, "vn")){
            sscanf(buffer, "vn %lf %lf %lf", &x, &y, &z);
            normals.emplace_back(x, y, z);
            //store vn
        }else if(starts_with(buffer, "v") && buffer[1] == ' '){
            sscanf(buffer, "v %lf %lf %lf", &x, &y, &z);
            vertices.emplace_back(x, y, z);
            //store v
        }else if(starts_with(buffer, "f")){
            int i = 1;
            int number = 0;
            int state;
            while(buffer[i] != '\0'){
                switch(buffer[i]){
                    case ' ': state = 0; number++; break;
                    case '/': state++; break;
                    default: break;
                }
                switch(state){
                    case 1: buffer[i] = ' '; break;
                    case 2: buffer[i] = ' '; state = 3; break;
                    default: break;
                }
                i++;
            }
            if(number == 3){
                sscanf(buffer + 1, "%d %d %d %d %d %d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
                Triangle *triangle = new Triangle(vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1], normals[vn1 - 1], normals[vn2 - 1], normals[vn3 - 1], mtl);
                triangles.push_back(triangle);
                // if(line_count > 14030){
                    // printf("%d %d %d %d %d %d %d %d\n", v1, vn1, v2, vn2, v3, vn3, v4, vn4);
                // }
                // printf("%d %d %d %d %d %d\n", v1, vn1, v2, vn2, v3, vn3);
            }else if(number == 4){
                // break the quadrilateral into triangles
              
                sscanf(buffer + 1, "%d %d %d %d %d %d %d %d", &v1, &vn1, &v2, &vn2, &v3, &vn3, &v4, &vn4);
                // if(line_count > 13990){
                //     printf("%d %d %d %d %d %d %d %d\n", v1, vn1, v2, vn2, v3, vn3, v4, vn4);
                // }
                Triangle *triangle1 = new Triangle(vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1], normals[vn1 - 1], normals[vn2 - 1], normals[vn3 - 1], mtl);
                Triangle *triangle2 = new Triangle(vertices[v1 - 1], vertices[v3 - 1], vertices[v4 - 1], normals[vn1 - 1], normals[vn3 - 1], normals[vn4 - 1], mtl);
                triangles.push_back(triangle1);
                triangles.push_back(triangle2);
                // printf("%d %d %d %d %d %d %d %d\n", v1, vn1, v2, vn2, v3, vn3, v4, vn4);
            }
            // deal with face
        }
    }
    // cout << "LINE COUNT " << line_count << endl;
}

void Model::read_material_lib(const char *material_lib_path){
    char buffer[1000];
    string mtl_name;
    Material *mtl;
    double r, g, b;
    double e;
    FILE *material_lib = fopen(material_lib_path, "r");
    if(material_lib == nullptr){
        cerr << "Unable to open material lib . " << material_lib_path << endl;
        exit(1);
    }
    while(fscanf(material_lib, "%[^\n]\n", buffer) != -1){
        if(starts_with(buffer, "newmtl")){
            mtl_name = move(string(buffer + 7));
            mtl = new Material();
            material_table[mtl_name] = mtl;
        }else if(starts_with(buffer, "Kd")){
            sscanf(buffer + 3, "%lf %lf %lf", &r, &g, &b);
            mtl->Kd << r, g, b;
        }else if(starts_with(buffer, "Ka")){
            sscanf(buffer + 3, "%lf %lf %lf", &r, &g, &b);
            mtl->Ka << r, g, b;
        }else if(starts_with(buffer, "Tf")){
            sscanf(buffer + 3, "%lf %lf %lf", &r, &g, &b);
            mtl->Tf << r, g, b;
        }else if(starts_with(buffer, "Ks")){
            sscanf(buffer + 3, "%lf %lf %lf", &r, &g, &b);
            mtl->Ks << r, g, b;
        }else if(starts_with(buffer, "Ni")){
            sscanf(buffer + 3, "%lf", &e);
            mtl->Ni = e;
        }else if(starts_with(buffer, "Ns")){
            sscanf(buffer + 3, "%lf", &e);
            mtl->Ns = e;
        }
    }
    for(auto &material:material_table){
        Material *mtl = material.second;
        double Kd_norm = mtl->Kd.norm();
        double Ks_norm = mtl->Ks.norm();
        double Ka_norm = mtl->Ka.norm();
        if(mtl->Ni > 1){
            mtl->material_type = mtl->TRANSPARENT;
        }else if(Ka_norm > 0){
            mtl->material_type = mtl->LIGHT;
        }
        if(Kd_norm == 0){
            mtl->diffuse = 0;
        }else if(Ks_norm > 0 && Kd_norm > 0){
            mtl->material_type = mtl->GLOSSY; // Diffuse and Specular
            mtl->diffuse = Kd_norm / Ks_norm;
        }else{
            mtl->diffuse = 1;
        }
        // printf("%s %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", material.first.c_str(), mtl->Kd[0], mtl->Kd[1], mtl->Kd[2], mtl->Ks[0],mtl->Ks[1],mtl->Ks[2], mtl->Ka[0], mtl->Ka[1], mtl->Ka[2]);
    }
}