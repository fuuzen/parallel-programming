import random
import sys

# mouse
# MIN_ID = 0
# MAX_ID = 524

# flower
MIN_ID = 1
MAX_ID = 930

OUTPUT_FILE = 'data/input.txt'

def check(file):
  import pandas as pd
  df = pd.read_csv(file)
  points = pd.concat([df['source'], df['target']]).unique()
  points = sorted(map(int, points))
  min_point = min(points)
  max_point = max(points)
  missing_points = [i for i in range(min_point, max_point + 1) if i not in points]
  print(f"最小点编号: {min_point}")
  print(f"最大点编号: {max_point}")
  print(f"缺失的点: {missing_points}")

# check('data/updated_flower.csv')
# check('data/updated_mouse.csv')

n = int(sys.argv[1])

with open(OUTPUT_FILE, 'w') as f:
  for _ in range(n):
    a, b = random.sample(range(MIN_ID, MAX_ID + 1), 2)
    f.write(f"{a} {b}\n")