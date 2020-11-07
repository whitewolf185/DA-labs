#include "TPatricia.h"

template<class T>
NPatricia::TNode<T> *NPatricia::TPatricia<T>::Find(T &value) {
  if (header == nullptr) {
    return nullptr;
  }

  TNode<T> *iter = header->left;
  size_t prevBit = header->bit;

  while (iter->bit > prevBit) {
    prevBit = iter->bit;
    if (GetBit(value, iter->bit)) {
      iter = iter->right;
    }
    else {
      iter = iter->left;
    }
  }
}

