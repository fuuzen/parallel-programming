#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <string>
#include <tuple>
#include <omp.h>
#include "timer.h"

using namespace std;

const double INF = numeric_limits<double>::max() / 2;

int main ( int argc, char *argv[] ) {
  int thread_num = 1;
  if (argc > 1) {
    thread_num = stoi(argv[1]);
    if (thread_num < 1) thread_num = 1;
  }
  omp_set_num_threads(thread_num);

  string csv_file = "./data/updated_mouse.csv";
  if (argc > 2) {
    csv_file = argv[2];
  }

  ifstream fin(csv_file);
  if (!fin.is_open()) {
    cerr << "无法打开邻接表文件: " << csv_file << endl;
    return 1;
  }
  string line;
  getline(fin, line); // 跳过表头
  int max_id = 0;
  vector<tuple<int, int, double>> edges;
  while (getline(fin, line)) {
    istringstream iss(line);
    string s, t, d;
    getline(iss, s, ','); getline(iss, t, ','); getline(iss, d, ',');
    int u = stoi(s), v = stoi(t);
    double w = stod(d);
    edges.emplace_back(u, v, w);
    if (u > max_id) max_id = u;
    if (v > max_id) max_id = v;
  }
  fin.close();
  int n_vertex = max_id + 1;

  // 构建邻接矩阵
  vector<vector<double>> dist(n_vertex, vector<double>(n_vertex, INF));
  for (int i = 0; i < n_vertex; ++i) dist[i][i] = 0;
  for (auto& e : edges) {
    int u, v; double w;
    tie(u, v, w) = e;
    dist[u][v] = w;
    dist[v][u] = w;
  }

  Timer *timer = new Timer();  // 开始计时

  // OpenMP 并行 Floyd-Warshall
  #pragma omp parallel for schedule(static)
  for (int k = 0; k < n_vertex; ++k) {
    for (int i = 0; i < n_vertex; ++i) {
      for (int j = 0; j < n_vertex; ++j) {
        if (dist[i][k] + dist[k][j] < dist[i][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
        }
      }
    }
  }

  delete timer;  // 结束计时

  vector<pair<int, int>> queries;
  int u, v;
  while (cin >> u >> v) {
    queries.emplace_back(u, v);
  }

  for (auto& q : queries) {
    int a = q.first, b = q.second;
    if (a >= 0 && a < n_vertex && b >= 0 && b < n_vertex && dist[a][b] < INF/2) {
      cout << dist[a][b] << endl;
    } else {
      cout << "inf" << endl;
    }
  }

  return 0;
}