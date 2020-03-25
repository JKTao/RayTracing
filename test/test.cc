#include "Model.hpp"
#include "Ultility.hpp"
#include <chrono>
#include <iostream>

using namespace std;
int main(){
    TicToc timer;
    Model model;

    model.read_material_lib("../models/cbox/cbox.mtl");
    cerr << timer.toc() << "ms" << endl;
    model.read_object_file("../models/cbox/cbox.obj");
    cerr << timer.toc() << "ms" << endl;
    return 0;
}