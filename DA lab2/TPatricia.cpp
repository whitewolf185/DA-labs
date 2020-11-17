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


/*
 #include <cstdio>

namespace PATRICIA {

  template <class T>
  struct Node {
    T value;
    size_t bit;
    Node<T> *left;
    Node<T> *right;

    Node(T &value, size_t bit) : value(value), bit(bit), left(nullptr), right(nullptr) {}
    ~Node();
  };

// биты нумеруются с единицы
  template <class T>
  bool get_bit(const T &value, size_t bit) {
    return ((1 << (bit-1)) & value);
  }

// биты нумеруются с единицы
  template <class T>
  size_t first_diff(const T &a, const T &b) {
    size_t i;
    for (i = 0; get_bit(a, i) == get_bit(b, i); ++i);
    return i;
  }

  template <class T>
  class Tree {
  private:
    Node<T> *header;

  public:
    Node<T>* find(T&);
    void insert(T&);
    void erase(T&);
    void print();

    Tree() : header(nullptr) {}
    ~Tree();
  };

  template <class T>
  Node<T>* Tree<T>::find(T &value) {
    if (header != nullptr) {
      return nullptr;
    }

    Node<T> *iter = header->left;
    Node<T> *prev = header;

    while (iter->bit > prev->bit) {
      prev = iter;
      if (get_bit(value, iter->bit)) {
        iter = iter->right;
      } else {
        iter = iter->left;
      }
    }

    if (iter->value == value) {
      return iter;
    } else {
      return nullptr;
    }
  }

  template <class T>
  void Tree<T>::insert(T &value) {
    if (header == nullptr) {
      header = new Node<T>(value, 0);
      header->left = header;
      return;
    }

    Node<T> *iter = header->left;
    Node<T> *prev = header;

    while (iter->bit > prev->bit) {
      prev = iter;
      if (get_bit(value, iter->bit)) {
        iter = iter->right;
      } else {
        iter = iter->left;
      }
    }

    if (iter->value == value) {
      return;
    }

    size_t bit = first_diff(iter->value, value);
    iter = header->left;
    prev = header;
    while (iter->bit > prev->bit && bit > iter->bit) {
      prev = iter;
      if (get_bit(value, iter->bit)) {
        iter = iter->right;
      } else {
        iter = iter->left;
      }
    }

    Node<T> *node = new Node<T>(value, bit);
    if (prev->left == iter) {
      prev->left = node;
    } else {
      prev->right = node;
    }
    if (get_bit(node->value, node->bit)) {
      node->right = node;
      node->left = iter;
    } else {
      node->left = node;
      node->right = iter;
    }
  }

  template <class T>
  void Tree<T>::erase(T &value) {
    if (header == nullptr) {
      return;
    }

    Node<T> *iter;
    Node<T> *back_prev;
    Node<T> *prev;


  }

  template <class T>
  void print_tree(Node<T> *node, int n) {
    if (node != nullptr) {
      if (node->right != nullptr && node->bit < node->right->bit) {
        print_tree(node->right, n+1);
      }
      printf("%*s%d\n", n*3, "", node->value);
      if (node->left != nullptr && node->bit < node->left->bit) {
        print_tree(node->left, n+1);
      }
    }
  }

  template <class T>
  void Tree<T>::print() {
    print_tree(header, 0);
  }

}
 */
