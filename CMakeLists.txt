cmake_minimum_required(VERSION 2.8) 
project(RayTracing) 
set(CMAKE_CXX_FLAGS " -O2 -pthread -march=native -std=c++17")
find_package(Eigen3 REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories(${EIGEN3_INCLUDE_DIRS})

add_executable(RayTracing src/main.cc src/Ray.cc src/Object.cc src/Camera.cc src/Scene.cc src/Ultility.cc src/RayTracer.cc)

# target_link_libraries(testCode PRIVATE ${OpenCV_LIBRARIES}) 
