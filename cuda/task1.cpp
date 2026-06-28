#include <iostream>
#include <cstdio>
#include <cuda_runtime.h>

__global__ void hello_kernel() {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    printf("CUDA thread %d in block %d\n", tid, blockIdx.x);
}

int main() {
    hello_kernel<<<2, 4>>>();
    cudaDeviceSynchronize();
    return 0;
}