#ifndef LONG_ALG_LAB6_LONG_NUM_H
#define LONG_ALG_LAB6_LONG_NUM_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <exception>
#include <iomanip>

//#define DEBUG

const int RADDIX = 4; //это у меня степень десятки
const int32_t BASE = pow(10, RADDIX);

class TLongAlg {
private:
  std::vector<int32_t> _data;

  int32_t ConvertToInt(const std::string &str) {
    int32_t result = 0;

    for (int i = 0; i < str.size(); ++i) {
      result = result * 10 + str[i] - '0';
    }

    return result;
  }

public:
  TLongAlg(const TLongAlg &obj) {
    _data = obj._data;
  }

  TLongAlg() = default;

  TLongAlg(const std::string &str) {
    int i;
    for (i = str.size() - 1; i > 0; i -= RADDIX) {
      if (i < RADDIX) {
        _data.push_back(ConvertToInt(str.substr(0, i + 1)));
      }

      else {
        _data.push_back(ConvertToInt(str.substr(i - RADDIX + 1, RADDIX)));
      }
    }

    if (i == 0) {
      _data.push_back(ConvertToInt(str.substr(0, 1)));
    }
  }

  TLongAlg(const int32_t &m) {
    _data.push_back(m);
  }

  std::vector<int32_t> ShowData() const {
    return _data;
  }


  //сложение
  friend TLongAlg operator+(const TLongAlg &lhs, const TLongAlg &rhs) {
    int32_t carry = 0;
    TLongAlg result = lhs;

    for (size_t i = 0; i < fmax(result._data.size(), rhs._data.size()) || carry; ++i) {
      if (i == result._data.size()) {
        result._data.push_back(0);
      }

      result._data[i] += carry + (i < rhs._data.size() ? rhs._data[i] : 0);
      carry = result._data[i] >= BASE;

      if (carry) {
        result._data[i] -= BASE;
      }
    }

    return result;
  }

  //больше
  friend bool operator>(const TLongAlg &lhs, const TLongAlg &rhs) {
    if (lhs._data.size() == rhs._data.size()) {
      for (int i = lhs._data.size() - 1; i >= 0; --i) {
        if (lhs._data[i] != rhs._data[i]) {
          return lhs._data[i] > rhs._data[i];
        }
      }
      return false;
    }

    return lhs._data.size() > rhs._data.size();
  }


  //меньше
  friend bool operator<(const TLongAlg &lhs, const TLongAlg &rhs) {
    if (lhs._data.size() == rhs._data.size()) {
      for (int i = lhs._data.size() - 1; i >= 0; --i) {
        if (lhs._data[i] != rhs._data[i]) {
          return lhs._data[i] < rhs._data[i];
        }
      }
      return false;
    }

    return lhs._data.size() < rhs._data.size();
  }

  //проверка равенства
  friend bool operator==(const TLongAlg &lhs, const TLongAlg &rhs) {
    if (lhs._data.size() != rhs._data.size()) {
      return false;
    }

    for (int i = lhs._data.size() - 1; i >= 0; --i) {
      if (lhs._data[i] != rhs._data[i]) {
        return false;
      }
    }
    return true;
  }

  //вычитание
  friend TLongAlg operator-(const TLongAlg &lhs, const TLongAlg &rhs) {
    if (lhs < rhs) {
      throw std::invalid_argument("Error");
    }

    int32_t carry = 0;
    TLongAlg result = lhs;

    for (size_t i = 0; i < lhs._data.size() || carry; ++i) {
      result._data[i] -= carry + (i < rhs._data.size() ? rhs._data[i] : 0);
      carry = result._data[i] < 0;
      if (carry) {
        result._data[i] += BASE;
      }
    }

    while (result._data.size() > 1 && result._data.back() == 0) {
      result._data.pop_back();
    }

    return result;
  }

  //умножение
  friend TLongAlg operator*(const TLongAlg &lhs, const TLongAlg &rhs) {

    TLongAlg result;
    result._data.resize(lhs._data.size() + rhs._data.size(), 0);

    for (int i = 0; i < rhs._data.size(); ++i) {
      for (int32_t carry = 0, j = 0; j < lhs._data.size() || carry; ++j) {
        int32_t current = result._data[i + j] + rhs._data[i] * 1ll * (j < lhs._data.size() ? lhs._data[j] : 0) + carry;
        result._data[i + j] = current % BASE;
        carry = current / BASE;
      }
    }

    while (result._data.size() > 1 && result._data.back() == 0) {
      result._data.pop_back();
    }

    return result;
  }

  //деление
  friend TLongAlg operator/(const TLongAlg &lhs, const TLongAlg &rhs) {
    TLongAlg result;
    TLongAlg peaceOfDigit;

    if (lhs < rhs) {
      return TLongAlg("0");
    }

    for (int i = lhs._data.size() - 1; i >= 0; --i) {
      int32_t l = -1;
      int32_t r = BASE;
      peaceOfDigit._data.insert(peaceOfDigit._data.begin(), lhs._data[i]);

      while (l + 1 < r) {
        int32_t m = (l + r) / 2;
        auto tmp = rhs * TLongAlg(m);
        (!(peaceOfDigit < rhs * TLongAlg(m))) ? l = m : r = m;
      }

      result._data.push_back(l);
      peaceOfDigit = peaceOfDigit - TLongAlg(l) * rhs;
    }

    //концовочка
    std::reverse(result._data.begin(), result._data.end());
    while (result._data.size() > 1 && result._data.back() == 0) {
      result._data.pop_back();
    }
    return result;
  }

  //возведение в степень
  friend TLongAlg operator^(const TLongAlg &lhs, TLongAlg rhs) {
    TLongAlg result = TLongAlg("1");
    if (lhs._data.size() == 1 && lhs._data[0] == 0 && rhs._data.size() == 1 && rhs._data[0] == 0) {
      throw std::invalid_argument("Error");
    }
    if (lhs._data.size() == 1 && lhs._data[0] == 0) {
      return TLongAlg("0");
    }
    if (rhs._data.size() == 1 && rhs._data[0] == 0) {
      return TLongAlg("1");
    }

    auto a = lhs;
    auto size = rhs._data.size();
    for (int i = 0; i < size; ++i) {
      while (rhs._data[i] > 0) {
        if (rhs._data[i] & 1) {
          result = result * a;
        }
        a = a * a;
        rhs._data[i] /= 2;
      }
    }


    return result;
  }


  //вывод
  friend std::ostream &operator<<(std::ostream &out, const TLongAlg &obj) {
    if (obj._data.empty()) {
      out << "0";
      return out;
    }

    out << obj._data.back();
    for (int i = obj._data.size() - 2; i >= 0; --i) {
      out << std::setfill('0') << std::setw(RADDIX) << obj._data[i];
    }

    return out;
  }

};

#endif //LONG_ALG_LAB6_LONG_NUM_H
