#include "Ray.hpp"
#include "Object.hpp"
#include "RayTracer.hpp"
#include "Scene.hpp"
#include "Model.hpp"
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>

cv::Vec3b convertToMat(Eigen::Vector3d pixel){
    double RGB[3];
    for(int i = 0; i < 3; i++){
        RGB[i] = (pixel[i] + 1) * 128;
    }
    //BGR model
    return cv::Vec3b(RGB[2], RGB[1], RGB[0]);
}
//TODO: Render the model normal and other model.
int main(){
    using Vec = Eigen::Vector3d;
    // Sphere *light = new Sphere(600, Vec(50,681.6-0.27,81.6),Vec(12,12,12),  Vec(0, 0, 0), 1, 1);
    // Sphere *sphere = new Sphere(16.5,Vec(27,16.5,47),  Vec(0, 0, 0),Vec(1,1,1)*.999, 1, 1);
    // std::vector<Sphere*> objects = {light, sphere};
    // Scene *scene = new Scene(std::vector<Object*>(objects.begin(), objects.end()));
    // Camera *camera = new Camera(28.8, Vec(50, 52, 295.6), Vec(0, -0.042612, -1), Vec(1, 0, 0), 768, 1024);//TODO set camera parameter;
    // RayTracer *raytracer = new RayTracer(scene, camera);
    // cv::Mat image(768, 1024, CV_8UC3);
    // Vec pixel;
    // for(int i = 0; i < camera->height; i++){
    //     for(int j = 0; j < camera->width; j++){
    //         pixel = raytracer->ray_tracing(camera->generate_ray(j, i), 0);
    //         std::cout << pixel[0] << " " <<  pixel[1] << " " << pixel[2] << std::endl;
    //         image.at<cv::Vec3b>(i, j) = convertToMat(pixel);
    //     }
    // }
    Model *model = new Model();
    model->read_material_lib("../models/diningroom/diningroom.mtl");
    model->read_object_file("../models/diningroom/diningroom.obj");
    Scene *scene = new Scene(model->triangles);
    Camera *camera = new Camera(90, Vec(0, 10, 10), Vec(0, 0, -1), Vec(0, 1, 0), 800, 800);
    RayTracer *raytracer = new RayTracer(scene, camera);
    cv::Mat image(800, 800, CV_8UC3, cv::Vec3b(0, 0, 0));
    Vec pixel;
    for(int i = 0; i < camera->height; i++){
        for(int j = 0; j < camera->width; j++){
            pixel = raytracer->ray_tracing(camera->generate_ray(j, i), 0);
            image.at<cv::Vec3b>(i, j) = convertToMat(pixel);
        }
    }
    // for(int i = 0; i < camera->height; i++){
    //     for(int j = 0; j < camera->width; j++){
    //         image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, i * 255.0/camera->width, j * 255.0/camera->height);
    //         std::cout << image.at<cv::Vec3b>(i, j) << std::endl;
    //     }
    // }

    cv::imwrite("./test.png", image);
    return 0;
}