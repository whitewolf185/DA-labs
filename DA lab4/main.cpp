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
        str.clear();
        spaceFlag = false;
      }
      else if (c == '\n') {
        if (spaceFlag) {
          ++wordCount;
          unsigned doomGuy;
          doomGuy = std::stoi(str);
          text.push_back({doomGuy, lineCount, wordCount});
          str.clear();
        }
        wordCount = 0;
        ++lineCount;
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

    const int m = Pattern.size();
    const int n = text.size();

    //если текст меньше шаблона
    if (Pattern.size() > text.size()) {
      return ans;
    }

    int shift = m - 1;

    while (i <= n - m) {
      int j = m - 1;

      while (j >= 0) {
        if (M[i + j] == UNDEFINED || (M[i + j] == arrGsN[j] && arrGsN[j] == 0)) {
          if (Pattern[j] == text[i + j]) {
            if (j <= 0) {
              M[i + m - 1] = m - 1;
              ans.push_back({text[i].lineCount, text[i].wordCount});
              shift = m - 1;
              break;
            }
            else {
              --j;
            }
          }

          else {
            M[i + m - 1] = m - 1 - j;
            int bmGs = RoolGoodSuff(j);
            int bmBc = RoolBadChar(j);
            shift = std::max(bmGs, std::max(bmBc, 1));
            break;
          }
        }//1 вариант

        else if (M[i + j] < arrGsN[j]) {
          j -= M[i + j];
          if (j <= 0) {
            ans.push_back({text[i].lineCount, text[i].wordCount});
            shift = m - 1;

            break;
          }
        }//2 вариант

        else if (M[i + j] >= arrGsN[j] && arrGsN[j] == j) {
          M[i + m - 1] = m - 1 - j;
          ans.push_back({text[i].lineCount, text[i].wordCount});
          int bmGs = RoolGoodSuff(j);
          int bmBc = RoolBadChar(j);
          shift = std::max(bmGs, std::max(bmBc, 1));

          break;
        }//3 вариант

        else if (M[i + j] > arrGsN[j] && arrGsN[j] < j) {
          M[i + j] = m - 1 - j + arrGsN[j];
          int bmGs = RoolGoodSuff(j);
          int bmBc = RoolBadChar(j);
          shift = std::max(bmGs, std::max(bmBc, 1));

          break;
        }//4 вариант

        else if (M[i + j] == arrGsN[j] && arrGsN[j] > 0 && arrGsN[j] < j) {
          j -= M[i + j];
          if (j <= 0) {
            ans.push_back({text[i].lineCount, text[i].wordCount});
            shift = m - 1;

            break;
          }
        }//5 вариант

        else {
          std::cout << "Some goes wrong" << std::endl;
          break;
        }
      }

      i += shift;
    }
    return ans;
  }
};


int main() {
  TTBM tbm;
  std::vector<TAnsVal> ans;

  ans = tbm.Do();

  if (ans.empty()) {
    std::cout << "Im empty" << std::endl;
  }

  for (auto i : ans) {
    std::cout << i << std::endl;
  }

  return 0;
}