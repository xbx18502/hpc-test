//#define N 1000

#include <iostream>
#include <vector>
#include <omp.h>
#include <random>

using namespace std;

//parallel type 0
void multiplyMatrices_parallel_0(const std::vector<std::vector<double>>& A,
                      const std::vector<std::vector<double>>& B,
                      std::vector<std::vector<double>>& C,
                      int numRowsA, int numColsA, int numColsB) {
    std::vector<std::vector<std::vector<double>>> vec3D(
        numRowsA,
        std::vector<std::vector<double>>(
            numColsB,
            std::vector<double>(numColsA)
        )
    );
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < numRowsA; i++) {
        for (int j = 0; j < numColsB; j++) {
            #pragma omp parallel for
            for (int k = 0; k < numColsA; k++) {
                vec3D[i][j][k] = (A[i][k] * B[k][j]);
            }
        }
    }
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < numRowsA; i++) {
        for (int j = 0; j < numColsB; j++) {
            double t = 0;
            #pragma omp parallel for reduction(+:t)
            for (int k = 0; k < numColsA; k++) {
                t+=vec3D[i][j][k];
            }
            C[i][j] = t;
        }
    }
}

//serial
void multiplyMatrices_serial(const std::vector<std::vector<double>>& A,
                      const std::vector<std::vector<double>>& B,
                      std::vector<std::vector<double>>& C,
                      int numRowsA, int numColsA, int numColsB) {
    
    for (int i = 0; i < numRowsA; i++) {
        for (int j = 0; j < numColsB; j++) {
            C[i][j] = 0;
            for (int k = 0; k < numColsA; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

bool compare(const std::vector<std::vector<double>>& C, const std::vector<std::vector<double>>& D){
    bool ans = true;
    int numRowsA = C.size();
    int numColsB = C[0].size();
    #pragma omp parallel for
    for(int i=0;i<numRowsA;i++){
        #pragma omp parallel for
        for(int j=0;j<numColsB;j++){
            if((C[i][j]-D[i][j])/C[i][j]>0.0001){
                cout<<"error"<<endl;
                ans = false;
            }
        }
    }
    return ans;

}

int main(int argc, char* argv[]) {

    int N = std::stoi(argv[1]);
    cout<<"args = "<<N<<endl;
    std::cout<<"max threads = "<<omp_get_max_threads()<<endl;
    int numRowsA = N, numColsA = N, numRowsB = N, numColsB = N;
    double startTime, endTime;

    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    std::vector<std::vector<double>> C0(N, std::vector<double>(N));
    std::vector<std::vector<double>> C1(N, std::vector<double>(N));
    

    // 初始化随机数生成器
    std::random_device rd;  // 随机数发生器
    std::mt19937 gen(rd()); // 用random_device初始化Mersenne Twister生成器
    std::uniform_real_distribution<> dis(0.0, 1.0); // 定义一个范围在0.0到1.0之间的均匀分布

    // 填充二维vector
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            A[i][j] = dis(gen); // 生成随机double并赋值
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            B[i][j] = dis(gen); // 生成随机double并赋值
        }
    }
    /*
    // std::cout<<"A = "<<std::endl;
    // for (const auto &row : A) {
    //     for (double elem : row) {
    //         std::cout << elem << " ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout<<"B = "<<std::endl;
    // for (const auto &row : B) {
    //     for (double elem : row) {
    //         std::cout << elem << " ";
    //     }
    //     std::cout << "\n";
    // }
    */
   
  

    // Serial matmul
    startTime = omp_get_wtime();
    multiplyMatrices_serial(A, B, C0, numRowsA, numColsA, numColsB);
    endTime = omp_get_wtime();

   

    std::cout << "serial Time taken: " << (endTime - startTime) << " seconds" << std::endl;

    // parallel 0 matmul
    startTime = omp_get_wtime();
    multiplyMatrices_parallel_0(A, B, C1, numRowsA, numColsA, numColsB);
    endTime = omp_get_wtime();

   

    std::cout << "parallel_0 Time taken: " << (endTime - startTime) << " seconds" << std::endl;
    
    std::cout<<"is there error?  :"<<!compare(C0,C1)<<std::endl;

    
    return 0;
}