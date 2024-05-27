#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

//#define N 1200  // Matrix size, ensure it's divisible by number of MPI processes

// Function to fill a matrix with data
void fill_matrix(double *matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = (double)(i % 100);  // Simple sequential fill for example
    }
}

// Matrix multiplication using OpenMP
void matrix_multiply(double *A, double *B, double *C, int rowsA, int colsA, int colsB) {
    #pragma omp parallel for
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            double sum = 0.0;
            for (int k = 0; k < colsA; ++k) {
                sum += A[i * colsA + k] * B[k * colsB + j];
            }
            C[i * colsB + j] = sum;
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int N = 3000;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process will handle a part of the matrix
    int rows_per_process = N / size;

    double *A, *B, *C, *subA, *subC;
    B = (double *)malloc(N * N * sizeof(double));  // B is fully replicated in each process
    subA = (double *)malloc(rows_per_process * N * sizeof(double));
    subC = (double *)malloc(rows_per_process * N * sizeof(double));

    // Root process initializes full matrices and distributes portions of A
    if (rank == 0) {
        A = (double *)malloc(N * N * sizeof(double));
        C = (double *)malloc(N * N * sizeof(double));
        fill_matrix(A, N, N);
    }
    fill_matrix(B, N, N);
    double startTime = omp_get_wtime();
    // Scatter rows of matrix A to different processes
    MPI_Scatter(A, rows_per_process * N, MPI_DOUBLE, subA, rows_per_process * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Local computation using OpenMP
    matrix_multiply(subA, B, subC, rows_per_process, N, N);

    // Gather the partial results back to the root process
    MPI_Gather(subC, rows_per_process * N, MPI_DOUBLE, C, rows_per_process * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double endTime = omp_get_wtime();
    printf("Size of matrix = %d, time = %f\n\n",N,endTime-startTime);
    // Optionally print some results at the root
    if (rank == 0) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                printf("%.2f ", C[i * N + j]);
            }
            printf("\n");
        }
        free(A);
        free(C);
    }

    free(B);
    free(subA);
    free(subC);
    MPI_Finalize();
    return 0;
}

/*
module load compiler/2024.0.2 mpi/2021.11
mpiicx -qopenmp matmul_omp_mpi.c -o matmul_omp_mpi.elf


mpiicx -O0 -pg -qopenmp matmul_omp_mpi.c -o matmul_omp_mpi.elf

*/