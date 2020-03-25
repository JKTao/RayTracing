#pragma once
#include <chrono>

class TicToc
{
public:
    TicToc();
    void tic();
    double toc();
    std::chrono::time_point<std::chrono::system_clock> start, end;
};
