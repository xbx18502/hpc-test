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

void run(int N){
    cout<<"size = "<<N<<endl;
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
}                      

int main(int argc, char* argv[]) {

    int N = std::stoi(argv[1]);
    //cout<<"args = "<<N<<endl;
    int size[] = {10,100,1000,5000};
    for(auto x:size){
        run(x);
    }
    
    
   
  

   

    
    return 0;
}
