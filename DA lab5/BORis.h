#ifndef SUFF_TREE_LAB5_MAIN_H
#define SUFF_TREE_LAB5_MAIN_H

#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>

const char FANTOM_CHAR1 = '%';
const char FANTOM_CHAR2 = '$';

class TBoris {
  //---variables
public:
  std::shared_ptr<int> end;
private:
  std::string texts;
  int globID = -1;


  //-----------------------simply nodes---------------------
  struct TBorNode {
    //variables
    int begin;
    std::shared_ptr<int> last;
    bool leaf = true;
    std::shared_ptr<TBorNode> url;
    std::shared_ptr<TBorNode> prev;
    std::unordered_map<char, std::shared_ptr<TBorNode>> next;
    int id;

    //constructors
    //Конструктор. Первые 2 аргумента всегда должны быть одни и те же. Последние 2 - начало и общий конец.
    //По умолчанию все новые ноды создаются со ссылкой на root.
    //TODO id-шники создаются только в листе
    TBorNode(const std::shared_ptr<TBorNode> &root, int &globID, const int &_begin,
             const std::shared_ptr<int> &end) {
      id = globID++;
      last = end;
      begin = _begin;
      url = root;
    }

    //Конструктор с задаваемой в конце end-ом, как число. Нужен для вставки ноды не как лист.
    TBorNode(const std::shared_ptr<TBorNode> &root, int &globID, const int &_begin, const int &end) {
      id = globID++;
      last.reset(new int(end));
      begin = _begin;
      url = root;
      leaf = false;
    }

    TBorNode() = delete;

    //destructors
    ~TBorNode() = default;

    //functions
    //Ф нужна для вывода данных в ноде. Нужна больше для отладки
    void PrintNode(const std::string &text) {
      for (int i = begin; i <= *last; ++i) {
        std::cout << text[i];
      }
      std::cout << "|";
    }

    //Нужна для вставки следующего элемента. В аргументах пара,
    // в которой должно содержаться первая буква и указатель со вставляемой нодой.
    void Insert(const std::pair<char, std::shared_ptr<TBorNode>> &tmp_pair) {
      next.insert(tmp_pair);
      leaf = false;
      id = -1;
    }

    //Ф получает ноду по символу. Нужно, чтобы доставать нужную ноду на развилке.
    // Обычно используется в Ф Next
    std::shared_ptr<TBorNode> GetNodeElem(const char &c) {
      auto tmp = next.find(c);
      if (tmp == next.end()) {
        return nullptr;
      }
      return tmp->second;
    }

    void Copy(const std::shared_ptr<TBorNode> &cmp) {
      last = cmp->last;
      leaf = cmp->leaf;
      next = cmp->next;
      id = cmp->id;
    }

  };

protected:
  std::shared_ptr<TBorNode> root;


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
    end.reset(new int(0));
    root.reset(new TBorNode(root, globID, -1, end));
    root->url = root;
  };

  ~TBoris() = default;

  void SetText(const std::string &str1, const std::string &str2) {
    texts = str1 + FANTOM_CHAR2;// + FANTOM_CHAR1 + str2 ;
  }

