#include <iostream>
#include <vector>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = (argc > 1) ? std::stoll(argv[1]) : 2000;
    int local_rows = N / size;

    std::vector<double> local_A(local_rows * N, 1.0);
    std::vector<double> x(N, 2.0);
    std::vector<double> local_y(local_rows, 0.0);

    double start_time = MPI_Wtime();

    MPI_Bcast(x.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_rows; ++i) {
        double sum = 0.0;
        for (int j = 0; j < N; ++j) {
            sum += local_A[i * N + j] * x[j];
        }
        local_y[i] = sum;
    }

    std::vector<double> y;
    if (rank == 0) y.resize(N);

    MPI_Gather(local_y.data(), local_rows, MPI_DOUBLE, y.data(), local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Data size: " << N << std::endl;
        std::cout << "Time (sec): " << (end_time - start_time) << std::endl;
    }
    MPI_Finalize();
    return 0;
}