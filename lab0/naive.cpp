#include "gemm.h"

void naiveGEMM(
  int A[DIMENSION][DIMENSION],
  int B[DIMENSION][DIMENSION],
  int C[DIMENSION][DIMENSION],
  int m,
  int n,
  int k
) {
  for(int i = 0; i < m; i++) {
    for(int j = 0; j < n; j++) {
      for(int l = 0; l < k; l++) {
        C[i][j] += A[i][l] * B[l][j];
      }
    }
  }
}