cmake_minimum_required(VERSION 2.8) 
project(RayTracing) 
set(CMAKE_CXX_COMPILER "g++-7")
set(CMAKE_CXX_FLAGS " -O2 -pthread -march=native -std=c++17")
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(RayTracing src/main.cc src/Ray.cc src/Object.cc src/Camera.cc src/Scene.cc src/Ultility.cc src/RayTracer.cc src/Model.cc)

target_link_libraries(RayTracing ${OpenCV_LIBRARIES}) 
