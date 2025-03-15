#include "gemm.h"

void naiveGEMM(
  int** A,
  int** B,
  int** C,
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