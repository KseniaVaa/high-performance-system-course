#include <iostream>
#include <omp.h>
#include <cstdlib>

int main(int argc, char* argv[]) {
    long long N = (argc > 1) ? std::stoll(argv[1]) : 1000000000; // Всего испытаний
    long long count = 0;

    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        unsigned int seed = 42 + omp_get_thread_num();
        long long local_count = 0;

        #pragma omp for
        for (long long i = 0; i < N; ++i) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                local_count++;
            }
        }

        #pragma omp atomic
        count += local_count;
    }

    double end_time = omp_get_wtime();
    double pi = 4.0 * count / N;

    std::cout << "Data size: " << N << std::endl;
    std::cout << "Pi: " << pi << std::endl;
    std::cout << "Time (sec): " << (end_time - start_time) << std::endl;
    return 0;
}