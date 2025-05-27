#include "main.h"

//! Compute reference data set matrix multiply on CPU
//! C = A * B
//! @param C          reference data, computed but preallocated
//! @param A          matrix A as provided to device
//! @param B          matrix B as provided to device
//! @param hA         height of matrix A
//! @param wA         width of matrix A
//! @param wB         width of matrix B
void matrixMulCPU(float *C, const float *A, const float *B, unsigned int hA,
                  unsigned int wA, unsigned int wB) {
  omp_set_num_threads(16);
  #pragma omp parallel for collapse(2) schedule(dynamic)
  for (unsigned int i = 0; i < hA; ++i)
    for (unsigned int j = 0; j < wB; ++j) {
      double sum = 0;
      for (unsigned int k = 0; k < wA; ++k) {
        double a = A[i * wA + k];
        double b = B[k * wB + j];
        sum += a * b;
      }
      C[i * wB + j] = (float)sum;
    }
}

// Allocates a matrix with random float entries.
void randomInit(float *data, int size) {
  for (int i = 0; i < size; ++i) data[i] = rand() / (float)RAND_MAX;
}

void printDiff(float *data1, float *data2, int width, int height,
               int iListLength, float fListTol) {
  printf("Listing first %d Differences > %.6f...\n", iListLength, fListTol);
  int i, j, k;
  int error_count = 0;

  for (j = 0; j < height; j++) {
    if (error_count < iListLength) {
      printf("\n  Row %d:\n", j);
    }

    for (i = 0; i < width; i++) {
      k = j * width + i;
      float fDiff = fabs(data1[k] - data2[k]);

      if (fDiff > fListTol) {
        if (error_count < iListLength) {
          printf("    Loc(%d,%d)\tCPU=%.5f\tGPU=%.5f\tDiff=%.6f\n", i, j,
                 data1[k], data2[k], fDiff);
        }

        error_count++;
      }
    }
  }

  printf(" \n  Total Errors = %d\n", error_count);
}

void initializeCUDA(
  int argc,
  char **argv,
  int &devID,
  int &iSizeMultiple,
  sMatrixSize &matrix_size
) {
  // By default, we use device 0, otherwise we override the device ID based on
  // what is provided at the command line
  cudaError_t error;
  devID = 0;

  devID = findCudaDevice(argc, (const char **)argv);

  if (checkCmdLineFlag(argc, (const char **)argv, "sizemult")) {
    iSizeMultiple = getCmdLineArgumentInt(argc, (const char **)argv, "sizemult");
  }

  iSizeMultiple = min(iSizeMultiple, MAX_MULTIPLE_ISIZE);
  iSizeMultiple = max(iSizeMultiple, MIN_MULTIPLE_ISIZE);

  cudaDeviceProp deviceProp;

  error = cudaGetDeviceProperties(&deviceProp, devID);

  if (error != cudaSuccess) {
    printf("cudaGetDeviceProperties returned error code %d, line(%d)\n", error, __LINE__);
    exit(EXIT_FAILURE);
  }

  int block_size = BLOCK_SIZE;

  matrix_size.uiWA = block_size * iSizeMultiple;
  matrix_size.uiHA = block_size * iSizeMultiple;
  matrix_size.uiWB = block_size * iSizeMultiple;
  matrix_size.uiHB = block_size * iSizeMultiple;
  matrix_size.uiWC = block_size * iSizeMultiple;
  matrix_size.uiHC = block_size * iSizeMultiple;

  printf("MatrixA(%u,%u), MatrixB(%u,%u), MatrixC(%u,%u)\n", matrix_size.uiHA,
         matrix_size.uiWA, matrix_size.uiHB, matrix_size.uiWB, matrix_size.uiHC,
         matrix_size.uiWC);

  if (matrix_size.uiWA != matrix_size.uiHB ||
      matrix_size.uiHA != matrix_size.uiHC ||
      matrix_size.uiWB != matrix_size.uiWC) {
    printf("ERROR: Matrix sizes do not match!\n");
    exit(-1);
  }
}

