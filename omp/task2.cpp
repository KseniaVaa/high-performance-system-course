#include <iostream>
#include <vector>
#include <numeric>
#include <omp.h>

int main(int argc, char* argv[]) {
    long long N = (argc > 1) ? std::stoll(argv[1]) : 1000000;

    std::vector<int> data(N, 1);
    long long sum = 0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < N; ++i) {
        sum += data[i];
    }

    double end_time = omp_get_wtime();

    std::cout << "Data size: " << N << std::endl;
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Time (sec): " << (end_time - start_time) << std::endl;

    return 0;
}