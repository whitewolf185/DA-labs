#ifndef DA_LABS_TPATRICIA_H
#define DA_LABS_TPATRICIA_H

#include <cstdio>
#include "TString.h"


namespace NPatricia {

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
