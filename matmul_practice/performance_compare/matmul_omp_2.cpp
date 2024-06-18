#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// allocate matrix
// double** allocateMatrix(int rows, int cols) {
//     double** matrix = (double**) malloc(rows * sizeof(double*));
//     for (int i = 0; i < rows; i++) {
//         matrix[i] = (double*) malloc(cols * sizeof(double));
//     }
//     return matrix;
// }
// double** allocateMatrix(int rows, int cols) {
//     double** matrix = (double**) malloc(rows * sizeof(double*));
//     for (int i = 0; i < rows; i++) {
//         posix_memalign((void**)&matrix[i], 64, cols * sizeof(double));  // 64-byte alignment for SIMD
//     }
//     return matrix;
// }


double** allocateMatrix(int rows, int cols) {
    double** matrix = (double**) malloc(rows * sizeof(double*));
    if (matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for matrix pointers.\n");
        return NULL;  // 
    }

    for (int i = 0; i < rows; i++) {
        if (posix_memalign((void**)&matrix[i], 64, cols * sizeof(double)) != 0) {
            fprintf(stderr, "Memory allocation failed for row %d.\n", i);
            // 
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;  //
        }
    }
    return matrix;
}

void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// void matrixMultiply(double** a, double** b, double** c, int n) {
//     int i, j, k;
//     double sum;

//     #pragma omp parallel for private(i, j, k, sum) shared(a, b, c)
//     for (i = 0; i < n; i++) {
//         for (j = 0; j < n; j++) {
//             sum = 0.0;
//             for (k = 0; k < n; k++) {
//                 sum += a[i][k] * b[k][j];
//             }
//             c[i][j] = sum;
//         }
//     }
// }
void matrixMultiply(double** a, double** b, double** c, int n) {
    int i, j, k;
    double sum;

    #pragma omp parallel for private(i, j, k, sum) shared(a, b, c)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            sum = 0.0;
            #pragma omp simd reduction(+:sum)
            for (k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
}
void matrixMultiply_serial(double** a, double** b, double** c, int n) {
    int i, j, k;
    double sum;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            sum = 0.0;
            for (k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
}

bool compare(double** C, double** D, int n){
    bool ans = true;
    int numRowsA = n;
    int numColsB = n;
    #pragma omp parallel for
    for(int i=0;i<numRowsA;i++){
        for(int j=0;j<numColsB;j++){
            if((C[i][j]-D[i][j])/C[i][j]>0.0001){
                printf("error");
                ans = false;
            }
        }
    }
    return ans;

}

int main(int argc, char **argv) {
    int n;
    if(argc>1){
        n  = strtol(argv[1], NULL, 10);  // size of array
    }
    else{
        n=1000;
    }
    printf("size = %d\n",n);
    double **a, **b, **c, **d;

    // allocate matrix
    a = allocateMatrix(n, n);
    b = allocateMatrix(n, n);
    c = allocateMatrix(n, n);
    d = allocateMatrix(n,n);
    // initilaize a ,b
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = i + j;
            b[i][j] = i - j;
        }
    }

    //matrixMultiply_serial(a, b, d, n);
    
    double startTime = omp_get_wtime();
    // matrix multiplication
    matrixMultiply(a, b, c, n);
    double endTime = omp_get_wtime();

    //bool IsCorrect = compare(c,d,n);
    printf("Size of matrix = %d, time = %f\n\n",n,endTime-startTime);
    
    //print part of matrix
    for (int i = 0; i < 10; i++) {  // print the first 10 rows and columns
        for (int j = 0; j < 10; j++) {
            printf("%lf ", c[i][j]);
        }
        printf("\n");
    }

    // free memory
    freeMatrix(a, n);
    freeMatrix(b, n);
    freeMatrix(c, n);

    return 0;
}

/*
 gcc -fopenmp -O3 matmul_omp.cpp -o  matmul_omp.elf
g++ -pg -fopenmp -O0 matmul_omp.cpp -o  matmul_omp.elf

g++ -O3 -fopenmp  matmul_omp_2.cpp -o matmul_omp_2.elf

*/
/*
#!/bin/bash
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 00:30:00
#SBATCH -N 1
#SBATCH -c 16
export OMP_NUM_THREADS=16
export I_MPI_DEBUG=5
export I_MPI_HYDRA_BOOTSTRAP=ssh


./matmul_omp.elf 

date
*/
