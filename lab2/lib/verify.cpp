#include "lib.h"
#include <cmath>

void serial_matrix_mult(double *A, double *B, double *C_serial, int m, int n, int k) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < k; j++) {
      C_serial[i * k + j] = 0.0;
      for (int p = 0; p < n; p++) {
        C_serial[i * k + j] += A[i * n + p] * B[p * k + j];
      }
    }
  }
}

int verify_results(double *C_parallel, double *C_serial, int m, int k) {
  string filename = "diff";
  double epsilon = 1e-10;
  int r = 0;
  FILE *fp = fopen(filename.c_str(), "w");
  
  if (fp == NULL) {
    printf("Error opening file for writing.\n");
    return -1;
  }
  
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < k; j++) {
      int index = i * k + j;
      if (fabs(C_parallel[index] - C_serial[index]) > epsilon) {
        fprintf(fp, "X ");
        r += 1;
      } else {
        fprintf(fp, "* ");
      }
    }
    fprintf(fp, "\n");
  }
  
  fclose(fp);
  printf("Comparison results written to %s\n", filename.c_str());
  printf("Total differences: %d (marked with X)\n", r);
  return r;
}