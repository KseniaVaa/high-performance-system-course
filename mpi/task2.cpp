#include <iostream>
#include <vector>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int N = (argc > 1) ? std::stoll(argv[1]) : 1000000;
    int local_N = N / size;
    std::vector<int> local_arr(local_N, 1);
    
    double start_time = MPI_Wtime();

    long long local_sum = 0;
    for (int i = 0; i < local_N; ++i) {
        local_sum += local_arr[i];
    }
    
    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Data size: " << N << std::endl;
        std::cout << "Sum: " << global_sum << std::endl;
        std::cout << "Time (sec): " << (end_time - start_time) << std::endl;
    }
    MPI_Finalize();
    return 0;
}