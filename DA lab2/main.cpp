#include "TPatricia.h"

int main() {
  char input[MAX_KEY];
  NPatricia::TPatricia<NPatricia::TData> patric;
  size_t curVal;

  while (scanf("%s", input) > 0) {
    //input
    if (input[0] == '+') {
      scanf("%s %llu", input, &curVal);
      NMyString::TString inputStr = input;
      NPatricia::TData inData(inputStr, curVal);
      patric.Insert(inData);
    }

      //erase
    else if (input[0] == '-') {
      scanf("%s", input);
      NMyString::TString inputStr = input;
      NPatricia::TData inData(input);
      patric.Erase(inData);
    }

      //find
    else {
      scanf("%s", input);
      NMyString::TString inputStr = input;
      NPatricia::TData inData(input);
      patric.FinalFind(inData);
    }
  }
  return 0;
}
