#ifndef SPH_KERNELS_H
#define SPH_KERNELS_H

#include <iostream>
#include <vector>

float normVector(std::vector<float> v);

float Wpoly6(std::vector<float> r, float h);

std::vector<float> gradWspiky(std::vector<float> r, float h); 

#endif