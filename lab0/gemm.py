import random
import time

N = 2048

def randomMatrix(m, n):
  # 产生随机矩阵
  matrix = []
  for _ in range(m):
    row = []
    for _ in range(n):
      row.append(random.randint(0, 1000000) / 1000) # 浮点数
    matrix.append(row)
  return matrix

def naiveGEMM(A, B):
  # 矩阵乘法
  m = len(A)
  n = len(A[0])
  k = len(B[0])
  C = [[0 for _ in range(k)] for _ in range(m)]
  for i in range(m):
    for j in range(k):
      for l in range(n):
        C[i][j] += A[i][l] * B[l][j]
  return C

if __name__ == '__main__':
  m = N
  n = N
  k = N
  A = randomMatrix(m, n)
  B = randomMatrix(n, k)
  start_time = time.process_time()
  C = naiveGEMM(A, B)
  end_time = time.process_time()
  elapsed_time = end_time - start_time
  flops = 2 * N**3
  print("Time taken: ", elapsed_time)
  print("GFLOPS: ", (flops / elapsed_time) / 1e9)