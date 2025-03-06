import random
import time

def randomMatrix(m, n):
  # 产生随机矩阵
  matrix = []
  for _ in range(m):
    row = []
    for _ in range(n):
      row.append(random.randint(0, 100))
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
  # m = random.randint(512, 2048)
  # n = random.randint(512, 2048)
  # k = random.randint(512, 2048)
  m = 2048
  n = 2048
  k = 2048
  A = randomMatrix(m, n)
  B = randomMatrix(n, k)
  start_time = time.process_time()
  C = naiveGEMM(A, B)
  end_time = time.process_time()
  print("Time taken: ", end_time - start_time)