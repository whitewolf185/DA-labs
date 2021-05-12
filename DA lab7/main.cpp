#include <iostream>
#include <stack>
#include <vector>
#include <time.h>

int Atoi(const char &c) {
  return c - '0';
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> p(m, 0);
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

  for (int i = 0; i < n; ++i) {
    //calculating p fot i's row
    for (int j = 0; j < m; ++j) {
      p[j] = (p[j] + 1) * !A[i][j];
    }

    //calculating R fot i's row
    std::vector<int> R(m, 0);
    {
      std::stack<int> S;
      for (int j = 0; j < m; ++j) {
        while (!S.empty() && p[j] < p[S.top()]) {
          R[S.top()] = j - 1;
          S.pop();
        }

        S.push(j);
      }
      while (!S.empty()) {
        R[S.top()] = m - 1;
        S.pop();
      }
    }

    //calculating L fot i's row
    std::vector<int> L(m, 0);
    {
      std::stack<int> S;
      for (int j = m - 1; j >= 0; --j) {
        while (!S.empty() && p[j] < p[S.top()]) {
          L[S.top()] = j + 1;
          S.pop();
        }

        S.push(j);
      }
      while (!S.empty()) {
        L[S.top()] = 0;
        S.pop();
      }
    }

    int s_iMax = 0;
    for (int j = 0; j < m; ++j) {
      s_iMax = std::max(s_iMax, p[j] * (R[j] - L[j] + 1));
    }

    sMax = std::max(sMax, s_iMax);
  }


  std::cout << sMax << std::endl;

  return 0;
}
