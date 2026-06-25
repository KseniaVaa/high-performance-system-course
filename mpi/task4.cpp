#include <iostream>
#include <random>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    long long N = (argc > 1) ? std::stoll(argv[1]) : 1000000000;
    long long local_N = N / size;
    
    double start_time = MPI_Wtime();
    
    unsigned int seed = 42 + rank;
    
    long long local_count = 0;
    for (long long i = 0; i < local_N; ++i) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }
    
    long long total_count = 0;
    MPI_Reduce(&local_count, &total_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    
    if (rank == 0) {
        double pi = 4.0 * total_count / N;
        std::cout << "Data size: " << N << std::endl;
        std::cout << "Pi: " << pi << std::endl;
        std::cout << "Time (sec): " << (end_time - start_time) << std::endl;
    }
    MPI_Finalize();
    return 0;
}