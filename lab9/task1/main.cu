#include <stdio.h>
#include <cuda_runtime.h>

__global__ void helloWorldKernel() {
  int blockId = blockIdx.x;
  int threadIdX = threadIdx.x;
  int threadIdY = threadIdx.y;
  printf(
    "Hello World from Thread (%d, %d) in Block %d!\n", 
    threadIdX, threadIdY, blockId
  );
}

int main() {
  int n, m, k;
  
  printf("Enter n (number of blocks, 1-32): ");
  scanf("%d", &n);
  printf("Enter m (block dimension x, 1-32): ");
  scanf("%d", &m);
  printf("Enter k (block dimension y, 1-32): ");
  scanf("%d", &k);
  
  if (n < 1 || n > 32 || m < 1 || m > 32 || k < 1 || k > 32) {
    printf("Error: All inputs must be between 1 and 32\n");
    return 1;
  }
  
  printf("Hello World from the host!\n");
  
  dim3 blockDim(m, k);
  dim3 gridDim(n, 1);
  
  helloWorldKernel<<<gridDim, blockDim>>>();
  
  cudaDeviceSynchronize();
  
  return 0;
}