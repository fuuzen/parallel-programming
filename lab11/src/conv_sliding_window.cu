#include "conv_sliding_window.h"


__global__ void conv2d_kernel(
  const float* input, const float* kernel, float* output,
  int input_h, int input_w,
  int kernel_h, int kernel_w,
  int channels, int stride, int padding,
  int output_h, int output_w
) {
  int out_x = blockIdx.x * blockDim.x + threadIdx.x;
  int out_y = blockIdx.y * blockDim.y + threadIdx.y;
  if (out_x < output_w && out_y < output_h) {
    float acc = 0.0f;
    int in_base_y = out_y * stride - padding;
    int in_base_x = out_x * stride - padding;
    for (int c = 0; c < channels; ++c) {
      for (int kh = 0; kh < kernel_h; ++kh) {
        for (int kw = 0; kw < kernel_w; ++kw) {
          int in_y = in_base_y + kh;
          int in_x = in_base_x + kw;
          if (in_y >= 0 && in_y < input_h && in_x >= 0 && in_x < input_w) {
            int input_idx = (in_y * input_w + in_x) * channels + c;
            int kernel_idx = (kh * kernel_w + kw) * channels + c;
            acc += input[input_idx] * kernel[kernel_idx];
          }
        }
      }
    }
    output[out_y * output_w + out_x] = acc;
  }
}


cv::Mat conv_sliding_window(
  cv::Mat &image,
  float * kernel,
  int kernel_h,
  int kernel_w,
  int in_channels,
  int stride,
  int padding,
  int batch_size
) {
  int out_batch_size = batch_size;
  if (batch_size != 1) {
    std::cerr << "Sorry, only supports batch size = 1 now!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  int out_channels = in_channels;
  int in_height = image.rows;
  int in_width = image.cols;
  int out_height = (in_height - kernel_h + 2 * padding) / stride + 1;
  int out_width = (in_width - kernel_w + 2 * padding) / stride + 1;
  if (out_height <= 0 || out_width <= 0) {
    std::cerr << "Invalid image size, kernel size, padding or stride!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  size_t kernel_bytes = in_channels * kernel_h * kernel_w * sizeof(float);  // only 1 out channel
  size_t input_bytes = batch_size * in_height * in_width * in_channels * sizeof(float);
  size_t output_bytes = out_batch_size * out_height * out_width * sizeof(float);  // only 1 out channel

  std::vector<cv::Mat> mv;

  float *d_input, *d_kernel, *d_output;
  cudaMalloc(&d_input, input_bytes);
  cudaMalloc(&d_kernel, kernel_bytes);
  cudaMalloc(&d_output, output_bytes);
  cudaMemcpy(d_input, image.ptr<float>(0), input_bytes, cudaMemcpyHostToDevice);

  dim3 threadsPerBlock(16, 16);
  dim3 numBlocks(
    (out_width + threadsPerBlock.x - 1) / threadsPerBlock.x, 
    (out_height + threadsPerBlock.y - 1) / threadsPerBlock.y
  );

  for (int i = 0; i < out_channels; ++i) {
    cudaMemcpy(d_kernel, kernel + (kernel_bytes/sizeof(float)), kernel_bytes, cudaMemcpyHostToDevice);
    
    conv2d_kernel<<<numBlocks, threadsPerBlock>>>(
      d_input, d_kernel, d_output,
      in_height, in_width,
      kernel_h, kernel_w,
      in_channels, stride, padding,
      out_height, out_width
    );
    
    mv.emplace_back(cv::Mat(out_height, out_width, CV_32FC1));
    checkCudaErrors(cudaDeviceSynchronize());
    cudaMemcpy(mv[i].ptr(0), d_output, output_bytes, cudaMemcpyDeviceToHost);
  }

  cv::Mat output_image(out_height, out_width, CV_32FC3);
  cv::merge(mv, output_image);

  cudaFree(d_kernel);
  cudaFree(d_input);
  cudaFree(d_output);
  return output_image;
}