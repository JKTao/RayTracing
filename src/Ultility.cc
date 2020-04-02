#include "Ultility.hpp"
#include <string>
#include <chrono>

Random::Random():uniform_double(0, 1){
}

double Random::double_rand(){
    return uniform_double(engine);
}

TicToc::TicToc() {   
    tic();
}   

void TicToc::tic(){   
    start = std::chrono::system_clock::now();
}   

double TicToc::toc(){   
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    start = end;
    return elapsed_seconds.count() * 1000;
}   

