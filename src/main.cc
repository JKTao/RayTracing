#include <Eigen/Core>
#include <iostream>
// #include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "Model.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "RayTracer.hpp"
#include "Scene.hpp"
#include "Ultility.hpp"

int logging = 0;
cv::Vec3b convertToMat(Eigen::Vector3d pixel) {
    double RGB[3];
    for (int i = 0; i < 3; i++) {
        pixel[i] = (pixel[i] > 1) ? 1 : pixel[i];
        RGB[i] = pow(pixel[i], 0.4) * 254 + 0.5;
    }
    // BGR
    return cv::Vec3b(RGB[2], RGB[1], RGB[0]);
}

using namespace std;
using Vec = Eigen::Vector3d;

int main(int argc, char *argv[]) {
    string config_file_path = "../config/veach_mis.yaml";
    if (argc == 2) {
        config_file_path = move(string(argv[1]));
    }
    Model *model = new Model(config_file_path);
    Scene *scene = new Scene(model->triangles);
    Camera *camera = new Camera(model->fov, model->eye, model->front, model->up,
                                model->height, model->width);
    RayTracer *raytracer = new RayTracer(scene, camera, model->max_depth);
    int iter_times = model->iteration_times;
    string save_image_path = model->save_image_path;
    cv::Mat image(model->height, model->width, CV_8UC3);
    TicToc timer;
    int h = camera->height;
    int w = camera->width;
    vector<Vec> sum_pixels(h * w, Vec(0, 0, 0));
    vector<Vec> avg_pixels(h * w);
    for (int k = 0; k < iter_times; k++) {
        // #pragma omp parallel for schedule(dynamic, 1)

// #pragma omp parallel num_threads(8)
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                Ray ray = camera->generate_ray(j, i);
                Vec pixel = raytracer->ray_tracing(ray, 0);
                sum_pixels[i * w + j] += pixel;
            }
        }
        if (k % 25 == 0) {
// #pragma omp parallel num_threads(8)
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    avg_pixels[i * w + j] = sum_pixels[i * w + j] / (k + 1);
                    image.at<cv::Vec3b>(i, j) =
                        convertToMat(avg_pixels[i * w + j]);
                }
            }
            cerr << "iteration " << k << " cost " << timer.toc() << " ms"
                 << endl;
            cv::imwrite(save_image_path, image);
        }
    }
#pragma omp parallel num_threads(8)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            avg_pixels[i * w + j] = sum_pixels[i * w + j] / iter_times;
            image.at<cv::Vec3b>(i, j) = convertToMat(avg_pixels[i * w + j]);
        }
    }
    cerr << "iteration " << iter_times << " cost " << timer.toc() << " ms"
         << endl;
    cv::imwrite(save_image_path, image);
}
