#include "conv_cudnn.h"


cv::Mat conv_cudnn(
  cv::Mat &image,
  float * kernel,
  int kernelH,
  int kernelW,
  int in_channels,
  int stride,
  int padding,
  int batch_size
) {
  cudnnHandle_t cudnn;
  checkCUDNN(cudnnCreate(&cudnn));

  cudnnTensorDescriptor_t input_descriptor;
  checkCUDNN(cudnnCreateTensorDescriptor(&input_descriptor));
  checkCUDNN(cudnnSetTensor4dDescriptor(
    input_descriptor,
    /*format=*/CUDNN_TENSOR_NHWC,
    /*dataType=*/CUDNN_DATA_FLOAT,
    /*batch_size=*/batch_size,
    /*channels=*/in_channels,
    /*image_height=*/image.rows,
    /*image_width=*/image.cols
  ));

  cudnnFilterDescriptor_t kernel_descriptor;
  checkCUDNN(cudnnCreateFilterDescriptor(&kernel_descriptor));
  checkCUDNN(cudnnSetFilter4dDescriptor(kernel_descriptor,
    /*dataType=*/CUDNN_DATA_FLOAT,
    /*format=*/CUDNN_TENSOR_NCHW,
    /*out_channels=*/in_channels,
    /*in_channels=*/in_channels,
    /*kernel_height=*/kernelH,
    /*kernel_width=*/kernelW
  ));

  cudnnConvolutionDescriptor_t convolution_descriptor;
  checkCUDNN(cudnnCreateConvolutionDescriptor(&convolution_descriptor));
  checkCUDNN(cudnnSetConvolution2dDescriptor(convolution_descriptor,
    /*pad_height=*/padding,
    /*pad_width=*/padding,
    /*vertical_stride=*/stride,
    /*horizontal_stride=*/stride,
    /*dilation_height=*/1,
    /*dilation_width=*/1,
    /*mode=*/CUDNN_CROSS_CORRELATION,
    /*computeType=*/CUDNN_DATA_FLOAT
  ));

  int out_batch_size, out_channels, out_height, out_width;
  checkCUDNN(cudnnGetConvolution2dForwardOutputDim(
    convolution_descriptor,
    input_descriptor,
    kernel_descriptor,
    &out_batch_size,
    &out_channels,
    &out_height,
    &out_width
  ));
  
  cudnnTensorDescriptor_t output_descriptor;
  checkCUDNN(cudnnCreateTensorDescriptor(&output_descriptor));
  checkCUDNN(cudnnSetTensor4dDescriptor(output_descriptor,
    /*format=*/CUDNN_TENSOR_NHWC,
    /*dataType=*/CUDNN_DATA_FLOAT,
    /*batch_size=*/out_batch_size,
    /*channels=*/out_channels,
    /*image_height=*/out_height,
    /*image_width=*/out_width
  ));

  cudnnConvolutionFwdAlgoPerf_t algo_perf;
  int returnedAlgoCount = 0;
  checkCUDNN(cudnnGetConvolutionForwardAlgorithm_v7(
    cudnn,
    input_descriptor,
    kernel_descriptor,
    convolution_descriptor,
    output_descriptor,
    1,
    &returnedAlgoCount,
    &algo_perf
  ));
  cudnnConvolutionFwdAlgo_t convolution_algorithm = algo_perf.algo;

  size_t workspace_bytes = 0;
  checkCUDNN(cudnnGetConvolutionForwardWorkspaceSize(
    cudnn,
    input_descriptor,
    kernel_descriptor,
    convolution_descriptor,
    output_descriptor,
    convolution_algorithm,
    &workspace_bytes
  ));

  size_t kernel_bytes = sizeof(float) * in_channels * out_channels * kernelH * kernelW;
  size_t input_bytes = batch_size * image.rows * image.cols * in_channels * sizeof(float);
  size_t output_bytes = out_batch_size * out_channels * out_height * out_width * sizeof(float);

  float *d_input, *d_kernel, *d_output, *d_workspace;
  cudaMalloc(&d_workspace, workspace_bytes);
  cudaMalloc(&d_input, input_bytes);
  cudaMalloc(&d_kernel, kernel_bytes);
  cudaMalloc(&d_output, output_bytes);
  cudaMemset(d_output, 0, output_bytes);
  cudaMemcpy(d_input, image.ptr<float>(0), input_bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_kernel, kernel, kernel_bytes, cudaMemcpyHostToDevice);

  const float alpha = 1, beta = 0;
  checkCUDNN(cudnnConvolutionForward(
    cudnn,
    &alpha,
    input_descriptor,
    d_input,
    kernel_descriptor,
    d_kernel,
    convolution_descriptor,
    convolution_algorithm,
    d_workspace,
    workspace_bytes,
    &beta,
    output_descriptor,
    d_output
  ));

  cv::Mat output_image(out_height, out_width, CV_32FC3);
  cudaMemcpy(output_image.ptr(0), d_output, output_bytes, cudaMemcpyDeviceToHost);

  cudaFree(d_kernel);
  cudaFree(d_input);
  cudaFree(d_output);
  cudaFree(d_workspace);
  cudnnDestroyTensorDescriptor(input_descriptor);
  cudnnDestroyTensorDescriptor(output_descriptor);
  cudnnDestroyFilterDescriptor(kernel_descriptor);
  cudnnDestroyConvolutionDescriptor(convolution_descriptor);
  cudnnDestroy(cudnn);
  return output_image;
}