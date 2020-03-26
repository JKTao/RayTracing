#include <Model.hpp>
#include <Ray.hpp>
#include <Object.hpp>
#include <iostream>
#include <cstdio>

using namespace std;
bool starts_with(char *str, const char *pattern){
    int i = 0;
    while(str[i] == pattern[i] && str[i] != '\0'){
        i++;
    }
    return (pattern[i] == '\0');
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
    while(fscanf(object_file, "%[^\n]\n", buffer) != -1){
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
                // printf("%d %d %d %d %d %d\n", v1, vn1, v2, vn2, v3, vn3);
            }else if(number == 4){
                // break the quadrilateral into triangles
                sscanf(buffer + 1, "%d %d %d %d %d %d %d %d", &v1, &vn1, &v2, &vn2, &v3, &vn3, &v4, &vn4);
                Triangle *triangle1 = new Triangle(vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1], normals[vn1 - 1], normals[vn2 - 1], normals[vn3 - 1], mtl);
                Triangle *triangle2 = new Triangle(vertices[v1 - 1], vertices[v3 - 1], vertices[v4 - 1], normals[vn1 - 1], normals[vn3 - 1], normals[vn4 - 1], mtl);
                triangles.push_back(triangle1);
                triangles.push_back(triangle2);
                // printf("%d %d %d %d %d %d %d %d\n", v1, vn1, v2, vn2, v3, vn3, v4, vn4);
            }
            // deal with face
        }
    }
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
        }else if(starts_with(buffer, "Ni")){
            sscanf(buffer + 3, "%lf", &e);
            mtl->Ni = e;
        }
    }
}