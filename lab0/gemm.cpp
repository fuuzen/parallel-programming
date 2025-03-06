#include "gemm.h"
#include "naive.cpp"

#include <chrono>
#include <iomanip>
chrono::time_point<chrono::high_resolution_clock> start_time, end_time;

void genRandomMatrix(int matrix[DIMENSION][DIMENSION]){
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 100);

  for(int i = 0; i < DIMENSION; i++){
    for(int j = 0; j < DIMENSION; j++){
      matrix[i][j] = dis(gen);
    }
  }
}

int main(){
  cin.tie(0);
  ios::sync_with_stdio(false);
  cout.tie(0);

  int matrixA[DIMENSION][DIMENSION];
  int matrixB[DIMENSION][DIMENSION];
  int matrixC[DIMENSION][DIMENSION];

  genRandomMatrix(matrixA);
  genRandomMatrix(matrixB);

  
  start_time = chrono::high_resolution_clock::now();

  #ifdef NAIVE
  naiveGEMM(matrixA, matrixB, matrixC, DIMENSION, DIMENSION, DIMENSION);
  #endif

  end_time = chrono::high_resolution_clock::now();
  auto ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
  auto us = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
  auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
  cout << ns.count() << " ns" << endl
     << us.count() << " us" << endl
     << ms.count() << " ms" << endl;

  return 0;
}
