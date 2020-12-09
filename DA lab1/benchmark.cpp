#include "TVector.h"
#include <vector>
#include <ctime>
#include <chrono>

const int LEN = 2049;

using duration_t = std::chrono::milliseconds;
const std::string DURATION_PREFIX = " sec";

struct TPair {
  int key;
  char val[LEN];

  TPair() {
    key = 0;
  }

  TPair(int n, char elem[LEN]) {
    strcpy(val, elem);
    key = n;
  }

  ~TPair() {

  }
};


void CountingSort(NVector::TVector<TPair> &data, const unsigned int &maxNum) {
  NVector::TVector<int> count(maxNum + 1);
  NVector::TVector<TPair> result(data.GetSize());
  count.Assign(0);
  for (int j = 0; j < data.GetSize(); ++j) {
    count[data[j].key]++;
  }

  for (int j = 1; j <= maxNum; ++j) {
    count[j] = count[j] + count[j - 1];
  }

  for (int j = data.GetSize() - 1; j >= 0; --j) {
    result[count[data[j].key] - 1] = data[j];
    count[data[j].key]--;
  }

  for (int j = 0; j < data.GetSize(); ++j) {
    data[j] = result[j];
  }
}


bool operator<(const TPair& lhs, const TPair& rhs) {
  return lhs.key < rhs.key;
}

int main() {
  NVector::TVector<TPair> values;
  std::vector<TPair> stl_values;
  int maxNum = -1;

  int key;
  char val[LEN];
  while (scanf("%d %s", &key, val) > 0) {
    TPair initialVal;
    initialVal.key = key;
    strcpy(initialVal.val, val);
    values.PushBack(initialVal);
    stl_values.push_back(initialVal);

    maxNum = std::max(maxNum, key);
  }

  //long double start_cl, end_cl;

  //start_cl = clock();
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  if (!values.Empty()) {
    CountingSort(values, maxNum);
  }
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  //end_cl = clock();
  uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>(end - start).count();

  //std::cout << (end_cl - start_cl)/1000000.0 << std::endl;

  //start_cl = clock();
  start = std::chrono::system_clock::now();
  if (!stl_values.empty()) {
    std::stable_sort(stl_values.begin(), stl_values.end());
  }
  end = std::chrono::system_clock::now();
  //end_cl = clock();
  uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>(end - start).count();
  //std::cout << (end_cl - start_cl)/1000000.0 << std::endl;


  std::cout << "Counting sort time: " << counting_sort_ts / 1000.0 << DURATION_PREFIX << std::endl;
  std::cout << "STL stable sort time: " << stl_sort_ts / 1000.0 << DURATION_PREFIX << '\n' << std::endl;

  return 0;
}
