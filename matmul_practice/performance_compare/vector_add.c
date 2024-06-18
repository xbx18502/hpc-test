#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    int N  = strtol(argv[1], NULL, 10);  // size of array
    printf("size = %d\n",N);
    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));
    double *C = (double *)malloc(N * sizeof(double));

    // 
    for (long long int i = 0; i < N; i++) {
        A[i] = i;
        B[i] = N - i;
    }

    double start = omp_get_wtime(); // 

    // 
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    double end = omp_get_wtime(); // 
    double time_spent = end - start;

    printf("Time spent: %f seconds\n", time_spent);

    // 
    double flops = (double)N / time_spent; // 
    double tflops = flops / 1e12; // 
    printf("Estimated TFLOPS: %f\n", tflops);

    // 
    free(A);
    free(B);
    free(C);

    return 0;
}
/*
gcc -fopenmp -O3 vector_add.c -o vector_add.elf
*/