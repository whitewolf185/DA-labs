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

  if (iter->val == value) {
    return iter;
  }
  else {
    return nullptr;
  }
}


template<class T>
void NPatricia::TPatricia<T>::Insert(T &value) {
  if (header == nullptr) {
    header = new TNode<T>(value, 0);
    header->left = header;
    return;
  }

  TNode<T> *iter = header->left;
  TNode<T> *prev = header;

  while (iter->bit > prev->bit) {
    prev = iter;
    if (GetBit(value, iter->bit)) {
      iter = iter->right;
    }
    else {
      iter = iter->left;
    }
  }

  if (iter->val == value) {
    return;
  }

  size_t bit = FirstDiff(iter->val, value);
  iter = header->left;
  prev = header;

  while (iter->bit > prev->bit && bit > iter->bit) {
    prev = iter;
    if (GetBit(value, iter->bit)) {
      iter = iter->right;
    }
    else {
      iter = iter->left;
    }
  }

  auto *newNode = new TNode<T>(value, bit);
  if (prev->left == iter) {
    prev->left = newNode;
  }
  else {
    prev->right = newNode;
  }

  if (GetBit(newNode->val, newNode->bit)) {
    newNode->right = newNode;
    newNode->left = iter;
  }
  else {
    newNode->left = newNode;
    newNode->right = iter;
  }
}