#include <iostream>
#include <vector>
#include <cuda_runtime.h>
#include <string>

__global__ void sum_kernel(const int* arr, long long* out, long long N) {
    long long i = blockIdx.x * blockDim.x + threadIdx.x;
    long long stride = blockDim.x * gridDim.x;

    long long local_sum = 0;
    for (long long k = i; k < N; k += stride) {
        local_sum += arr[k];
    }

    atomicAdd((unsigned long long*)out, (unsigned long long)local_sum);
}

int main(int argc, char** argv) {
    long long N = (argc > 1) ? std::stoll(argv[1]) : 1000000;
    std::vector<int> h_arr(N, 1);

    int *d_arr;
    long long *d_out;
    cudaMalloc(&d_arr, N * sizeof(int));
    cudaMalloc(&d_out, sizeof(long long));

    cudaMemcpy(d_arr, h_arr.data(), N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemset(d_out, 0, sizeof(long long));

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    int threadsPerBlock = 256;
    int blocksPerGrid = 256;

    sum_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_arr, d_out, N);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);

    long long h_out = 0;
    cudaMemcpy(&h_out, d_out, sizeof(long long), cudaMemcpyDeviceToHost);

    std::cout << "Data size: " << N << std::endl;
    std::cout << "Sum: " << h_out << std::endl;
    std::cout << "Time (sec): " << (ms / 1000.0f) << std::endl;

    cudaFree(d_arr);
    cudaFree(d_out);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    return 0;
}