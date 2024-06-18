#define double float

#include <stdio.h>
#include <stdlib.h>
#include <openacc.h>
#include <time.h>



// Function to initialize the matrix with random numbers
void initializeMatrix(double* mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = rand() % 100;  // Random double between 0 and 99
    }
}

// Function to perform matrix multiplication using OpenACC
void multiplyMatrices_0(double* A, double* B, double* C, int numRowsA, int numColsA, int numColsB) {
    #pragma acc data copyin(A[0:numRowsA*numColsA], B[0:numColsA*numColsB]) copyout(C[0:numRowsA*numColsB])
    {
        #pragma acc kernels
        for (int i = 0; i < numRowsA; i++) {
            for (int j = 0; j < numColsB; j++) {
                double sum = 0.0;
                for (int k = 0; k < numColsA; k++) {
                    sum += A[i * numColsA + k] * B[k * numColsB + j];
                }
                C[i * numColsB + j] = sum;
            }
        }
    }
}
// Function to perform matrix multiplication using OpenACC
void multiplyMatrices(double* A, double* B, double* C, int numRowsA, int numColsA, int numColsB) {
    #pragma acc data copyin(A[0:numRowsA*numColsA], B[0:numColsA*numColsB]) copyout(C[0:numRowsA*numColsB])
    {
        #pragma acc parallel loop 
        for (int i = 0; i < numRowsA; i++) {
            for (int j = 0; j < numColsB; j++) {
                double sum = 0.0;
                for (int k = 0; k < numColsA; k++) {
                    sum += A[i * numColsA + k] * B[k * numColsB + j];
                }
                C[i * numColsB + j] = sum;
            }
        }
    }
}
void run(int N){
    int numRowsA = N, numColsA = N, numColsB = N;
    double *A = (double*)malloc(numRowsA * numColsA*sizeof(double));
    double *B = (double*)malloc(numColsA * numColsB*sizeof(double));
    double *C = (double*)malloc(numRowsA * numColsB*sizeof(double));

    // Initialize matrices with random data
    //srand(2024);
    // initializeMatrix(A, numRowsA, numColsA);
    // initializeMatrix(B, numColsA, numColsB);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i*N+j] = i + j;
            B[i*N+j] = i - j;
        }
    }



    clock_t start = clock();
    
    // Perform matrix multiplication
    multiplyMatrices(A, B, C, numRowsA, numColsA, numColsB);
    clock_t end = clock();
   
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    
    // Optionally, print the result matrix C
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%f ",C[i * numColsB + j]);
        }
        printf("\n");
    }
    printf("size = %d, time :%f second\n",N,time_spent);
    // Clean up resources
    // delete[] A;
    // delete[] B;
    // delete[] C;
}
int main(int argc, char **argv) {
    int n  = strtol(argv[1], NULL, 10);  // size of array
    printf("size = %d\n",n);
    run(n);


    return 0;
}

/*
module load nvhpc/23.3
nvc -Minfo=accel -acc -gpu=ccall matmul_acc.cpp -o matmul_acc.elf
nvc -pg -O0 -Minfo=accel -acc -gpu=ccall matmul_acc.cpp -o matmul_acc.elf

./matmul_acc.elf
gprof matmul_acc.elf gmon.out > analysis1.txt

*/