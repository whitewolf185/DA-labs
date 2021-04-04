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
    std::string str1;
    std::string str2;
    std::cin >> str1 >> str2;
    a.SetText(str1, str2);
    a.Build();
    a.Colorize();
//    TBoris::TIterator iter (a.GetRoot());
//    std::cout << std::endl;
//    a.PrintTree(iter,0);
//    std::cout << "im done";
  }
  catch (const std::invalid_argument &e) {
    std::cerr << e.what();
  }
  catch (const std::logic_error &e) {
    std::cerr << e.what();
  }
#endif

#ifndef TRY_BOR

  std::cout << ' ' * 4;
#endif

  return 0;
}