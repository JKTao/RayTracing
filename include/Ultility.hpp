#pragma once
#include <chrono>
#include <random>

class Random{
public:
    std::uniform_real_distribution<double> uniform_double;
    std::default_random_engine engine;
    Random();
    double double_rand();
};

class TicToc
{
public:
    TicToc();
    void tic();
    double toc();
    std::chrono::time_point<std::chrono::system_clock> start, end;
};
