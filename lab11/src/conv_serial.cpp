#define OFFSET(row, col, ld) ((row) * (ld) + (col))

/*
  @brief: 串行卷积实现 CPU代码 NCHW
  @param in inC inH inW: 输入矩阵(数组) channel height width
  @param out outC outH outW: 输出矩阵 channel height width
  @param kernel kernelH kernelW: 卷积核 height width
*/
void conv_serial(
  float *in, float *out, float *kernel, int batch_size,
  int inC, int inH, int inW,
  int outC, int outH, int outW,
  int kernelH, int kernelW
) {
  float val;
  int out_pos, in_pos, kernel_pos;
  for (int oc = 0; oc < outC; oc++) // 每个输出通道
  {
    // 对一个位置的操作 用当前输入channel卷积去对相应的输出channel
    // 保证每个outChannel都是多inChannel累积的结果
    for (int i = 0; i < outH; i++)
    {
      for (int j = 0; j < outW; j++)
      {
        val = 0; // 避免累积和需要多次读取写入
        out_pos = oc * outH * outW + OFFSET(i, j, outW);
        for (int ic = 0; ic < inC; ic++) // 对每个输入通道
        {
          for (int ii = 0; ii < kernelH; ii++)
          {
            for (int jj = 0; jj < kernelW; jj++)
            {
              in_pos = ic * inH * inW + OFFSET(i + ii, j + jj, inW);
              kernel_pos = oc * kernelH * kernelW + OFFSET(ii, jj, kernelW);
              val += in[in_pos] * kernel[kernel_pos];
            }
          }
        }
        out[out_pos] = val; // 与cudnn计算结果为相反数
      }
    }
  }
}