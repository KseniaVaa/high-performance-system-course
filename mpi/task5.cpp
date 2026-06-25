#include <iostream>
#include <vector>
#include <mpi.h>

std::vector<int> create_local_array(long long N, int rank, int size) {
    long long local_N = N / size;
    std::vector<int> local_arr(local_N);

    long long offset = rank * local_N;
    for (long long i = 0; i < local_N; ++i) {
        local_arr[i] = static_cast<int>((i + offset) % 1000);
    }

    int max_count = 5;
    int target_max = 9999;
    for (int i = 0; i < max_count; ++i) {
        long long global_index = i * (N / max_count);
        if (global_index >= offset && global_index < offset + local_N) {
            local_arr[global_index - offset] = target_max;
        }
    }

    return local_arr;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    long long N = (argc > 1) ? std::stoll(argv[1]) : 10000000;
    long long local_N = N / size;

    std::vector<int> local_arr = create_local_array(N, rank, size);

    double start_time = MPI_Wtime();

    int local_max = local_arr[0];
    for (long long i = 0; i < local_N; ++i) {
        if (local_arr[i] > local_max) local_max = local_arr[i];
    }
    
    int global_max = 0;
    MPI_Allreduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    
    long long local_count = 0;
    for (long long i = 0; i < local_N; ++i) {
        if (local_arr[i] == global_max) local_count++;
    }
    
    long long global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    
    if (rank == 0) {
        std::cout << "Data size: " << N << std::endl;
        std::cout << "Count: " << global_count << std::endl;
        std::cout << "Time (sec): " << (end_time - start_time) << std::endl;
    }
    MPI_Finalize();
    return 0;
}