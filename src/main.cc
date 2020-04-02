#include "Ray.hpp"
#include "Object.hpp"
#include "RayTracer.hpp"
#include "Scene.hpp"
#include "Model.hpp"
#include "Ultility.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Core>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

int logging = 0;
cv::Vec3b convertToMat(Eigen::Vector3d pixel){
    double RGB[3];
    for(int i = 0; i < 3; i++){
        pixel[i] = (pixel[i] > 1)?1:pixel[i];
        RGB[i] = pow(pixel[i], 0.4) * 254 + 0.5;
    }
    //BGR
    return cv::Vec3b(RGB[2], RGB[1], RGB[0]);
}

using namespace std;
using Vec = Eigen::Vector3d;


int main(int argc, char *argv[]){
    string config_file_path = "../config/veach_mis.yaml";
    if(argc == 2){
        config_file_path =  move(string(argv[1]));
    }
    Model *model = new Model(config_file_path);
    Scene *scene = new Scene(model->triangles);
    Camera *camera = new Camera(model->fov, model->eye, model->front, model->up, model->height, model->width);
    RayTracer *raytracer = new RayTracer(scene, camera, model->max_depth);
    int iter_times = model->iteration_times;
    string save_image_path = model->save_image_path;
    cv::Mat image(model->height, model->width, CV_8UC3, cv::Vec3b(0, 0, 0));
    TicToc timer;
    #pragma omp parallel for schedule(dynamic, 1)
    for(int i = 0; i < camera->height; i++){
        for(int j = 0; j < camera->width; j++){
            Vec sum_pixel(0, 0, 0);
            Vec pixel;
            //(380, 247) a special pixel in first plane
            if(j == 693 && i == 508){
                logging = 0;
            }else if(j == 693 && i == 508){
                logging = 0;
            }else{
                logging = 0;
            }
            for(int k = 0; k < iter_times; k++){
                // cout << i << " " << j << " " << k << endl;
                Ray ray = camera->generate_ray(j, i);
                if(logging == 1){
                    cout << "RAYDIRECT" << ray.direction[0] << " " << ray.direction[1] << " " << ray.direction[2] << endl;
                    cout << "RAYORIGIN" << ray.origin[0] << " " << ray.origin[1] << " " << ray.origin[2] << endl;
                }
                pixel = raytracer->ray_tracing(ray, 0);
                // cout << i << " " << j << " " << pixel[0] << " " << pixel[1] << " " <<  pixel[2] << endl;
                sum_pixel = sum_pixel + pixel;
                // cv::Vec3b pixel_ = image.at<cv::Vec3b>(i, j);
                // double x_ = pixel[0], y_ = pixel[1], z_ = pixel[2];
                // cout << i << " " << j << " " << x_ << " " << y_ << " " << z_ << endl;
            }
            sum_pixel = sum_pixel/iter_times;
            image.at<cv::Vec3b>(i, j) = convertToMat(sum_pixel);
            if(logging == 2){
                cout << i << " " << j << " " << (int)(image.at<cv::Vec3b>(i, j)[0]) << " " << (int)(image.at<cv::Vec3b>(i, j)[1]) << " " << (int)(image.at<cv::Vec3b>(i, j)[2]) << endl;
            }
            // if(i == 379 || i == 378){
            //     image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 254, 0);
            // }
        }
    }
    cerr << "iteration " << iter_times << " cost " << timer.toc() << " ms" << endl;
    cv::imwrite(save_image_path, image);
    return 0;
}
