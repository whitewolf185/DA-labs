#ifndef SUFF_TREE_LAB5_MAIN_H
#define SUFF_TREE_LAB5_MAIN_H

#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>

const char FANTOM_CHAR1 = '$';
const char FANTOM_CHAR2 = '%';

class TBoris {
  //---variables
public:
  std::shared_ptr<unsigned int> end;
private:
  std::string texts;
  unsigned globID = 0;


  //-----------------------simply nodes---------------------
  struct TBorNode {
    //variables
    unsigned int begin;
    std::shared_ptr<unsigned> last;
    bool leaf = true;
//    std::unique_ptr<TBorNode> prev;
    std::unordered_map<char, TBorNode *> next;
    unsigned id;

    //constructors
    TBorNode(unsigned &globID, const unsigned &_begin, const std::shared_ptr<unsigned> &end) {
      id = globID++;
      last = end;
      begin = _begin;
    }

    TBorNode() = delete;

    //destructors
    ~TBorNode() = default;

    //functions
    void PrintNode(const std::string &text) {
      for (int i = begin; i <= *last; ++i) {
        (text[i] == FANTOM_CHAR1 || text[i] == FANTOM_CHAR2) ? std::cout << " " : std::cout << text[i];
      }
      std::cout << "|";
    }
  };

  std::unordered_map<char, TBorNode *> root;


//----functions
//    ------------------------------TEST FUNCTIONS----------------------------------

  void TestPrinter() {
    for (auto item : root) {
      item.second->PrintNode(texts);
      std::cout << std::endl;
    }
  }
//    -----------------------------------END----------------------------------------

public:
  TBoris() {
    end.reset(new unsigned int(0));
  };

  ~TBoris() {
    for (auto &item : root) {
      delete item.second;
    }
  }

  void SetText(const std::string &str1, const std::string &str2) {
    texts = str1 + FANTOM_CHAR1 + str2 + FANTOM_CHAR2;
  }


  void Test() {
    *end = texts.size() - 1;
    root.insert(std::pair<char, TBorNode *>(texts[0], new TBorNode(globID, 0, end)));

    TestPrinter();
  }


};


#endif //SUFF_TREE_LAB5_MAIN_H
