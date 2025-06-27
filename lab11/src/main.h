#pragma once
/*
* 提供如下三种方法实现的卷积计算的测试：
* 1. 最原始的单个滑动窗口实现简单直接卷积
* 2. im2col+gemm 实现卷积
* 3. cuDNN 实现卷积
* 
* Auther: Fuuzen
*/

// Utilities and system includes
#include <assert.h>

// helper for shared functions common to CUDA Samples
#include <helper_cuda.h>

// CUDA runtime
#include <cuda_runtime.h>

// conversion between image file and cv::Mat
#include "image.h"

// convolution params
#define BATCH_SIZE 1
#define KERNEL_H 3
#define KERNEL_W 3
#define CHANNELS 3 // 认为是 RGB 三通道
#define STRIDE 1

// 边缘检测卷积核(KCHW)
const float KERNEL[KERNEL_H * KERNEL_W * CHANNELS * CHANNELS] = {
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
  1,  1, 1, 1, -8, 1, 1,  1, 1,
};
