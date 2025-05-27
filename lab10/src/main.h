/*
* 原代码:
* https://github.com/ndd314/cuda_examples/blob/master/0_Simple/matrixMulCUBLAS/new/matrixMulCUBLAS.cpp
* 
* 增加了对我自己实现的 CUDA 矩阵乘法的测试
* 增加对矩阵规模的控制，同时增加了一个参数控制使用哪种方法测试
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