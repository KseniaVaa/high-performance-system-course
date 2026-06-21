#include <iostream>
#include <vector>
#include <omp.h>

int main(int argc, char* argv[]) {
    int N = (argc > 1) ? std::stoll(argv[1]) : 2000;
    std::vector<double> A(N * N, 1.0);
    std::vector<double> x(N, 2.0);
    std::vector<double> y(N, 0.0);
    
    double start_time = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        double sum = 0;
        for (int j = 0; j < N; ++j) {
            sum += A[i * N + j] * x[j];
        }
        y[i] = sum;
    }
    
    double end_time = omp_get_wtime();

    std::cout << "Data size: " << N << std::endl;
    std::cout << "Time (sec): " << (end_time - start_time) << std::endl;

    return 0;
}