//--------------------------------ITERATOR-------------------------------
  class TIterator {
  private:
    std::shared_ptr<TBorNode> prevNode;
    std::shared_ptr<TBorNode> activeNode;

  public:
    TIterator() {
      activeNode = nullptr;
      prevNode = nullptr;
    }

    TIterator(const std::shared_ptr<TBorNode> &node) {
      activeNode = node;
      prevNode = nullptr;
    }

    ~TIterator() = default;

    //Ф перехода на следующую ноду
    std::shared_ptr<TBorNode> Next(const char &c) {
      if (activeNode == nullptr) {
        return nullptr;
      }

      auto tmp = activeNode->GetNodeElem(c);
      if (tmp != nullptr) {
        return tmp;
      }
      return nullptr;
    }

    bool FindPath(const char &c) {
      if (activeNode == nullptr) {
        return false;
      }

      auto tmp = activeNode->GetNodeElem(c);
      if (tmp != nullptr) {
        return true;
      }
      return false;
    }

    //Ф перехода по суфф ссылке
    void GoThrowURL() {
      if (activeNode->url == nullptr) {
        throw std::logic_error("Trying to call nullptr in URL");
      }
      prevNode = activeNode;
      activeNode = activeNode->url;
    }

    void Insert(const std::pair<char, std::shared_ptr<TBorNode>> &tmp_pair) {
      activeNode->Insert(tmp_pair);
    }

    //Ф нужна для того, чтобы разделить ноду на 2. С _ идут аргументы обязательные для создания новой ноды.
    //Потом идут begin - индекс символа нового элемента, splitter - индекс символа сплитуемого элемента.
    //Стоит также отметить, что после сплита я остаюсь на той же ноде, что и был. Никакого перехода нет.
    void Split(const std::shared_ptr<TBorNode> &_root, int &_globID, const std::string &_texts,
               const std::shared_ptr<int> &_end,
               const int &begin, const unsigned &splitter) {

      //старый элемент
      auto oldNode = std::make_shared<TBorNode>(_root, _globID, splitter, _end);
      oldNode->Copy(activeNode);
      --_globID;

      activeNode->last.reset(new int(splitter - 1));
      activeNode->leaf = false;
      activeNode->id = -1;
      activeNode->next.clear();
      activeNode->Insert(std::pair<char, std::shared_ptr<TBorNode>>(_texts[splitter],
                                                                    oldNode));
      //новый элемент
      activeNode->Insert(std::pair<char, std::shared_ptr<TBorNode>>(_texts[begin],
                                                                    std::make_shared<TBorNode>(_root, _globID, begin,
                                                                                               _end)));
    }

    TIterator &operator=(const std::shared_ptr<TBorNode> &rhs) {
      if (rhs != nullptr) {
        activeNode = rhs;
      }
      return *this;
    }

    TIterator &operator=(const TIterator &rhs) {
      activeNode = rhs.activeNode;
      prevNode = rhs.prevNode;
      return *this;
    }

    //getters and setters
    std::shared_ptr<TBorNode> GetActiveNode() const {
      return activeNode;
    }

    std::shared_ptr<TBorNode> GetPrevNode() const {
      return prevNode;
    }

    char GetSymbol(const std::string &_texts, const int &edge, const int &activeLen) const {
      auto tmp = activeNode->GetNodeElem(_texts[edge]);
      if (tmp != nullptr) {
        return _texts[activeNode->GetNodeElem(_texts[edge])->begin + activeLen];
      }
      throw std::logic_error("Trying to call non exist char in GetSymbol");
    }

    //Геттер нужен для того, чтобы вернуть ноль, если prevNode пустой. Иначе вернуть сам этот prevNode.
    bool IsPrevNOTNull() {
      if (prevNode == nullptr) {
        return false;
      }
      return true;
    }

    void SetPrevNull() {
      prevNode = nullptr;
    }

    void SetPrev(const std::shared_ptr<TBorNode> &node) {
      prevNode = node;
    }

    //отладочная функция
    void PrintNode(const std::string &_texts) {
      activeNode->PrintNode(_texts);
      std::cout << std::endl;
    }

  };
//--------------------------END OF ITERATOR--------------------------

//getters
  std::shared_ptr<TBorNode> GetRoot() const {
    return root;
  }

  int &GetID() {
    return globID;
  }

  std::string GetTexts() const {
    return texts;
  }

  std::shared_ptr<int> GetEnd() const {
    return end;
  }


  void Build() {
    int remainder = 0;
    int activeLen = 0;
    int edge = 0;
    TIterator Node(root);
    bool splitFlag = true;

    for (*end = 0; *end < texts.size(); ++*end) {
      ++remainder;
      Node.SetPrevNull();
      while (remainder > 0) {

        std::shared_ptr<TBorNode> checkNode = (Node.Next(texts[edge]));
        while (checkNode != nullptr && activeLen > *checkNode->last - checkNode->begin) {
          activeLen -= *checkNode->last - checkNode->begin + 1;
          Node = checkNode;
          edge = *end - activeLen;
          checkNode = Node.Next(texts[edge]);
        }

        if (activeLen <= 0 && !Node.FindPath(texts[*end])) {
          splitFlag = true;
          --remainder;
          Node.Insert(std::pair<char, std::shared_ptr<TBorNode>>(texts[*end],
                                                                 std::make_shared<TBorNode>(root, globID, *end, end)));
          Node.GoThrowURL();
          Node.SetPrevNull();
          edge = -1;
        }

          //Сплитуем
        else if (Node.Next(texts[edge]) != nullptr &&
                 activeLen > 0 &&
                 texts[*end] != texts[Node.Next(texts[edge])->begin + activeLen]) {
          splitFlag = true;
          TIterator tmp(Node.Next(texts[edge]));
          tmp.Split(root, globID, texts, end, *end, Node.Next(texts[edge])->begin + activeLen);
          --remainder;

          if (Node.IsPrevNOTNull()) {
            Node.GetPrevNode()->url = tmp.GetActiveNode();
            Node.SetPrevNull();
          }

          if (Node.GetActiveNode() == root) {
            tmp.GoThrowURL();
            Node = tmp;
            while (texts[edge] == texts[edge + 1]) {
              ++edge;
            }
            ++edge;
            --activeLen;
          }
          else {
            Node.GoThrowURL();
            Node.SetPrev(tmp.GetActiveNode());
          }
        }

        else {
          if (splitFlag) {
            splitFlag = false;
            edge = *end;
          }
          ++activeLen;
          break;
        }
      }
    }
    --*end;

    TestPrinter();
  }

  void Test() {
    *end = texts.size() - 2;
    root->Insert(
            std::pair<char, std::shared_ptr<TBorNode>>(texts[0],
                                                       std::make_shared<TBorNode>(root, globID, 0, end)));

    TestPrinter();
  }


};
//TODO Поиск выполнятся через DFS.
//TODO При поиске нужно будет определить на какой позиции находится 1-ый сентинел, чтобы потом, дойдя до листа,
// можно было выяснить к какому слову относится данный суффикс.

#endif //SUFF_TREE_LAB5_MAIN_H


//aabaa