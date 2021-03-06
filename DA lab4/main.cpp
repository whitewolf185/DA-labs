#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

const int UNDEFINED = -5;

struct TValue {
  unsigned val;
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

bool operator!=(const unsigned lhs, const TValue &rhs) {
  return lhs != rhs.val;
}

std::ostream &operator<<(std::ostream &out, const TAnsVal obj) {
  out << obj.lineCount << ", " << obj.wordCount;
  return out;
}

class TTBM {
private:
  std::unordered_map<unsigned, int> rightIdPattern;
  std::vector<unsigned> Pattern;
  std::vector<TValue> text;
  std::vector<int> arrGsN;
  std::vector<int> M;
  std::string str;

//мапка нужна для того, чтобы в массиве table сопостовлять алфавит с индексами
  void PatternParser() {
    str.clear();
    char c;
    c = getchar();
    //нужно для вставки
    unsigned id = 1;
    bool spaceFlag = false;
    while (c != '\n' && c != -1) {
      if (c == ' ' && spaceFlag) {
        unsigned doomGuy = std::stoul(str);
        Pattern.push_back(doomGuy);
        ++id;
        spaceFlag = false;
        str.clear();
      }
      else if (c >= '0' && c <= '9') {
        str.push_back(c);
        spaceFlag = true;
      }
      c = getchar();
    }

    if (spaceFlag) {
      unsigned doomGuy = std::stoul(str);
      Pattern.push_back(doomGuy);
      ++id;
    }
  }

  void TextParser() {
    str.clear();
    char c;
    unsigned lineCount = 1;
    unsigned wordCount = 0;
    bool spaceFlag = false;

    while ((c = getchar()) != EOF) {
      if (c == ' ' && spaceFlag) {
        ++wordCount;
        unsigned doomGuy;
        doomGuy = std::stoul(str);
        text.push_back({doomGuy, lineCount, wordCount});
        str.clear();
        spaceFlag = false;
      }
      else if (c == '\n') {
        if (spaceFlag) {
          ++wordCount;
          unsigned doomGuy;
          doomGuy = std::stoul(str);
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
    }
    if (spaceFlag) {
      ++wordCount;
      unsigned doomGuy;
      doomGuy = std::stoul(str);
      text.push_back({doomGuy, lineCount, wordCount});
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

    RoolBadChar();
    RoolGoodSuff();

    M.resize(text.size() + 1, UNDEFINED);

  }

  void RoolBadChar() {
    for (int i = 0; i < (int) Pattern.size() - 1; ++i) {
      rightIdPattern[Pattern[i]] = i;
    }
  }

  void RoolGoodSuff() {
    int n = (int) Pattern.size();
    std::vector<unsigned int> revPat = Pattern;
    std::reverse(revPat.begin(), revPat.end());
    std::vector<int> zFunc = Z(revPat);
    std::vector<int> N(zFunc.rbegin(), zFunc.rend());
    std::vector<int> L(n + 1, n);

    int j;
    for (int i = 0; i < n - 1; ++i) {
      j = n - N[i];
      L[j] = i;
    }

    std::vector<int> l(n + 1, n);
    for (int i = n - 1; i >= 0; --i) {
      j = n - i;
      if (N[j - 1] == j) {
        l[i] = (j - 1);
      }
      else {
        l[i] = l[i + 1];
      }
    }


    for (int i = 0; i < n + 1; ++i) {
      if (L[i] == n) {
        L[i] = l[i];
      }
    }

    for (auto &x : L) {
      if (x != n) {
        x = n - 1 - x;
      }
    }

    arrGsN = L;
  }

  std::vector<TAnsVal> Do() {
    std::vector<TAnsVal> ans;
    //если паттерн пустой, то мы и найти ничего не сможем
    if (Pattern.empty()) {
      return ans;
    }

    //если текст меньше шаблона
    if (Pattern.size() > text.size()) {
      return ans;
    }

    const int m = Pattern.size();
    const int n = text.size();
    int j = m, bound = 0, shift = 0;

    for (int i = 0; i < 1 + n - m;) {
      //1 вариант
      if (M[i + j - 1] == UNDEFINED || (M[i + j - 1] == 0 && arrGsN[j] == M[i + j - 1])) {
        //тут чистый БМ
        //arrGsN - это массив сдвивов правила хорошего суффикса
        for (j = m - 1; j >= bound; j--) {
          if (Pattern[j] != text[i + j]) {
            M[i + m - 1] = m - 1 - j;
            break;
          }
        }
        if (j < bound) {
          ans.push_back({text[i].lineCount, text[i].wordCount});
          bound = m - arrGsN[0];
          j = -1;
        }
        else {
          bound = 0;
        }
        if (j < bound) {
          shift = arrGsN[j + 1];
          j = m;
        }
        else {
          shift = std::max({1, arrGsN[j + 1], j - rightIdPattern[text[i + j].val]});
          j = m;
        }
      }

        //2 вариан
      else if (M[i + j - 1] < arrGsN[j]) {
        j -= M[i + j - 1];

        if (j <= 0) {
          ans.push_back({text[i].lineCount, text[i].wordCount});
          bound = m - arrGsN[0];
          shift = arrGsN[0];
          j = m;
        }
      }

        //3 вариант
      else if (M[i + j - 1] >= arrGsN[j] && arrGsN[j] == j) {
        M[i + m - 1] = m - 1 - j;
        ans.push_back({text[i].lineCount, text[i].wordCount});
        shift = std::max({1, arrGsN[j + 1], j - rightIdPattern[text[i + j].val]});
        j = m;
      }

        //4 вариант
      else if (M[i + j - 1] > arrGsN[j] && arrGsN[j] < j) {
        M[i + j - 1] = m - 1 - j + arrGsN[j];
        shift = std::max({1, arrGsN[j + 1], j - rightIdPattern[text[i + j].val]});
        j = m;
      }

        //5 вариант
      else if (M[i + j - 1] == arrGsN[j] && arrGsN[j] > 0 && arrGsN[j] < j) {
        j -= M[i + j - 1];

        if (j <= 0) {
          ans.push_back({text[i].lineCount, text[i].wordCount});
          bound = m - arrGsN[0];
          shift = arrGsN[0];
          j = m;
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
  for (auto i : ans) {
    std::cout << i << std::endl;
  }
  return 0;
}