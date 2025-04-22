#include "parallel_for.h"
#include "../util/util.h"


typedef struct {
  double *A;
  double *B;
  double *C;
  int n;
  int k;
} GemmArg;
