#ifndef DA_LABS_TPATRICIA_H
#define DA_LABS_TPATRICIA_H

#include <cstdio>
#include "TString.h"


namespace NPatricia {

  size_t GetBitFromString(const NMyString::TString &tmpString);

  struct TData {
    size_t key;
    size_t digits;

    TData() : digits(0), key(0) {}

    TData(const NMyString::TString &str, const size_t &dig) : digits(dig) {
      key = GetBitFromString(str);
    }

    explicit TData(const NMyString::TString &str) : digits(0) {
      key = GetBitFromString(str);
    }

    friend std::ostream &operator<<(std::ostream &cout, const TData &outVal) {
      cout << outVal.digits;
      return cout;
    }

    bool operator!=(const TData &rhs) const {
      return this->key != rhs.key;
    }

    friend bool operator!=(const TData &lhs, const TData &rhs) {
      return lhs.key != rhs.key;
    }

    friend bool operator==(const TData &lhs, const TData &rhs) {
      return lhs.key == rhs.key;
    }

    friend bool operator<(const TData &lhs, const TData &rhs) {
      return lhs.key < rhs.key;
    }

    friend bool operator>(const TData &lhs, const TData &rhs) {
      return lhs.key > rhs.key;
    }
  };

  template<class T>
  struct TNode {
    T val;
    size_t bit;
    TNode<T> *left;
    TNode<T> *right;

    TNode(T &value, size_t bites) : val(value), bit(bites), left(nullptr), right(nullptr) {}

    friend std::ostream &operator<<(std::ostream &cout, const TNode<T> &node) {
      cout << node.val;
      return cout;
    }

    ~TNode();
  };

  template<class T>
  class TPatricia {
  private:
    TNode<T> *header;

    bool GetBit(const T &value, size_t bit);

    size_t FirstDiff(const T &lhs, const T &rhs) {
      size_t i;
      for (i = 0; GetBit(lhs, i) == GetBit(rhs, i); ++i) {}
      return i;
    }

    TNode<T> *Find(const T &);

  public:

    void FinalFind(const T &value) {
      auto finder = Find(value);
      if (finder == nullptr) {
        std::cout << "Exist\n";
        return;
      }
      std::cout << "OK: " << finder << '\n';
    }

    void Insert(const T &);

    void Erase(T &);

    TPatricia() : header(nullptr) {}

    ~TPatricia();
  };

}
#endif //DA_LABS_TPATRICIA_H
