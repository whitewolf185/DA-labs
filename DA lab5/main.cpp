#define TRY_BOR

#ifdef TRY_BOR
#include "BORis.h"
#include <time.h>
#endif

#ifndef TRY_BOR
#include <iostream>
#include <memory>
#endif

int main() {
#ifdef TRY_BOR
  TBoris a;
  int64_t start = clock();
  try {
    std::string str1;
    std::string str2 = "";
    std::cin >> str1;
    a.SetText(str1, str2);
    a.Build();
#ifdef DEBUG
    TBoris::TIterator iter(a.GetRoot());
    std::cout << std::endl;
    a.PrintTree(iter, 0);
#endif

    int64_t end = clock();
    int64_t time = end - start;
    std::cout << "My time " << time / 1000 << '.' << time % 1000 << " ms\n";

  }
  catch (const std::invalid_argument &e) {
    std::cerr << e.what();
  }
  catch (const std::logic_error &e) {
    std::cerr << e.what();
  }
#endif

  return 0;
}