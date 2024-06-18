//#include <cblas.h>   //if use openblas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <mkl.h> //if use mkl blas
int main(int argc, char **argv) {
    long long int n  = strtol(argv[1], NULL, 10);  // size of array
    printf("size = %d\n",n);
    
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

    long long int timesOfOperation = n*n*(2*n-1);
    printf("Size of matrix = %d, time = %f, TFLOPS = %f\n\n",n,time_spent, timesOfOperation/time_spent/1e12);
    
    // print part of matrix
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%f ", C[i * n + j]);
        }
        printf("\n");
    }

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

module compiler/2023.0.0
module load mkl/2023.0.0
icpx -qmkl -O -o matmul_BLAS.elf matmul_BLAS.cpp
module purge

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