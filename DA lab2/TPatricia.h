#ifndef DA_LABS_TPATRICIA_H
#define DA_LABS_TPATRICIA_H

#include <cstdio>
#include "TString.h"

//TODO я могу просто зашифровать ключ и работать с ним,
// ведь получение ключа от меня не требуеются

namespace NPatricia {

  size_t GetBitFromString(const NMyString::TString &tmpString) {
    size_t result = 0;

    for (char i : tmpString) {
      if (i >= 'A' && i <= 'Z') {
        result = result * 26 + i - 'A';
      }
      else if (i >= 'a' && i <= 'z') {
        result = result * 26 + i - 'a';
      }
    }
    return result;
  }

  struct TData {
    NMyString::TString key;
    size_t digits;

    TData() : digits(0) {}
  };

  template<class T>
  struct TNode {
    T val;
    size_t bit;
    TNode<T> *left;
    TNode<T> *right;

    TNode(T &value, size_t bites) : val(value), bit(bites), left(nullptr), right(nullptr) {}

    ~TNode();
  };

  template<class T>
  class TPatricia {
  private:
    TNode<T> *header;

    bool GetBit(const T &value, size_t bit) {
      return ((1 << (bit - 1)) & value);
    }

    size_t FirstDiff(const T &lhs, const T &rhs) {
      size_t i;
      for (i = 0; get_bit(lhs, i) == get_bit(rhs, i); ++i) {}
      return i;
    }

  public:
    TNode<T> *Find(T &);

    void Insert(T &);

    void Erase(T &);

    void Key(T &);

    TPatricia() : header(nullptr) {}

    ~TPatricia();
  };

}
#endif //DA_LABS_TPATRICIA_H
