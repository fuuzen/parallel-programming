#include "conv_im2col_gemm.h"
#include "helper_cuda.h"


__global__ void im2col_kernel(
  const float* input_data, float* col_data,
  int channels,
  int in_h, int in_w,
  int ker_h, int ker_w,
  int stride_h, int stride_w,
  int pad_h, int pad_w,
  int out_h, int out_w
) {
  int out_y = blockIdx.y * blockDim.y + threadIdx.y;
  int out_x = blockIdx.x * blockDim.x + threadIdx.x;

  if (out_y < out_h && out_x < out_w) {
    int col_index = out_y * out_w + out_x;
    for (int c = 0; c < channels; ++c) {
      for (int kh = 0; kh < ker_h; ++kh) {
        for (int kw = 0; kw < ker_w; ++kw) {
          int in_y = out_y * stride_h - pad_h + kh;
          int in_x = out_x * stride_w - pad_w + kw;

          int row_index = c * ker_h * ker_w + kh * ker_w + kw;
          int col_buffer_index = row_index * (out_h * out_w) + col_index;

          if (in_y >= 0 && in_y < in_h && in_x >= 0 && in_x < in_w) {
            col_data[col_buffer_index] = input_data[c * in_h * in_w + in_y * in_w + in_x];
          } else {
            col_data[col_buffer_index] = 0.0f;
          }
        }
      }
    }
  }
}


__global__ void gemm_kernel(
  const float* A,
  const float* B,
  float* C,
  int m, int n, int k
) {
  __shared__ float shareA[BLOCK_SIZE][BLOCK_SIZE];
  __shared__ float shareB[BLOCK_SIZE][BLOCK_SIZE];
  
  int bx = blockIdx.x;
  int by = blockIdx.y;
  int tx = threadIdx.x;
  int ty = threadIdx.y;
  int row = by * BLOCK_SIZE + ty;
  int col = bx * BLOCK_SIZE + tx;
  float temp = 0;
  
  for (int t = 0; t < (n + BLOCK_SIZE - 1) / BLOCK_SIZE; t++) {
    if (row < m && t * BLOCK_SIZE + tx < n) {
      shareA[ty][tx] = A[row * n + t * BLOCK_SIZE + tx];
    } else {
      shareA[ty][tx] = 0.0f;
    }
    if (t * BLOCK_SIZE + ty < n && col < k) {
      shareB[ty][tx] = B[(t * BLOCK_SIZE + ty) * k + col];
    } else {
      shareB[ty][tx] = 0.0f;
    }
    __syncthreads();
    if (row < m && col < k) {
      for (int i = 0; i < BLOCK_SIZE; i++) {
        temp += shareA[ty][i] * shareB[i][tx];
      }
    }
    __syncthreads();
  }
  if (row < m && col < k) {
    C[row * k + col] = temp;
  }
}


cv::Mat conv_im2col_gemm(
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
  if (out_batch_size != 1) {
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

  int m = out_channels;
  int n = in_channels * kernel_h * kernel_w;
  int k = out_height * out_width;

  std::vector<cv::Mat> out_channels_mat;

  size_t kernel_bytes = out_channels * in_channels * kernel_h * kernel_w * sizeof(float);
  size_t input_bytes = batch_size * in_height * in_width * in_channels * sizeof(float);
  size_t output_bytes = m * k * sizeof(float);
  size_t col_bytes = n * k * sizeof(float);

  float *d_input, *d_kernel, *d_output, *d_col;
  cudaMalloc(&d_input, input_bytes);
  cudaMalloc(&d_col, col_bytes);
  cudaMemcpy(d_input, image.ptr<float>(0), input_bytes, cudaMemcpyHostToDevice);
  
  // im2col
  dim3 im2col_block(BLOCK_SIZE, BLOCK_SIZE);
  dim3 im2col_grid(
    (out_width + im2col_block.x - 1) / im2col_block.x, 
    (out_height + im2col_block.y - 1) / im2col_block.y
  );
  im2col_kernel<<<im2col_grid, im2col_block>>>(
    d_input, d_col,
    in_channels,
    in_height, in_width,
    kernel_h, kernel_w,
    stride, stride,
    padding, padding,
    out_height, out_width
  );
  checkCudaErrors(cudaDeviceSynchronize());

  cudaFree(d_input);
  cudaMalloc(&d_kernel, kernel_bytes);
  cudaMalloc(&d_output, output_bytes);
  cudaMemcpy(d_kernel, kernel, kernel_bytes, cudaMemcpyHostToDevice);
  
  // gemm
  dim3 gemm_block(BLOCK_SIZE, BLOCK_SIZE);
  dim3 gemm_grid(
    (k + gemm_block.x - 1) / gemm_block.x,
    (m + gemm_block.y - 1) / gemm_block.y
  );
  gemm_kernel<<<gemm_grid, gemm_block>>>(
    d_kernel,
    d_col,
    d_output,
    m, n, k
  );
  
  cv::Mat output_image(out_height, out_width, CV_32FC3);
  checkCudaErrors(cudaDeviceSynchronize());
  cudaMemcpy(output_image.ptr(0), d_output, output_bytes, cudaMemcpyDeviceToHost);

  cudaFree(d_col);
  cudaFree(d_kernel);
  cudaFree(d_output);
  return output_image;
}