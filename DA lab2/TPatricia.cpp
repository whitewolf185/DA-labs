#include "TPatricia.h"

template<class T>
bool NPatricia::TPatricia<T>::GetBit(const T &value, size_t bit) {
  return ((1u << (bit - 1)) & value);
}

template<>
bool NPatricia::TPatricia<NPatricia::TData>::GetBit(const TData &value, size_t bit) {
  return ((1u << (bit - 1)) & value.key);
}

template<class T>
NPatricia::TNode<T> *NPatricia::TPatricia<T>::Find(const T &value) {
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
  std::cout << "OK\n";
}


template<class T>
void NPatricia::TPatricia<T>::Erase(T &value) {
  //если вообще ничего нет
  if (header == nullptr) {
    std::cout << "NoSuchWord\n";
    return;
  }

  //если слова нет (из-за этого может быть tl)
  NPatricia::TNode<T> *elem = Find(value);
  if (elem == nullptr) {
    std::cout << "NoSuchWord\n";
    return;
  }

  TNode<T> *iter = header->left;
  TNode<T> *prev = header;

  if (header->val == value && (header->left == nullptr || header->left->bit == header->bit)) {
    delete[] header;
    header = nullptr;
    std::cout << "OK\n";
  }

  //-----------случай с ссылкой на себя--------------
  if (elem->right->bit == elem->bit || elem->left->bit == elem->bit) {
    while (iter->bit > prev->bit || iter == elem) {
      prev = iter;
      if (GetBit(value, iter->bit)) {
        iter = iter->right;
      }
      else {
        iter = iter->left;
      }
    }

    if (iter->bit == iter->right->bit) {
      if (prev->right->val == iter->val) {
        prev->right = iter->left;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
      if (prev->left->val == iter->val) {
        prev->left = iter->left;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
    }

    else if (iter->bit == iter->left->bit) {
      if (prev->right->val == iter->val) {
        prev->right = iter->right;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
      if (prev->left->val == iter->val) {
        prev->left = iter->right;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
    }
    return;
  }
  //----конец случая ссылки на себя------

  while (iter->bit > prev->bit) {
    prev = iter;
    if (GetBit(value, iter->bit)) {
      iter = iter->right;
    }
    else {
      iter = iter->left;
    }
  }

  iter->val = prev->val;
  TNode<T> *M;
  auto X = iter;
  if (prev->left != iter) {
    M = prev->left;
  }
  else {
    M = prev->right;
  }

  value = prev->val;

  iter = header->left;
  prev = header;

  while (iter->bit > prev->bit) {
    prev = iter;
    if (GetBit(value, iter->bit)) {
      iter = iter->right;
    }
    else {
      iter = iter->left;
    }
  }

  auto R = prev;
  auto Q = iter;

  iter = header->left;
  prev = header;

  while (iter->bit > prev->bit || Q == iter) {
    prev = iter;
    if (GetBit(value, iter->bit)) {
      iter = iter->right;
    }
    else {
      iter = iter->left;
    }
  }
  auto N = prev;

  //-----------начинаю переприсваивание------------
  if (N->left == Q) {
    N->left = M;
  }
  else {
    N->right = M;
  }

  if (R->left == Q) {
    R->left = X;
  }
  else {
    R->right = X;
  }

  delete[] Q;

}