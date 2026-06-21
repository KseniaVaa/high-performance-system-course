#include <iostream>
#include <vector>
#include <omp.h>

int main(int argc, char* argv[]) {
    long long N = (argc > 1) ? std::stoll(argv[1]) : 1000000;

    std::vector<int> arr(N);
    for (int i = 0; i < N; ++i) {
        arr[i] = i % 1000; 
    }

    int max_count = 5;
    int target_max = 9999;

    for (int i = 0; i < max_count; ++i) {
        long long index = i * (N / max_count);
        arr[index] = target_max;
    }

    double start_time = omp_get_wtime();

    int max_val = arr[0];
    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < N; ++i) {
        if (arr[i] > max_val) max_val = arr[i];
    }

    int count = 0;
    #pragma omp parallel for reduction(+:count)
    for (int i = 0; i < N; ++i) {
        if (arr[i] == max_val) count++;
    }

    double end_time = omp_get_wtime();
    std::cout << "Data size: " << N << std::endl;
    std::cout << "Count: " << count << std::endl;
    std::cout << "Time (sec): " << (end_time - start_time) << std::endl;
    return 0;
}