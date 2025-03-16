#include "../gemm.h"

/**
 * 最 naive 的实现方式
 */
 void method0(
  double *A,
  double *B,
  double *C
);

/**
 * 调整循环顺序的实现
 */
void method1(
  double *A,
  double *B,
  double *C
) ;

/**
 * 循环展开的实现
 */
void method2(
  double *A,
  double *B,
  double *C
);

/**
 * Intel MKL 的实现
 */
void method3(
  const double *A,
  const double *B,
  double *C
);