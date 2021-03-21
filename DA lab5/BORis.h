#ifndef SUFF_TREE_LAB5_MAIN_H
#define SUFF_TREE_LAB5_MAIN_H

#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <vector>


class TBoris {
private:
  std::vector<std::string> texts;
  static const char FANTOM_CHAR = '$';

  //simply nodes
  struct TBorNode {
    std::string data;
    TBorNode *url = nullptr;
    TBorNode *next = nullptr;

    TBorNode() = default;

    ~TBorNode() {
      if (this->next != nullptr) {
        delete this->next;
      }
    }
  };

  //root of all nodes
  struct TRootBorNode {
    std::vector<TBorNode *> nodes;

    ~TRootBorNode() = default;

    void Push(const std::string &str) {
      TBorNode *tmp = new TBorNode;
      tmp->data = str;
      nodes.push_back(tmp);
    }
  };


  TRootBorNode root;

  void TestInsert(const std::string &str) {

  }

  void TestPrinter(int i) {

  }

public:
  TBoris() = default;

  ~TBoris() {
    while (!root.nodes.empty()) {
      delete root.nodes.back();
      root.nodes.pop_back();
    }
  }

  void SetText(const std::string &str) {
    texts.push_back(str + FANTOM_CHAR);
  }

  //size of vector texts. Needed to know how many patterns i have
  unsigned int Size() const {
    return texts.size();
  }

  void Test() {
    for (int i = 0; i < Size(); ++i) {
      TestInsert(texts[i]);
    }

    for (int i = 0; i < Size(); ++i) {
      TestPrinter(i);
    }
  }


};


#endif //SUFF_TREE_LAB5_MAIN_H
