/*
* 提供如下三种方法实现的卷积计算的测试：
* 1. 最原始的单个滑动窗口实现简单直接卷积
* 2. im2col+gemm 实现卷积
* 3. cuDNN 实现卷积
* 
* Auther: Fuuzen
*/

// Utilities and system includes
#include <omp.h>  // accelerate host computation
#include <assert.h>
#include <helper_string.h>  // helper for shared functions common to CUDA Samples

// CUDA runtime
#include <cuda_runtime.h>
#include <cublas_v2.h>

// CUDA and CUBLAS functions
#include <helper_image.h>
#include <helper_cuda.h>

#define MAX_MULTIPLE_ISIZE 128
#define MIN_MULTIPLE_ISIZE 1
#define BLOCK_SIZE 32

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif
#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

// Optional Command-line multiplier for matrix sizes
typedef struct _matrixSize {
  unsigned int uiWA, uiHA, uiWB, uiHB, uiWC, uiHC;
} sMatrixSize;


void gemm(float* A, float* B, float* C, sMatrixSize &matrix_size);