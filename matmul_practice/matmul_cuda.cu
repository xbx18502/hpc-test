#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <cuda_runtime.h>
#include <iostream>

__global__ void matrixMultiplyKernel(double *A, double *B, double *C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < N && col < N) {
        double sum = 0.0;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

void multiplyMatricesCUDA(const double* A, const double* B, double* C, int N) {
    double *d_A, *d_B, *d_C;

    // Allocate device memory
    cudaMalloc((void **)&d_A, N*N*sizeof(double));
    cudaMalloc((void **)&d_B, N*N*sizeof(double));
    cudaMalloc((void **)&d_C, N*N*sizeof(double));

    // Copy data from host to device
    cudaMemcpy(d_A, A, N*N*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N*N*sizeof(double), cudaMemcpyHostToDevice);

    // Setup grid and block dimensions
    dim3 dimBlock(16, 16); // Block size of 16x16
    dim3 dimGrid((N + dimBlock.x - 1) / dimBlock.x, (N + dimBlock.y - 1) / dimBlock.y);

    // Launch the kernel
    matrixMultiplyKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, N);

    // Copy result back to host
    cudaMemcpy(C, d_C, N*N*sizeof(double), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

int main() {
    const int N = 1024; // Example size
    double *A, *B, *C;

    // Allocate matrices
    A = new double[N*N];
    B = new double[N*N];
    C = new double[N*N];

    // Seed random number generator
    srand(time(NULL));

    // Initialize matrices A and B with random double values
    for (int i = 0; i < N * N; ++i) {
        A[i] = rand() % 100;  // Random integers from 0 to 99
        B[i] = rand() % 100;  // Random integers from 0 to 99
    }

    // Call the CUDA matrix multiplication function
    multiplyMatricesCUDA(A, B, C, N);

    // Clean up resources
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
