#define TRY_BOR

#ifdef TRY_BOR
#include "BORis.h"
#endif

#ifndef TRY_BOR
#include <iostream>
#include <memory>
#endif

int main() {
#ifdef TRY_BOR
  TBoris a;
  try {
    std::string str1 = "aabaa";
    std::string str2 = "aa";
    a.SetText(str1, str2);
    a.Build();
    std::cout << "im done";
  }
  catch (const std::invalid_argument &e) {
    std::cerr << e.what();
  }
  catch (const std::logic_error &e) {
    std::cerr << e.what();
  }
#endif

#ifndef TRY_BOR
  int i = 0;
  for (i = 0; i <= 10; i++) {

  }
  std::cout << i;
#endif

  return 0;
}