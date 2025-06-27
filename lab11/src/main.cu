#include "main.h"
#include "conv_sliding_window.h"
#include "conv_im2col_gemm.h"
#include "conv_cudnn.h"


void initialize(
  int argc,
  char **argv,
  int &devID,
  int &method,
  int &stride,
  char **input,
  char **output
) {
  // By default, we use device 0, otherwise we override the device ID based on
  // what is provided at the command line
  devID = 0;
  devID = findCudaDevice(argc, (const char **)argv);

  if (checkCmdLineFlag(argc, (const char **)argv, "method")) {
    // method 参数选择不同实现方法: 0 代表滑动窗口实现, 1 代表 im2col+gemm 实现, 其他代表 CuDNN 实现
    method = getCmdLineArgumentInt(argc, (const char **)argv, "method");
  }
  if (checkCmdLineFlag(argc, (const char **)argv, "input")) {
    getCmdLineArgumentString(argc, (const char **)argv, "input", input);
  }
  if (checkCmdLineFlag(argc, (const char **)argv, "output")) {
    getCmdLineArgumentString(argc, (const char **)argv, "output", output);
  }
  if (checkCmdLineFlag(argc, (const char **)argv, "stride")) {
    stride = getCmdLineArgumentInt(argc, (const char **)argv, "stride");
  }
}


void convolution(int devID, int method, int stride, char *input_file, char *output_file) {
  cudaDeviceProp deviceProp;
  checkCudaErrors(cudaGetDeviceProperties(&deviceProp, devID));
  int nIter = 30;
  cudaEvent_t start, stop;
  checkCudaErrors(cudaEventCreate(&start));
  checkCudaErrors(cudaEventCreate(&stop));
  cv::Mat input, output;
  input = load_image(input_file);

  printf("Computing result using %s\n\n",
    method==0 ? "sliding window" :
    method==1 ? "im2col+gemm" :
    "CuDNN"
  );

  printf("Perform warm up convolution...");
  switch(method){
    case 0:
      output = conv_sliding_window(input, (float*)KERNEL, KERNEL_H, KERNEL_W, CHANNELS, stride);
      break;
    case 1:
      output = conv_im2col_gemm(input, (float*)KERNEL, KERNEL_H, KERNEL_W, CHANNELS, stride);
      break;
    default:
      output = conv_cudnn(input, (float*)KERNEL, KERNEL_H, KERNEL_W, CHANNELS, stride);
  }
  checkCudaErrors(cudaGetLastError());
  save_image(output_file, output);
  printf("done.\n\n");
  printf("Output image has been saved to %s.\n\n", output_file);

  // Record the start event
  checkCudaErrors(cudaEventRecord(start, NULL));

  // Evaluate performance
  printf("Evaluating performance...");
  for (int j = 0; j < nIter; j++) {
    switch(method){
      case 0:
        conv_sliding_window(input, (float*)KERNEL, KERNEL_H, KERNEL_W, CHANNELS, stride);
        break;
      case 1:
        conv_im2col_gemm(input, (float*)KERNEL, KERNEL_H, KERNEL_W, CHANNELS, stride);
        break;
      default:
        conv_cudnn(input, (float*)KERNEL, KERNEL_H, KERNEL_W, CHANNELS, stride);
    }
  }
  printf("done.\n\n");

  // Record the stop event
  checkCudaErrors(cudaEventRecord(stop, NULL));

  // Wait for the stop event to complete
  checkCudaErrors(cudaEventSynchronize(stop));

  float msecTotal = 0.0f;
  checkCudaErrors(cudaEventElapsedTime(&msecTotal, start, stop));

  // Compute and print the performance
  float msecPerConv = msecTotal / nIter;
  double flopsPerConv = 2.0 * CHANNELS * CHANNELS * output.rows * output.cols * KERNEL_H * KERNEL_W;
  double gigaFlops =
      (flopsPerConv * 1.0e-9f) / (msecPerConv / 1000.0f);
  printf("Performance= %.2f GFlop/s, Time= %.3f msec, Size= %.0f Ops\n\n",
          gigaFlops, msecPerConv, flopsPerConv);
}

// Program main
int main(int argc, char **argv) {
  printf("[Convolution] - Starting...\n\n");

  int devID = 0, method = 2, stride = 1;
  char default_input[10] = "input.jpg";
  char default_output[11] = "output.jpg";
  char * input = default_input, * output = default_output;
  
  initialize(argc, argv, devID, method, stride, &input, &output);

  convolution(devID, method, stride, input, output);

  return 0;
}
