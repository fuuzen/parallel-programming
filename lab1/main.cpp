#include "main.h"
#include "lib/lib.h"

int np = 16;
int N = 2048;
int M = 2048;
int K = 2048;

int main(int argc, char **argv){
  cout << "[输入矩阵参数 N]" << endl;
  N = input(128, 2048);
  cout << "[输入矩阵参数 M]" << endl;
  M = input(128, 2048);
  cout << "[输入矩阵参数 K]" << endl;
  K = input(128, 2048);
  cout << "[输入线程数量 np]" << endl;
  K = input(1, 16);

  double *A = (double *)malloc(M * K * sizeof(double));
  double *B = (double *)malloc(K * N * sizeof(double));
  double *C = (double *)malloc(M * N * sizeof(double));

  // 初始化矩阵 A B C
  for (int i = 0; i < M * K; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < K * N; i++) B[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < M * N; i++) C[i] = 0.0;

  chrono::time_point<chrono::high_resolution_clock> start_time = chrono::high_resolution_clock::now();

  #ifdef METHOD1
  mpi_gemm(A, B, C);
  #endif
  #ifdef METHOD2
  mpi_gemm(A, B, C);
  #endif

  chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();

  chrono::duration<double> elapsed = end_time - start_time;

  double flops = 2.0 * N * N * N;
  double gflops = flops / elapsed.count();

  cout << "Time: " << elapsed.count() << " s" << endl << "GFLOPS: " << gflops << endl;

  return 0;
}
