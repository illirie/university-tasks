#define kernel addKernel
#include "mainGPU.h"

__global__ 
void addKernel(double* c, double* a, double* b, unsigned int size) {
    for (int i = blockIdx.x * blockDim.x + threadIdx.x; i < size; i += blockDim.x * gridDim.x)
        c[i] = a[i] + b[i];
}