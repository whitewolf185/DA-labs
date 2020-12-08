#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <exception>


struct TValue {
  unsigned int val;
  unsigned int lineCount;
  unsigned int wordCount;
};

void PatternParser(std::map<int, int> &Pattern) {
  char c;
  std::string str;
  unsigned id = 1;
  c = getchar();
  bool spaceFlag = false;
  while (c != '\n') {
    if (c == ' ' && spaceFlag) {
      unsigned doomGuy = std::stoi(str);
      Pattern.insert({id, doomGuy});
      ++id;
      spaceFlag = false;
    }
    else {
      str.push_back(c);
      spaceFlag = true;
    }
    c = getchar();
  }
}


void TextParser(std::vector<TValue> &text) {
  char c;
  unsigned lineCount = 1;
  unsigned wordCount = 0;
  std::string str;
  bool spaceFlag = false;

  while ((c = getchar()) != EOF) {
    if (c == ' ' && spaceFlag) {
      ++wordCount;
      unsigned doomGuy;
      doomGuy = std::stoi(str);
      text.push_back({doomGuy, lineCount, wordCount});
      str.clear();
      spaceFlag = false;
    }
    else if (c == '\n') {
      ++lineCount;
      wordCount = 0;
      if (spaceFlag) {
        ++wordCount;
        unsigned doomGuy;
        doomGuy = std::stoi(str);
        text.push_back({doomGuy, lineCount, wordCount});
        str.clear();
      }
      spaceFlag = false;
    }

    else if (c >= '0' && c <= '9') {
      str.push_back(c);
      spaceFlag = true;
    }

    else {
      throw std::invalid_argument("Wrong argument. Check your input\n");
    }
  }
}

void TBM() {

}


int main() {
  std::map<int, int> Pattern;
  std::vector<TValue> Text;

  PatternParser(Pattern);
  TextParser(Text);


  return 0;
}