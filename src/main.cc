#include "Ray.hpp"
#include "Object.hpp"
#include "RayTracer.hpp"
#include "Scene.hpp"
#include "Model.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Core>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Vec3b convertToMat(Eigen::Vector3d pixel){
    double RGB[3];
    for(int i = 0; i < 3; i++){
        pixel[i] = (pixel[i] > 1)?1:pixel[i];
        RGB[i] = (pixel[i] + 1.00) * 127;
    }
    //BGR
    return cv::Vec3b(RGB[2], RGB[1], RGB[0]);
}

using namespace std;
using Vec = Eigen::Vector3d;
int main(){
    string config_file_path = "../config/veach_mis.yaml";
    Model *model = new Model(config_file_path);
    // cout << model->fov << " " << model->height << " " << model->width << " " << endl;
    // cout << model->triangles.size() << " " << endl;
    cout << model->up[0] << " "  << model->up[1] << " " << model->up[2] << endl;
    cout << model->front[0] << " "  << model->front[1] << " " << model->front[2] << endl;
    Scene *scene = new Scene(model->triangles);
    Camera *camera = new Camera(model->fov, model->eye, model->front, model->up, model->height, model->width);
    RayTracer *raytracer = new RayTracer(scene, camera);
    cv::Mat image(model->height, model->width, CV_8UC3, cv::Vec3b(0, 0, 0));
    Vec pixel;
    for(int i = 0; i < camera->height; i++){
        for(int j = 0; j < camera->width; j++){
            pixel = raytracer->ray_tracing(camera->generate_ray(j, i), 0);
            image.at<cv::Vec3b>(i, j) = convertToMat(pixel);
            cv::Vec3b pixel_ = image.at<cv::Vec3b>(i, j);
            double x_ = pixel[0], y_ = pixel[1], z_ = pixel[2];
            cout << i << " " << j << " " << x_ << " " << y_ << " " << z_ << endl;
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