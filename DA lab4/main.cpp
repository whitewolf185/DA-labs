#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <exception>
#include <set>

//#define DEBUG

std::set<unsigned> ALPHABET;

struct TValue {
  unsigned int val;
  unsigned int lineCount;
  unsigned int wordCount;
};

struct TAnsVal {
  unsigned lineCount;
  unsigned wordCount;
};

//мапка нужна для того, чтобы в массиве table сопостовлять алфавит с индексами
void PatternParser(std::vector<unsigned> &Pattern, std::map<unsigned, unsigned> &mapPattern) {
  char c;
  std::string str;
  c = getchar();
  unsigned id = 1;
  bool spaceFlag = false;
  while (c != '\n') {
    if (c == ' ' && spaceFlag) {
      unsigned doomGuy = std::stoi(str);
      Pattern.push_back(doomGuy);
      mapPattern.insert({doomGuy, id});
      ++id;
      ALPHABET.insert(doomGuy);
      spaceFlag = false;
      str.clear();
    }
    else {
      str.push_back(c);
      spaceFlag = true;
    }
    c = getchar();
  }
  if (spaceFlag) {
    unsigned doomGuy = std::stoi(str);
    Pattern.push_back(doomGuy);
    mapPattern.insert({doomGuy, id});
    ++id;
    ALPHABET.insert(doomGuy);
    spaceFlag = false;
    str.clear();
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
      ALPHABET.insert(doomGuy);
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
        ALPHABET.insert(doomGuy);
        str.clear();
      }
      spaceFlag = false;
    }

    else if (c >= '0' && c <= '9') {
      str.push_back(c);
      spaceFlag = true;
    }

    else if (c != ' ') {
      throw std::invalid_argument("Wrong argument. Check your input\n");
    }
  }
  if (spaceFlag) {
    ++wordCount;
    unsigned doomGuy;
    doomGuy = std::stoi(str);
    text.push_back({doomGuy, lineCount, wordCount});
    ALPHABET.insert(doomGuy);
    str.clear();
  }
}


bool operator==(const unsigned lhs, const TValue &rhs) {
  return lhs == rhs.val;
}


std::vector<TAnsVal> TBM(const std::vector<unsigned> &Pattern, const std::vector<TValue> &Text,
                         const std::map<unsigned, unsigned> &mapPattern) {

}

std::ostream &operator<<(std::ostream &out, const TAnsVal obj) {
  out << obj.lineCount << ", " << obj.wordCount;
  return out;
}


int main() {
  std::vector<unsigned> Pattern;
  std::vector<TValue> Text;
  std::map<unsigned, unsigned> mapPattern;
  std::vector<TAnsVal> ans;

  PatternParser(Pattern, mapPattern);
  try {
    TextParser(Text);
  }
  catch (const std::invalid_argument &er) {
    std::terminate();
  }


  ans = TBM(Pattern, Text, mapPattern);

  for (auto i : ans) {
    std::cout << i << std::endl;
  }

  return 0;
}