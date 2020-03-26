#include "Model.hpp"
#include "Ultility.hpp"
#include <chrono>
#include <iostream>

using namespace std;
int main(){
    TicToc timer;
    Model model;

    model.read_material_lib("../models/diningroom/diningroom.mtl");
    cerr << timer.toc() << "ms" << endl;
    model.read_object_file("../models/diningroom/diningroom.obj");
    cerr << timer.toc() << "ms" << endl;
    cerr << model.triangles.size() << endl;
    return 0;
}