//! Run a simple test matrix multiply using CUBLAS
void matrixMultiply(int argc, char **argv, int devID, sMatrixSize &matrix_size, int method, int verify) {
  cudaDeviceProp deviceProp;

  checkCudaErrors(cudaGetDeviceProperties(&deviceProp, devID));

  int block_size = 32;

  // allocate host memory for matrices A and B
  unsigned int size_A = matrix_size.uiWA * matrix_size.uiHA;
  unsigned int mem_size_A = sizeof(float) * size_A;
  float *h_A = (float *)malloc(mem_size_A);
  unsigned int size_B = matrix_size.uiWB * matrix_size.uiHB;
  unsigned int mem_size_B = sizeof(float) * size_B;
  float *h_B = (float *)malloc(mem_size_B);

  // set seed for rand()
  srand(2025);

  // initialize host memory
  randomInit(h_A, size_A);
  randomInit(h_B, size_B);

  // allocate device memory
  float *d_A, *d_B, *d_C;
  unsigned int size_C = matrix_size.uiWC * matrix_size.uiHC;
  unsigned int mem_size_C = sizeof(float) * size_C;

  // allocate host memory for the result
  float *h_C = (float *)malloc(mem_size_C);
  float *h_CUBLAS = (float *)malloc(mem_size_C);

  checkCudaErrors(cudaMalloc((void **)&d_A, mem_size_A));
  checkCudaErrors(cudaMalloc((void **)&d_B, mem_size_B));
  checkCudaErrors(cudaMemcpy(d_A, h_A, mem_size_A, cudaMemcpyHostToDevice));
  checkCudaErrors(cudaMemcpy(d_B, h_B, mem_size_B, cudaMemcpyHostToDevice));
  checkCudaErrors(cudaMalloc((void **)&d_C, mem_size_C));

  // setup execution parameters
  dim3 threads(block_size, block_size);
  dim3 grid(matrix_size.uiWC / threads.x, matrix_size.uiHC / threads.y);

  // create and start timer
  printf("Computing result using %s...", method ? "my own implementation" : "CUBLAS");

  // execute the kernel
  int nIter = 30;

  // CUBLAS version 2.0
  {
    const float alpha = 1.0f;
    const float beta = 0.0f;
    cublasHandle_t handle;
    cudaEvent_t start, stop;

    checkCudaErrors(cublasCreate(&handle));

    // Perform warmup operation
    if (method) {
      // 我自己的实现
      gemm(d_A, d_B, d_C, matrix_size);
    } else {
      checkCudaErrors(cublasSgemm(
        handle, CUBLAS_OP_N, CUBLAS_OP_N, matrix_size.uiWB, matrix_size.uiHA,
        matrix_size.uiWA, &alpha, d_B, matrix_size.uiWB, d_A,
        matrix_size.uiWA, &beta, d_C, matrix_size.uiWB));
    }

    // Allocate CUDA events that we'll use for timing
    checkCudaErrors(cudaEventCreate(&start));
    checkCudaErrors(cudaEventCreate(&stop));

    // Record the start event
    checkCudaErrors(cudaEventRecord(start, NULL));

    for (int j = 0; j < nIter; j++) {
      if (method) {
        // 我自己的实现
        gemm(d_A, d_B, d_C, matrix_size);
      } else {
        // note cublas is column primary!
        // need to transpose the order
        checkCudaErrors(cublasSgemm(
          handle, CUBLAS_OP_N, CUBLAS_OP_N, matrix_size.uiWB, matrix_size.uiHA,
          matrix_size.uiWA, &alpha, d_B, matrix_size.uiWB, d_A,
          matrix_size.uiWA, &beta, d_C, matrix_size.uiWB));
      }
    }

    printf("done.\n");

    // Record the stop event
    checkCudaErrors(cudaEventRecord(stop, NULL));

    // Wait for the stop event to complete
    checkCudaErrors(cudaEventSynchronize(stop));

    float msecTotal = 0.0f;
    checkCudaErrors(cudaEventElapsedTime(&msecTotal, start, stop));

    // Compute and print the performance
    float msecPerMatrixMul = msecTotal / nIter;
    double flopsPerMatrixMul = 2.0 * (double)matrix_size.uiHC *
                               (double)matrix_size.uiWC *
                               (double)matrix_size.uiHB;
    double gigaFlops =
        (flopsPerMatrixMul * 1.0e-9f) / (msecPerMatrixMul / 1000.0f);
    printf("Performance= %.2f GFlop/s, Time= %.3f msec, Size= %.0f Ops\n",
           gigaFlops, msecPerMatrixMul, flopsPerMatrixMul);

    // copy result from device to host
    checkCudaErrors(
        cudaMemcpy(h_CUBLAS, d_C, mem_size_C, cudaMemcpyDeviceToHost));

    // Destroy the handle
    checkCudaErrors(cublasDestroy(handle));
  }

  if (verify) {
    // compute reference solution
    printf("Computing result using host CPU...");
    float *reference = (float *)malloc(mem_size_C);
    matrixMulCPU(reference, h_A, h_B, matrix_size.uiHA, matrix_size.uiWA,
                matrix_size.uiWB);
    printf("done.\n");

    // check result (CUBLAS)
    bool resCUBLAS = sdkCompareL2fe(reference, h_CUBLAS, size_C, 1.0e-6f);

    if (resCUBLAS != true) {
      printDiff(reference, h_CUBLAS, matrix_size.uiWC, matrix_size.uiHC, 100,
                1.0e-5f);
    }

    printf("Comparing CUBLAS Matrix Multiply with CPU results: %s\n",
          (true == resCUBLAS) ? "PASS" : "FAIL");
    
    free(reference);
  }

  // clean up memory
  free(h_A);
  free(h_B);
  free(h_C);
  checkCudaErrors(cudaFree(d_A));
  checkCudaErrors(cudaFree(d_B));
  checkCudaErrors(cudaFree(d_C));
}

// Program main
int main(int argc, char **argv) {
  printf("[Matrix Multiply CUBLAS] - Starting...\n");

  int devID = 0, sizeMult = 5;
  sMatrixSize matrix_size;

  // method 参数选择不同实现方法: 0 代表 CUBLAS 实现 (默认); 非 0 代表我自己的实现;
  int method = 0;
  if (checkCmdLineFlag(argc, (const char **)argv, "method")) {
    method = getCmdLineArgumentInt(argc, (const char **)argv, "method");
  }

  // verify 参数表示是否验证计算正确性: 0 代表不验证 (默认); 非 0 代表验证;
  int verify = 0;
  if (checkCmdLineFlag(argc, (const char **)argv, "verify")) {
    verify = getCmdLineArgumentInt(argc, (const char **)argv, "verify");
  }
  
  initializeCUDA(argc, argv, devID, sizeMult, matrix_size);

  matrixMultiply(argc, argv, devID, matrix_size, method, verify);

  return 0;
}
