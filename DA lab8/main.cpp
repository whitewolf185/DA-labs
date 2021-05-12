#include <iostream>
#include <vector>
#include <time.h>

int main() {
  long long N, p;
  std::cin >> N >> p;
  long long M;
  std::cin >> M;
  int64_t start = clock();
  std::vector<unsigned long long> pows(N, 1);
  long long count = (int) N - 1;
  for (int i = 1; i < N; ++i) {
    pows[i] = pows[i - 1] * p;

    if (pows[i] > M) {
      count = i;
      break;
    }
  }

  std::vector<unsigned long long> ans(N);

  while (M > 0) {
    ans[count] = M / pows[count];
    M %= (int) pows[count];
    count--;
  }

  int64_t end = clock();

  int64_t time = end - start;
  std::cout << "Greevy time " << time / 1000 << '.' << time % 1000 << " ms\n";

  /*for (auto& item : ans){
    std::cout << item << std::endl;
  }*/
  return 0;
}

// 4294967295