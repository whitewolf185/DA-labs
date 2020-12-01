#include <map>
#include "patricia.h"
#include <time.h>

int main() {
  long double stdMapTime = 0;
  long double patriciaTime = 0;
  long double start, end;
  std::map<std::string, unsigned long long> mp;
  std::ofstream fout;
  std::ifstream fin;

  char input[MAX_LEN];
  unsigned long long value;

  TPatricia<char> *patric;
  try {
    patric = new TPatricia<char>();
  }
  catch (const std::bad_alloc &e) {
    std::cout << "ERROR: fail to allocate the requested storage space\n";
    exit(0);
  }

  TNode<char> *node;

  while ((std::cin >> input)) {
    if (!std::strcmp(input, "+")) {
      std::cin >> input;
      LowerCase(input);
      std::cin >> value;

      start = clock();
      patric->Insert(input, value);
      end = clock();

      patriciaTime = patriciaTime + end - start;

      // std::map
      start = clock();
      if (mp.find(input) == mp.end()) {
        mp[input] = value;
      }
      end = clock();
      stdMapTime = stdMapTime + end - start;
    }
    else if (!std::strcmp(input, "-")) {
      std::cin >> input;
      LowerCase(input);

      start = clock();
      patric->Delete(input);
      end = clock();
      patriciaTime += end - start;


      // std::map
      start = clock();
      if (mp.find(input) != mp.end()) {
        mp.erase(input);
      }
      end = clock();
      stdMapTime = stdMapTime + end - start;
    }

    else {
      LowerCase(input);
      start = clock();
      node = patric->Find(input);
      end = clock();

      patriciaTime += end - start;

      start = clock();
      mp.find(input);
      end = clock();
      stdMapTime = stdMapTime + end - start;

    }
  }

  delete patric;
  printf("patricia: %.3Lf ms\n", patriciaTime / 1000.0);
  printf("std::map: %.3Lf ms\n", stdMapTime / 1000.0);
  return 0;
}