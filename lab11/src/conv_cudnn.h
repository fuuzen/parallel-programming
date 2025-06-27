#include <cudnn_cnn.h>
#include <opencv2/opencv.hpp>

#define checkCUDNN(expression)                               \
  {                                                          \
    cudnnStatus_t status = (expression);                     \
    if (status != CUDNN_STATUS_SUCCESS) {                    \
      std::cerr << "Error on line " << __LINE__ << ": "      \
                << cudnnGetErrorString(status) << std::endl; \
      std::exit(EXIT_FAILURE);                               \
    }                                                        \
  }


/**
 * @brief: CuDNN 实现卷积
 */
cv::Mat conv_cudnn(
  cv::Mat &image,
  float * kernel,
  int kernelH,
  int kernelW,
  int in_channels = 3,
  int stride = 1,
  int padding = 0,
  int batch_size = 1
);