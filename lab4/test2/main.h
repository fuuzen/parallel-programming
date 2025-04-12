#include "../util/util.h"

const double QUADRA_RAND_MAX_DOUBLE = 1.0;

// RAND_MAX = 0x7fffffff
const uint64_t QUADRA_RAND_MAX_INT = 0x3fffffff00000001;

// 线程参数结构体
typedef struct {
  int count_all;
  int count_inside;
} Args;
