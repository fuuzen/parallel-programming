#include "gemm.h"
#include "methods/methods.h"

int main(){
  double *A = (double *)malloc(M * K * sizeof(double));
  double *B = (double *)malloc(K * N * sizeof(double));
  double *C = (double *)malloc(M * N * sizeof(double));

  // 初始化矩阵 A B C
  for (int i = 0; i < M * K; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < K * N; i++) B[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < M * N; i++) C[i] = 0.0;

  chrono::time_point<chrono::high_resolution_clock> start_time = chrono::high_resolution_clock::now();

  #ifdef METHOD0
  method0(A, B, C);
  #endif
  #ifdef METHOD1
  method1(A, B, C);
  #endif
  #ifdef METHOD2
  method2(A, B, C);
  #endif
  #ifdef METHOD3
  method3(A, B, C);
  #endif

  chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();

  chrono::duration<double> elapsed = end_time - start_time;

  double flops = 2.0 * N * N * N;
  double gflops = flops / elapsed.count();

  cout << "Time: " << elapsed.count() << " s" << endl << "GFLOPS: " << gflops << endl;

  return 0;
}
