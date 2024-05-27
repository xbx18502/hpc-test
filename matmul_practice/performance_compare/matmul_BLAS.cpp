#include <cblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
int main() {
    int n = 3000;
    
    double *A = (double *)malloc(n * n * sizeof(double));
    double *B = (double *)malloc(n * n * sizeof(double));
    double *C = (double *)malloc(n * n * sizeof(double));

    // 
    // for (int i = 0; i < n * n; i++) {
    //     A[i] = (double)(i % 255);
    //     B[i] = (double)((i + 1) % 255);
    // }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i*n+j] = i + j;
            B[i*n+j] = i - j;
        }
    }


    auto start = std::chrono::high_resolution_clock::now();
    //clock_t start = clock();
    //  c = a*b
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                n, n, n, 1.0, A, n, B, n, 0.0, C, n);
    //clock_t end = clock();
    //double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    double time_spent = elapsed_seconds.count();


    printf("Size of matrix = %d, time = %f\n\n",n,time_spent);
    printf("C[10][20] = %f, C[20][30] = %f, C[30][40] = %f, C[40][50] = %f"
    , C[10*n+20],C[20*n+30],C[30*n+40],C[40*n+50]);
    // print part of matrix
    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         printf("%f ", C[i * n + j]);
    //     }
    //     printf("\n");
    // }

    free(A);
    free(B);
    free(C);
    return 0;
}

/*
OpenBLAS
*/
/*

g++ -O3 -o matmul_BLAS.elf matmul_BLAS.cpp -lopenblas
*/
/*
#!/bin/bash
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 00:30:00
#SBATCH -N 1
#SBATCH -c 16

./matmul_BLAS.elf 

date
*/