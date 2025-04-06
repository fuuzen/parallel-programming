#include "../main.h"

/**
 * 矩阵参数信息
 */
typedef struct {
  int m;
  int n;
  int k;
  int local_m;
  int block_m;
  int block_n;
  int block_k;
} MatrixParams;

/**
 * @brief 创建 MPI_Datatype MatrixParams
 */
MPI_Datatype create_matrix_params_type();

/**
 * @brief 输入一个合法的整数
 * @param min 最小值
 * @param max 最大值
 * @return int
 */
int input (int min, int max);

/**
 * @brief 输入一个合法的整数，必须整除一个给定的 mod
 * @param min 最小值
 * @param max 最大值
 * @param mod mod
 * @return int
 */
int input (int min, int max, int mod);

/**
 * @brief Intel MKL 的单线程 GEMM 实现
 * @param A 左乘矩阵，M x K
 * @param B 右乘矩阵，K x N
 * @param C 返回矩阵，M x N
 * @param params 矩阵参数
 */
void mkl_gemm (
  const double *A,
  const double *B,
  double *C,
  MatrixParams *params
);

/**
 * @brief 调整循环顺序并且循环展开的单线程 GEMM 实现
 * @param A 左乘矩阵，M x K
 * @param B 右乘矩阵，K x N
 * @param C 返回矩阵，M x N
 * @param params 矩阵参数
 */
 void gemm (
  const double *A,
  const double *B,
  double *C,
  MatrixParams *params
);

/**
 * @brief (Cannon 算法)调整循环顺序并且循环展开的单线程 GEMM 实现
 * @param A 左乘矩阵，M x K
 * @param B 右乘矩阵，K x N
 * @param C 返回矩阵，M x N
 * @param params 矩阵参数
 */
void gemm_cannon(
  const double *A,
  const double *B,
  double *C,
  MatrixParams *params
);

void serial_matrix_mult(double *A, double *B, double *C_serial, int m, int n, int k);

int verify_results(double *C_parallel, double *C_serial, int m, int k);