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
    std::unique_ptr<TBorNode> url;
//    std::unique_ptr<TBorNode> prev;
    std::unordered_map<char, TBorNode *> next;
    unsigned id;

    //constructors
    //Конструктор. Первые 2 аргумента всегда должны быть одни и те же. Последние 2 - начало и общий конец.
    TBorNode(TBorNode *root, unsigned &globID, const unsigned &_begin, const std::shared_ptr<unsigned> &end) {
      id = globID++;
      last = end;
      begin = _begin;
      url.reset(root);
    }

    //Конструктор с задаваемой в конце end-ом, как число. Нужен для вставки ноды не как лист.
    TBorNode(TBorNode *root, unsigned &globID, const unsigned &_begin, const unsigned &end) {
      id = globID++;
      last.reset(new unsigned(end));
      begin = _begin;
      url.reset(root);
      leaf = false;
    }

    TBorNode() = delete;

    //destructors
    ~TBorNode() = default;

    //functions
    //Ф нужна для вывода данных в ноде. Нужна больше для отладки
    void PrintNode(const std::string &text) {
      for (int i = begin; i <= *last; ++i) {
        (text[i] == FANTOM_CHAR1 || text[i] == FANTOM_CHAR2) ? std::cout << " " : std::cout << text[i];
      }
      std::cout << "|";
    }

    //Нужна для вставки следующего элемента. В аргументах пара,
    // в которой должно содержаться первая буква и указатель со вставляемой нодой.
    void Insert(const std::pair<char, TBorNode *> &tmp_pair) {
      next.insert(tmp_pair);
      leaf = false;
    }

    //Ф получает ноду по символу. Нужно, чтобы доставать нужную ноду на развилке.
    // Обычно используется в Ф Next
    TBorNode *GetNodeElem(const char &c) {
      auto tmp = next.find(c);
      if (tmp == next.end()) {
        return nullptr;
      }
      return tmp->second;
    }
  };

protected:
  TBorNode *root;


//----functions
//    ------------------------------TEST FUNCTIONS----------------------------------

private:
  void TestPrinter() {
    for (auto item : root->next) {
      item.second->PrintNode(texts);
      std::cout << std::endl;
    }
  }
//    -----------------------------------END----------------------------------------

public:
  TBoris() {
    end.reset(new unsigned int(0));
    root = new TBorNode(root, globID, 0, end);
  };

  ~TBoris() {
    for (auto &item : root->next) {
      delete item.second;
    }
  }

  void SetText(const std::string &str1, const std::string &str2) {
    texts = str1 + FANTOM_CHAR1 + str2 + FANTOM_CHAR2;
  }

//--------------------------------ITERATOR-------------------------------
  class TIterator {
  private:
    int reminder = 0;
    int activeLen = 0;
    std::unique_ptr<TBorNode> prevUrl;
    std::unique_ptr<TBorNode> activeNode;

  public:
    TIterator() {
      prevUrl = nullptr;
      activeNode = nullptr;
    }

    TIterator(TBorNode *node) {
      activeNode.reset(node);
      prevUrl = nullptr;
    }

    ~TIterator() = default;

    //Ф перехода на следующую ноду
    void Next(const char &c) {
      if (activeNode == nullptr) {
        throw std::logic_error("Trying to call nullptr");
      }

      TBorNode *tmp = activeNode->GetNodeElem(c);
      if (tmp != nullptr) {
        activeNode.reset(tmp);
      }
    }

    //Ф перехода по суфф ссылке
    void GoThrowURL() {
      if (activeNode->url == nullptr) {
        throw std::logic_error("Trying to call nullptr in URL");
      }

      prevUrl = std::move(activeNode);
      activeNode = std::move(activeNode->url);
    }

  };
//--------------------------END OF ITERATOR--------------------------

  void Test() {
    *end = texts.size() - 1;
    root->Insert(std::pair<char, TBorNode *>(texts[0], new TBorNode(root, globID, 0, end)));

    TestPrinter();
  }


};


#endif //SUFF_TREE_LAB5_MAIN_H
