#include <bits/stdc++.h>
#include <omp.h>
#include <x86intrin.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Matrix allocation using a single continuous memory block
double* allocateMatrix(int size) {
    return (double*) malloc(size * size * sizeof(double));
}

void freeMatrix(double* matrix) {
    free(matrix);
}
/*when blocksize2 = 8*/
void matrixMultiply(double* a, double* b, double* c, int n, int blockSize0, int blockSize1, int blockSize2) {
    int i, j, k, ii, jj, kk;
    double sum;

    #pragma omp parallel for private(i, j, k, ii, jj, kk, sum) shared(a, b, c)
    for (ii = 0; ii < n; ii += blockSize0) {
        for (jj = 0; jj < n; jj += blockSize1) {
            for (kk = 0; kk < n; kk += blockSize2) {
                for (i = ii; i < MIN(ii + blockSize0, n); i++) {
                    for (j = jj; j < MIN(jj + blockSize1, n);j++ ) {
                        sum = c[i*n + j];
                        //for (k = kk; k < MIN(kk + blockSize2, n); k+=8) {
                            k=kk;
                            
                            sum += a[i*n + k] * b[k+ j*n];
                            sum += a[i*n + (k+1)] * b[(k+1) + j*n];
                            sum += a[i*n + (k+2)] * b[(k+2) + j*n];
                            sum += a[i*n + (k+3)] * b[(k+3) + j*n];
                            sum += a[i*n + (k+4)] * b[(k+4) + j*n];
                            sum += a[i*n + (k+5)] * b[(k+5) + j*n];
                            sum += a[i*n + (k+6)] * b[(k+6) + j*n];
                            sum += a[i*n + (k+7)] * b[(k+7) + j*n];
                            k+=8;
                        //}
                        c[i*n + j] = sum;
                        
                    }
                    
                }
            }
        }
    }
}
/*when blocksize2 = 4*/
void matrixMultiply0(double* a, double* b, double* c, int n, int blockSize0, int blockSize1, int blockSize2) {
    int i, j, k, ii, jj, kk;
    double sum;

    #pragma omp parallel for private(i, j, k, ii, jj, kk, sum) shared(a, b, c)
    for (ii = 0; ii < n; ii += blockSize0) {
        for (jj = 0; jj < n; jj += blockSize1) {
            for (kk = 0; kk < n; kk += blockSize2) {
                for (i = ii; i < MIN(ii + blockSize0, n); i++) {
                    for (j = jj; j < MIN(jj + blockSize1, n);j++ ) {
                        sum = c[i*n + j];
                        //for (k = kk; k < MIN(kk + blockSize2, n); k+=8) {
                            k=kk;
                            
                            sum += a[i*n + k] * b[k+ j*n];
                            sum += a[i*n + (k+1)] * b[(k+1) + j*n];
                            sum += a[i*n + (k+2)] * b[(k+2) + j*n];
                            sum += a[i*n + (k+3)] * b[(k+3) + j*n];
                            k+=4;
                        //}
                        c[i*n + j] = sum;
                        
                    }
                    
                }
            }
        }
    }
}
int main(int argc, char **argv) {
    long long int n = strtol(argv[1], NULL, 10);  // size of matrix
    int blockSize0 =  8; // This should be tuned to your specific CPU cache size
    int blockSize1 = 16;
    int blockSize2 = 8;
    // if(n%blockSize0!=0||n%blockSize1!=0||n%blockSize2!=0){
    //     printf("The size of matrix is not divisible by blocksize\n");
    //     return -1;
    // }
    double *a, *b, *c;
    a = allocateMatrix(n);
    b = allocateMatrix(n);
    c = allocateMatrix(n);

    // Initialize matrices
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i*n + j] = i + j;
            b[i + j*n] = i - j;
        }
    }
    
    double startTime = omp_get_wtime();
    matrixMultiply(a, b, c, n, blockSize0, blockSize1, blockSize2);
    double endTime = omp_get_wtime();
    long long int timesOfOperation = n*n*(2*n-1);
    double time = endTime-startTime;
    printf("Size of matrix = %lld, time = %f, TFLOPS = %f\n\n",n,time,timesOfOperation/time/1e12);

    // Print part of matrix
    for (int i = 0; i < 10; i++) {  // print the first 10 rows and columns
        for (int j = 0; j < 10; j++) {
            printf("%lf ", c[i*n + j]);
        }
        printf("\n");
    }

    // Free memory
    freeMatrix(a);
    freeMatrix(b);
    freeMatrix(c);

    return 0;
}
