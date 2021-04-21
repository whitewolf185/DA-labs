#include <iostream>
#include <vector>
#include <time.h>

bool IsReq(const std::vector<std::vector<int>> &A, const std::pair<int, int> &a, const std::pair<int, int> &b) {
  for (int i = a.first; i < b.first; ++i) {
    for (int j = a.second; j < b.second; ++j) {
      if (A[i][j] == 1) {
        return false;
      }
    }
  }
  return true;
}

int Atoi(const char &c) {
  return c - '0';
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<std::vector<int>> A(n, std::vector<int>(m, 0));
  int sMax = 0;

  int64_t start = clock();

  //input
  for (int i = 0; i < n; ++i) {
    std::string input;
    std::cin >> input;
    for (int j = 0; j < m; ++j) {
      int digit = Atoi(input[j]);
      A[i][j] = digit;
    }
  }

  for (int x = 0; x < n; ++x) {
    for (int y = 0; y < m; ++y) {
      for (int i = x; i < n; ++i) {
        for (int j = y; j < m; ++j) {
          std::pair<int, int> a(x, y);
          std::pair<int, int> b(i, j);
          if (IsReq(A, a, b)) {
            sMax = std::max(sMax, (b.first - a.first) * (b.second - a.second));
          }
        }
      }
    }
  }

  int64_t end = clock();
  int64_t time = end - start;
  std::cout << "Naive time " << time / 1000 << '.' << time % 1000 << " ms\n";


  return 0;
}
