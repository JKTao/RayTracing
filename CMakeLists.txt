cmake_minimum_required(VERSION 2.8) 
project(RayTracing) 
set(CMAKE_CXX_FLAGS " -O2 -pthread -march=native")
find_package(Eigen3 REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories(${EIGEN3_INCLUDE_DIRS})

add_executable(RayTracing src/main.cc src/Ray.cc src/Object.cc src/Camera.cc src/Ultility.cc) 

# target_link_libraries(testCode PRIVATE ${OpenCV_LIBRARIES}) 
