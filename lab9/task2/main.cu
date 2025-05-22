#include <stdio.h>
#include <cuda_runtime.h>
#include <stdlib.h>
#include <time.h>

#define EPSILON 0.000001

#define CUDA_CHECK(call) \
  do { \
    cudaError_t err = call; \
    if (err != cudaSuccess) { \
      fprintf(stderr, "CUDA error in %s:%d: %s\n", __FILE__, __LINE__, \
        cudaGetErrorString(err)); \
      exit(1); \
    } \
  } while (0)

__global__ void transposeGlobal(double *A, double *AT, int n) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  
  if (x < n && y < n) {
    AT[y * n + x] = A[x * n + y];
  }
}

__global__ void transposeShared(double *A, double *AT, int n, int tile_size) {
  __shared__ double tile[32][32];
  int x = blockIdx.x * tile_size + threadIdx.x;
  int y = blockIdx.y * tile_size + threadIdx.y;

  if (x < n && y < n) {
    tile[threadIdx.y][threadIdx.x] = A[y * n + x];
  }
  __syncthreads();

  if (x < n && y < n) {
    AT[x * n + y] = tile[threadIdx.x][threadIdx.y];
  }
}

void initializeMatrix(double *A, int n) {
  for (int i = 0; i < n * n; i++) {
    A[i] = (double)rand() / RAND_MAX;
  }
}

int verifyTransposed(double *A, double *AT, int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i == j && abs(A[i * n + j] - AT[i * n + j]) > EPSILON) {
        return 1;
      } else if (abs(A[i * n + j] - AT[j * n + i]) > EPSILON) {
        return 1;
      } 
    }
  }
  return 0;
}

int main( int argc, char *argv[] ) {
  int n = 2048;
  int tile_size = 32;
  char memory_mode = 'G';

  n = atoi(argv[1]);
  tile_size = atoi(argv[2]);
  memory_mode = argv[3][0];

  // if (n < 512 || n > 2048) {
  //   printf("Error: n must be between 512 and 2048\n");
  //   return 1;
  // }
  if (tile_size > 32) {
    printf("Error: tile_size must below 32\n");
    return 1;
  }
  if (memory_mode != 'G' && memory_mode != 'S') {
    printf("Error: memory_mode must be G or S\n");
    return 1;
  }

  size_t size = n * n * sizeof(double);
  
  double *h_A = (double *)malloc(size);
  double *h_AT = (double *)malloc(size);
  
  initializeMatrix(h_A, n);

  double *d_A, *d_AT;
  CUDA_CHECK(cudaMalloc(&d_A, size));
  CUDA_CHECK(cudaMalloc(&d_AT, size));
  
  CUDA_CHECK(cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice));

  dim3 blockDim(tile_size, tile_size);
  dim3 gridDim((n + tile_size - 1) / tile_size, (n + tile_size - 1) / tile_size);

  cudaEvent_t start, stop;
  CUDA_CHECK(cudaEventCreate(&start));
  CUDA_CHECK(cudaEventCreate(&stop));
  CUDA_CHECK(cudaEventRecord(start));

  if (memory_mode == 'G') {
    transposeGlobal<<<gridDim, blockDim>>>(d_A, d_AT, n);
  } else {
    transposeShared<<<gridDim, blockDim>>>(d_A, d_AT, n, tile_size);
  }
  CUDA_CHECK(cudaGetLastError());

  CUDA_CHECK(cudaEventRecord(stop));
  CUDA_CHECK(cudaEventSynchronize(stop));
  float milliseconds = 0;
  CUDA_CHECK(cudaEventElapsedTime(&milliseconds, start, stop));

  CUDA_CHECK(cudaMemcpy(h_AT, d_AT, size, cudaMemcpyDeviceToHost));

  printf("%.5E\n", milliseconds);

  int ret = verifyTransposed(h_A, h_AT, n);

  free(h_A);
  free(h_AT);
  CUDA_CHECK(cudaFree(d_A));
  CUDA_CHECK(cudaFree(d_AT));
  CUDA_CHECK(cudaEventDestroy(start));
  CUDA_CHECK(cudaEventDestroy(stop));

  return ret;
}