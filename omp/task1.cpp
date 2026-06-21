#include <iostream>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int num = omp_get_num_threads();

        #pragma omp critical
        std::cout << "OpenMP Thread " << id << " of " << num << "\n";
    }
    return 0;
}