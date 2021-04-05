#ifndef SUFF_TREE_LAB5_MAIN_H
#define SUFF_TREE_LAB5_MAIN_H

//#define DEBUG

#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <set>

const char FANTOM_CHAR1 = '%';
const char FANTOM_CHAR2 = '$';

class TBoris {
  //---variables
public:
  std::shared_ptr<int> end;
private:
  std::string texts;
  int globID = -1;
  int one2two = -1;


  //-----------------------simply nodes---------------------
  struct TBorNode {
    //variables
    int begin;
    std::shared_ptr<int> last;
    bool leaf = true;
    std::shared_ptr<TBorNode> url;
    std::unordered_map<char, std::shared_ptr<TBorNode>> next;
    int id;
    bool red = false;
    bool blue = false;

    //constructors
    //Конструктор. Первые 2 аргумента всегда должны быть одни и те же. Последние 2 - начало и общий конец.
    //По умолчанию все новые ноды создаются со ссылкой на root.
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
    }

    //Нужна для вставки следующего элемента. В аргументах пара,
    // в которой должно содержаться первая буква и указатель со вставляемой нодой.
    void Insert(const std::pair<char, std::shared_ptr<TBorNode>> &tmp_pair) {
      next.insert(tmp_pair);
      leaf = false;
    }

    //Ф получает ноду по символу. Нужно, чтобы доставать нужную ноду на развилке.
    // Обычно используется в Ф Next
    std::shared_ptr<TBorNode> GetNodeElem(const char &c) {
      if (!next.empty()) {
        if (next.find(c) == next.end()) {
          return nullptr;
        }
        return next.find(c)->second;
      }
      return nullptr;
    }

    void Copy(const std::shared_ptr<TBorNode> &cmp) {
      last = cmp->last;
      leaf = cmp->leaf;
      next = cmp->next;
      id = cmp->id;
      url = cmp->url;
    }

  };

protected:
  std::shared_ptr<TBorNode> root;

  void Deleter(std::shared_ptr<TBorNode> &node) {
    if (!node->leaf) {
      for (auto &item : node->next) {
        Deleter(item.second);
      }
    }
    node->url = nullptr;
  }

