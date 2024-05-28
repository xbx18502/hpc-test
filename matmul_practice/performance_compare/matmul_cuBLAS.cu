#include <cublas_v2.h>
#include <cuda_runtime.h>
#include <iostream>
#include <time.h>

int main(int argc, char **argv) {
    int n  = strtol(argv[1], NULL, 10);  // size of array
    printf("size = %d\n",n);
    cublasHandle_t handle;
    float *d_A, *d_B, *d_C;
    float *A, *B, *C; // array in host memory
    float alpha = 1.0f, beta = 0.0f;
    size_t size = n * n * sizeof(float);

    A = new float[n*n];
    B = new float[n*n];
    C = new float[n*n]; // Allocate host memory to store result

    // Initialize A , B
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i*n+j] = float(i + j);
            B[i*n+j] = float(i - j);
        }
    }

    // Allocate A ,B ,C on GPU
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    // copy A and B to GPU
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    cublasCreate(&handle);

    clock_t start = clock();
    //  C = alpha * A * B + beta * C
    cublasSgemm(handle, CUBLAS_OP_T, CUBLAS_OP_T, n, n, n, &alpha, d_A, n, d_B, n, &beta, d_C, n);
    clock_t end = clock();
    // copy result back to host
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("size = %d, time :%f second\n",n,time_spent);
    
    // print part of matrix
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%f ",C[i * n + j]) ;
        }
        std::cout << "\n";
    }

    // free resources
    cublasDestroy(handle);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}

/*
module load cuda/12.0.0
nvcc -o matmul_cuBLAS.elf  matmul_cuBLAS.cu -lcublas

*/
