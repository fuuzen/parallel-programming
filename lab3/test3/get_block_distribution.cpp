#include "main.h"
#include <cmath>

pair<int, int> get_block_distribution(int num_threads, int M, int N) {
  int best_rows = 1;
  int best_cols = num_threads;
  double best_ratio_diff = fabs(1.0 - (double)(M*best_cols)/(N*best_rows));
  
  // 尝试所有可能的因数分解
  for (int rows = 1; rows <= num_threads; rows++) {
    if (num_threads % rows != 0) continue;
    int cols = num_threads / rows;
    double ratio = (double)(M*cols)/(N*rows);
    double ratio_diff = fabs(1.0 - ratio);
    
    if (ratio_diff < best_ratio_diff) {
      best_ratio_diff = ratio_diff;
      best_rows = rows;
      best_cols = cols;
    }
  }
  
  return {best_rows, best_cols};
}