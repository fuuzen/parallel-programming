#include "gemm.h"
#include "naive.cpp"

#include <chrono>
#include <iomanip>
chrono::time_point<chrono::high_resolution_clock> start_time, end_time;

void genRandomMatrix(int matrix[N][N]){
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 100);

  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      matrix[i][j] = dis(gen);
    }
  }
}

int main(){
  cin.tie(0);
  ios::sync_with_stdio(false);
  cout.tie(0);

  int matrixA[N][N];
  int matrixB[N][N];
  int matrixC[N][N];

  genRandomMatrix(matrixA);
  genRandomMatrix(matrixB);
  
  start_time = chrono::high_resolution_clock::now();

  #ifdef NAIVE
  naiveGEMM(matrixA, matrixB, matrixC, N, N, N);
  #endif

  end_time = chrono::high_resolution_clock::now();

  chrono::duration<double> elapsed = end_time - start_time;

  double flops = 2.0 * N * N * N;
  double gflops = (flops / elapsed.count()) / 1e9;

  cout << "time: " << elapsed.count() << " s" << endl << "GLOPS: " << gflops << endl;

  return 0;
}
