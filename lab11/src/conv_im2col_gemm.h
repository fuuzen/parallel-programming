#include <opencv2/opencv.hpp>
#include <helper_cuda.h>

#define BLOCK_SIZE 32

/**
 * @brief: im2col + gemm 实现卷积
 */
cv::Mat conv_im2col_gemm(
  cv::Mat &image,
  float * kernel,
  int kernelH,
  int kernelW,
  int in_channels = 3,
  int stride = 1,
  int padding = 0,
  int batch_size = 1
);