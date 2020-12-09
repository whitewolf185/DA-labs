#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <exception>
#include <set>
#include <list>

//#define DEBUG

const int SKIP_ALL = -10;
const int UNDEFINED = -5;

struct TValue {
  unsigned int val;
  unsigned int lineCount;
  unsigned int wordCount;
};

struct TAnsVal {
  unsigned lineCount;
  unsigned wordCount;
};

bool operator==(const unsigned lhs, const TValue &rhs) {
  return lhs == rhs.val;
}

std::ostream &operator<<(std::ostream &out, const TAnsVal obj) {
  out << obj.lineCount << ", " << obj.wordCount;
  return out;
}


class TTBM {
private:
  std::map<unsigned, std::vector<int>> rightIdPattern;
  std::vector<unsigned> Pattern;
  std::vector<TValue> text;
  std::set<unsigned> ALPHABET;
  std::vector<int> arrGsN;
  std::vector<int> M;
  std::vector<int> N;

//мапка нужна для того, чтобы в массиве table сопостовлять алфавит с индексами
  void PatternParser() {
    char c;
    std::string str;
    c = getchar();
    unsigned id = 0;
    bool spaceFlag = false;
    while (c != '\n') {
      if (c == ' ' && spaceFlag) {
        unsigned doomGuy = std::stoi(str);
        Pattern.push_back(doomGuy);
        auto search = rightIdPattern.find(doomGuy);
        if (search != rightIdPattern.end()) {
          search->second.push_back(id);
        }
        else {
          rightIdPattern.insert({doomGuy, std::vector<int>(id)});
        }
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
      auto search = rightIdPattern.find(doomGuy);
      if (search != rightIdPattern.end()) {
        search->second.push_back(id);
      }
      else {
        rightIdPattern.insert({doomGuy, std::vector<int>(id)});
      }
      ++id;
      ALPHABET.insert(doomGuy);
      spaceFlag = false;
      str.clear();
    }
  }

  void TextParser() {
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


  static std::vector<int> Z(std::vector<unsigned> pat) {
    int size = pat.size();
    std::vector<int> z(size);
    int L = 0, R = 0;


    for (int i = 1; i < size; ++i) {
      if (i < R && i >= L) {
        int min = std::min(R - i, z[i - L]);
        if (min == R - i) {

          //наивный
          for (int j = R; pat[j] == pat[j - R] && j < size; ++j) {
            ++min;
          }
        }
        z[i] = min;
      }

      else {
        L = i;
        R = L + z[i];

        //наивный
        while (i + z[i] < size && pat[z[i]] == pat[i + z[i]]) {
          ++z[i];
        }
      }
    }

    return z;
  }


public:
  TTBM() {
    PatternParser();
    TextParser();
    int size = Pattern.size();
    std::vector<unsigned> revPat = Pattern;

    std::reverse(revPat.begin(), revPat.end());

    std::vector<int> z;
    z = Z(revPat);

    std::reverse(z.begin(), z.end());

    arrGsN = z;

    M.resize(text.size(), UNDEFINED);

  }

  int RoolBadChar(int patternPos) {
    auto search = rightIdPattern.find(Pattern[patternPos]);
    if (search != rightIdPattern.end()) {
      auto elem = std::lower_bound(search->second.begin(), search->second.end(), patternPos);
      if (elem == search->second.end()) {
        return SKIP_ALL;
      }
      return patternPos - (*elem);
    }
    return SKIP_ALL;
  }

  int RoolGoodSuff(int patterPos) {
    int size = Pattern.size();

    int alpha = size - patterPos;

    for (int i = size; i < 0; --i) {
      if (arrGsN[i - 1] == alpha && i - 1 < patterPos) {
        return i - 1;
      }
    }
    return SKIP_ALL;
  }

  std::vector<TAnsVal> Do() {
    std::vector<TAnsVal> ans;
    if (Pattern.size() == 0) {
      return ans;
    }

    int i = 0;
    int u = 0;

    //если текст меньше шаблона
    if (i > text.size()) {
      return ans;
    }

    int curI = i;
    int curJ = j;

    int shift = j;
    while (i < text.size()) {

      if (M[curI] == UNDEFINED || (M[curI] == 0 && arrGsN[curJ] == M[curI])) {
        if (Pattern[curJ] == text[curI]) {
          if (curJ == 0) {
            ans.push_back({text[curI].lineCount, text[curI].wordCount});
            M[i] = Pattern.size() - 1;
            i += std::min((int) (i + shift), (int) (text.size() - shift));
            curJ = j;
            curI = i;
          }
          else {
            --curI;
            --curJ;
          }//curJ == 0
        }

        else {
          M[i] = i - curI;
          int bmBc = RoolBadChar(curJ);
          int bmGs = RoolGoodSuff(curJ);

          if (bmBc == SKIP_ALL && bmGs == SKIP_ALL) {
            shift = j;
            i += std::min((int) (i + shift), (int) (text.size() - shift));
            curI = i;
            curJ = j;
            continue;
          }

          shift = std::max(bmGs + 1, std::max(bmBc, 1));
          i += std::min((int) (i + shift), (int) (text.size() - shift));
          curI = i;
          curJ = j;
          continue;

        }//Pattern[curJ] == text[curI]
      }

      else if () {

      }//1 вариант
    }
  }
};


int main() {
  std::vector<unsigned> Pattern;
  std::vector<TValue> Text;
  std::map<unsigned, std::list<int>> mapPattern;
  std::vector<TAnsVal> ans;

  PatternParser(Pattern, mapPattern);
  try {
    TextParser(Text);
  }
  catch (const std::invalid_argument &er) {
    std::terminate();
  }


  ans = TTBM(Pattern, Text, mapPattern);

  for (auto i : ans) {
    std::cout << i << std::endl;
  }

  return 0;
}