//----functions
public:
  TBoris() {
    end = std::make_shared<int>(0);
    root = std::make_shared<TBorNode>(root, globID, -1, end);
    root->url = root;
  };

  ~TBoris() {
    Deleter(root);
  };

  void SetText(const std::string &str1, const std::string &str2) {
    texts = str1 + FANTOM_CHAR1 + str2 + FANTOM_CHAR2;
    one2two = str1.size();
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

    explicit TIterator(const std::shared_ptr<TBorNode> &node) {
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
    void Split(const std::shared_ptr<TBorNode> &_root, int &_globID, const std::string &_texts,
               const std::shared_ptr<int> &_end,
               const int &begin, const int &splitter, std::shared_ptr<TBorNode> Node) {
      Node->next.erase(_texts[activeNode->begin]);
      Node->Insert(std::pair<char, std::shared_ptr<TBorNode>>(_texts[activeNode->begin],
                                                              std::make_shared<TBorNode>(_root, _globID,
                                                                                         activeNode->begin,
                                                                                         splitter - 1)));
      std::shared_ptr<TBorNode> tmp = Node->GetNodeElem(_texts[activeNode->begin]);
      --_globID;
      //новый элемент
      tmp->Insert(std::pair<char, std::shared_ptr<TBorNode>>(_texts[begin],
                                                             std::make_shared<TBorNode>(_root, _globID, begin,
                                                                                        _end)));
      //Старый элемент
      activeNode->begin = splitter;
      tmp->Insert(std::pair<char, std::shared_ptr<TBorNode>>(_texts[activeNode->begin],
                                                             activeNode));
    }

    TIterator &operator=(const std::shared_ptr<TBorNode> &rhs) {
      if (rhs != nullptr) {
        activeNode = rhs;
      }
      return *this;
    }

    TIterator &operator=(const TIterator &rhs) {
      prevNode = rhs.prevNode;
      return *this;
    }

    //getters and setters
    std::shared_ptr<TBorNode> GetActiveNode() const {
      return activeNode;
    }

    int GetBegin() const {
      return activeNode->begin;
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
    bool IsPrevNotNull() {
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

    void ColorBlue() {
      activeNode->blue = true;
    }

    void ColorRed() {
      activeNode->red = true;
    }

    bool BothColored() {
      return activeNode->red && activeNode->blue;
    }

    bool GetRed() {
      return activeNode->red;
    }

    bool GetBlue() {
      return activeNode->blue;
    }

    std::string GetString(const std::string &_texts, const int &_begin, const int &_end) {
      std::string result;
      for (int i = _begin; i <= _end; ++i) {
        result += _texts[i];
      }
      return result;
    }

    //отладочная функция
#ifdef DEBUG
    [[maybe_unused]] void PrintNode(const std::string &_texts) {
      activeNode->PrintNode(_texts);
      std::cout << std::endl;
    }

    [[maybe_unused]] void PrintNode_1(const std::string &_texts) {
      activeNode->PrintNode(_texts);
    }
#endif

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
    int activeLen = 0;
    int edge = 0;
    TIterator Node(root);
    bool splitFlag = true;

#ifdef DEBUG
    int i;
#endif

    for (*end = 0; *end < texts.size(); ++*end) {
#ifdef DEBUG
      i = *end;
#endif
      Node.SetPrevNull();
      while (globID <= *end) {
        std::shared_ptr<TBorNode> checkNode = (Node.Next(texts[edge]));
        while (checkNode != nullptr && activeLen > *checkNode->last - checkNode->begin) {
          activeLen = activeLen - 1 - (*checkNode->last - checkNode->begin);
          Node = checkNode;
          edge = *end - activeLen;
          checkNode = Node.Next(texts[edge]);
        }

        if (Node.IsPrevNotNull() && activeLen == 0) {
          Node.GetPrevNode()->url = Node.GetActiveNode();
          Node.SetPrevNull();
        }

        if (activeLen <= 0 && !Node.FindPath(texts[*end])) {
          splitFlag = true;
//          --remainder;
          Node.Insert(std::pair<char, std::shared_ptr<TBorNode>>(texts[*end],
                                                                 std::make_shared<TBorNode>(root, globID, *end, end)));
          if (Node.IsPrevNotNull()) {
            Node.GetPrevNode()->url = Node.GetActiveNode();
            Node.SetPrevNull();
          }
          Node.GoThrowURL();
          Node.SetPrevNull();
          edge = 0;
        }

          //Сплитуем
        else if (Node.Next(texts[edge]) != nullptr &&
                 activeLen > 0 &&
                 texts[*end] != texts[Node.Next(texts[edge])->begin + activeLen]) {
          splitFlag = true;
          {
            TIterator tmp(Node.Next(texts[edge]));
            tmp.Split(root, globID, texts, end, *end, Node.Next(texts[edge])->begin + activeLen, Node.GetActiveNode());
          }
          TIterator tmp(Node.Next(texts[edge]));
          if (Node.IsPrevNotNull()) {
            Node.GetPrevNode()->url = tmp.GetActiveNode();
            Node.SetPrevNull();
          }
//          --remainder;
          if (Node.GetActiveNode() == root) {
            Node.SetPrev(tmp.GetActiveNode());
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
#ifdef DEBUG
          TIterator iter(root);
          PrintTree(iter, 0);
          std::cout << "-------------" << i << std::endl;
#endif
          break;
        }
#ifdef DEBUG
        TIterator iter(root);
        PrintTree(iter, 0);
        std::cout << "-------------" << i << std::endl;
#endif
      }
    }
    --*end;
  }

private:
  void ColorizeHelp(TIterator &node, std::set<std::string> &ans, int &ansCount, int len) {
    len = len + (*node.GetActiveNode()->last - node.GetActiveNode()->begin) + 1;
    if (node.GetActiveNode()->leaf) {
      if (node.GetActiveNode()->begin <= one2two) {
        node.ColorRed();
        return;
      }
      node.ColorBlue();
      return;
    }

    for (const auto &item : node.GetActiveNode()->next) {
      TIterator nextNode(item.second);
      ColorizeHelp(nextNode, ans, ansCount, len);

      if (nextNode.GetBlue()) {
        node.ColorBlue();
      }
      if (nextNode.GetRed()) {
        node.ColorRed();
      }
    }

    if (node.BothColored()) {
      if (len > ansCount) {
        ans.clear();
        ansCount = len;
        ans.insert(node.GetString(texts, *node.GetActiveNode()->last - len + 1, *node.GetActiveNode()->last));
      }
      else if (len == ansCount) {
        ans.insert(node.GetString(texts, *node.GetActiveNode()->last - len + 1, *node.GetActiveNode()->last));
      }
    }
  }

public:
  void Colorize() {
    std::set<std::string> ans;
    int ansCount = 0;
    TIterator node(root);
    for (const auto &item : node.GetActiveNode()->next) {
      TIterator nextNode(item.second);
      ColorizeHelp(nextNode, ans, ansCount, 0);
    }

    std::cout << ansCount << std::endl;
    if (!ans.empty()) {
      for (const auto &item : ans) {
        std::cout << item << std::endl;
      }
    }
  }

  void PrintTree(const TIterator &node, int count) {
    if (node.GetActiveNode()->leaf) {
      for (int i = 1; i < count; ++i) {
        std::cout << '-';
      }
      node.GetActiveNode()->PrintNode(texts);
      std::cout << std::endl;
      return;
    }

    if (node.GetActiveNode() != root) {
      for (int i = 1; i < count; ++i) {
        std::cout << '-';
      }
      node.GetActiveNode()->PrintNode(texts);
      std::cout << std::endl;
    }

    ++count;
    for (const auto &item : node.GetActiveNode()->next) {
      TIterator tmp(item.second);
      PrintTree(tmp, count);
    }
  }


};
#endif //SUFF_TREE_LAB5_MAIN_H