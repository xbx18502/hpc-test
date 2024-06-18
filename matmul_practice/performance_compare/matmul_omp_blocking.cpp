#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Matrix allocation remains the same
double** allocateMatrix(int rows, int cols) {
    double** matrix = (double**) malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*) malloc(cols * sizeof(double));
    }
    return matrix;
}

void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void matrixMultiply(double** a, double** b, double** c, int n, int blockSize0, int blockSize1, int blockSize2) {
    int i, j, k, ii, jj, kk;
    double sum;

    #pragma omp parallel for private(i, j, k, ii, jj, kk, sum) shared(a, b, c)
    for (ii = 0; ii < n; ii += blockSize0) {
        for (jj = 0; jj < n; jj += blockSize1) {
            for (kk = 0; kk < n; kk += blockSize2) {
                for (i = ii; i < MIN(ii + blockSize0, n); i++) {
                    for (j = jj; j < MIN(jj + blockSize1, n); j++) {
                        sum = c[i][j];
                        for (k = kk; k < MIN(kk + blockSize2, n); k+=4) {
                            sum += a[i][k] * b[k][j];
                            sum += a[i][k+1] * b[k+1][j];
                            sum += a[i][k+2] * b[k+2][j];
                            sum += a[i][k+3] * b[k+3][j];
                            // sum += a[i][k+4] * b[k+4][j];
                            // sum += a[i][k+5] * b[k+5][j];
                            // sum += a[i][k+6] * b[k+6][j];
                            // sum += a[i][k+7] * b[k+7][j];
                        }
                        c[i][j] = sum;
                    }
                }
            }
        }
    }
}


int main(int argc, char **argv) {
    long long int n = strtol(argv[1], NULL, 10);  // size of matrix
    int blockSize0 =  16; // This should be tuned to your specific CPU cache size
    int blockSize1 = 8;
    int blockSize2 = 16;
    double **a, **b, **c;
    a = allocateMatrix(n, n);
    b = allocateMatrix(n, n);
    c = allocateMatrix(n, n);

    // Initialize matrices
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = i + j;
            b[i][j] = i - j;
        }
    }

    double startTime = omp_get_wtime();
    matrixMultiply(a, b, c, n, blockSize0, blockSize1,blockSize2);
    double endTime = omp_get_wtime();
    long long int timesOfOperation = n*n*(2*n-1);
    double time = endTime-startTime;
    printf("Size of matrix = %lld, time = %f, TFLOPS = %f\n\n",n,time,timesOfOperation/time/1e12);

    // Print part of matrix
    for (int i = 0; i < 10; i++) {  // print the first 10 rows and columns
        for (int j = 0; j < 10; j++) {
            printf("%lf ", c[i][j]);
        }
        printf("\n");
    }

    // Free memory
    freeMatrix(a, n);
    freeMatrix(b, n);
    freeMatrix(c, n);

    return 0;
}
