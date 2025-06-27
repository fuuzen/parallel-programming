#include <opencv2/opencv.hpp>
#include <helper_cuda.h>


/**
 * @brief: 滑动窗口实现卷积
 */
cv::Mat conv_sliding_window(
  cv::Mat &image,
  float * kernel,
  int kernelH,
  int kernelW,
  int in_channels = 3,
  int stride = 1,
  int padding = 0,
  int batch_size = 1
);