#include "main.h"

__global__ void gemmKernel(
  float* __restrict__ A,
  float* __restrict__ B,
  float* __restrict__ C,
  sMatrixSize matrix_size
) {
  __shared__ float shareA[BLOCK_SIZE][BLOCK_SIZE];
  __shared__ float shareB[BLOCK_SIZE][BLOCK_SIZE];
  int bx = blockIdx.x;
  int by = blockIdx.y;
  int tx = threadIdx.x;
  int ty = threadIdx.y;
  int row = by * BLOCK_SIZE + ty;
  int col = bx * BLOCK_SIZE + tx;
  float temp = 0;
  for(int i = 0; i < matrix_size.uiWA/BLOCK_SIZE; ++i){
    shareA[ty][tx] = A[row*matrix_size.uiWA + (i*BLOCK_SIZE + tx)];
    shareB[ty][tx] = B[(i*BLOCK_SIZE + ty)*matrix_size.uiWB + col];
    __syncthreads();
    for(int k = 0; k < BLOCK_SIZE; ++k){
      temp += shareA[ty][k] * shareB[k][tx];
      __syncthreads();
    }
  }
  C[row*matrix_size.uiWC + col] = temp;
}

// Tiling 版本的矩阵乘法，需要矩阵规模为 BLOCK_SIZE 的倍数
void gemm(float* A, float* B, float* C, sMatrixSize &matrix_size) {
  dim3 blockDim(BLOCK_SIZE, BLOCK_SIZE);
  dim3 gridDim(
    matrix_size.uiWB / blockDim.x,
    matrix_size.uiHA / blockDim.y
  );

  gemmKernel<<<gridDim, blockDim>>>(A, B, C, matrix_size);

  checkCudaErrors(cudaDeviceSynchronize());
}