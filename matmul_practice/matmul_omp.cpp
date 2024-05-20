#include <iostream>
#include <vector>
#include <omp.h>

void multiplyMatrices(const std::vector<std::vector<int>>& A,
                      const std::vector<std::vector<int>>& B,
                      std::vector<std::vector<int>>& C,
                      int numRowsA, int numColsA, int numColsB) {
    #pragma omp parallel for
    for (int i = 0; i < numRowsA; i++) {
        for (int j = 0; j < numColsB; j++) {
            C[i][j] = 0;
            for (int k = 0; k < numColsA; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int numRowsA = 2, numColsA = 3, numRowsB = 3, numColsB = 2;
    
    // 初始化矩阵 A
    std::vector<std::vector<int>> A = {
        {1, 2, 3},
        {4, 5, 6}
    };

    // 初始化矩阵 B
    std::vector<std::vector<int>> B = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    // 初始化结果矩阵 C
    std::vector<std::vector<int>> C(numRowsA, std::vector<int>(numColsB, 0));

    // 计算矩阵乘法
    double startTime = omp_get_wtime();
    multiplyMatrices(A, B, C, numRowsA, numColsA, numColsB);
    double endTime = omp_get_wtime();

    // 打印结果矩阵
    std::cout << "Result Matrix C:" << std::endl;
    for (int i = 0; i < numRowsA; i++) {
        for (int j = 0; j < numColsB; j++) {
            std::cout << C[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Time taken: " << (endTime - startTime) << " seconds" << std::endl;

    return 0;
}
