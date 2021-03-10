#include "long_num.h"
#include <ctime>

int main() {
  std::string str1;
  std::string str2;
  char op;
  clock_t start, end;

  start = clock();
  while (std::cin >> str1 >> str2 >> op) {
    TLongAlg a(str1);
    TLongAlg b(str2);
    TLongAlg res;

    //сложение
    if (op == '+') {
      try {
        res = a + b;
//        std::cout << res << std::endl;
      }
      catch (const std::invalid_argument &e) {
//        std::cout << e.what() << std::endl;
      }
    }

    //вычитание
    if (op == '-') {
      try {
        res = a - b;
//        std::cout << res << std::endl;
      }
      catch (const std::invalid_argument &e) {
//        std::cout << e.what() << std::endl;
      }
    }

    if (op == '*') {
      try {
        res = a * b;
//        std::cout << res << std::endl;
      }
      catch (const std::invalid_argument &e) {
//        std::cout << e.what() << std::endl;
      }
    }

    if (op == '/') {
      try {
        res = a / b;
//        std::cout << res << std::endl;
      }
      catch (const std::invalid_argument &e) {
//        std::cout << e.what() << std::endl;
      }
    }

    if (op == '^') {
      try {
        res = a ^ b;
//        std::cout << res << std::endl;
      }
      catch (const std::invalid_argument &e) {
//        std::cout << e.what() << std::endl;
      }
    }

    if (op == '<') {
//      (a < b) ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
    }

    if (op == '>') {
//      (a > b) ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
    }

    if (op == '=') {
//      (a == b) ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
    }

  }
  end = clock();


  std::cout << (end - start) / 1000.0 << " ms\n";
  return 0;
}

//298590200129603503574987989737953624387021361057470866305663516339940017116334383632113653752435315363002944346853571966
//9473369926944777334496169091676424880560342702474920723544458971996052254428302142873265551280786496675644168739612670774313153057

//298590200129603503574987989737953624387021361057470866305663516339940017116334383632113653752435315363002944346853571966
//9473369926944777334496169091676424880560342702474920723544458971996052254428302142873265551280786496675644168739612670774313153057

// 0^0 = error