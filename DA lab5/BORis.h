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
    std::shared_ptr<TBorNode> url;
    std::shared_ptr<TBorNode> prev;
    std::unordered_map<char, std::shared_ptr<TBorNode>> next;
    unsigned id;

    //constructors
    //�����������. ������ 2 ��������� ������ ������ ���� ���� � �� ��. ��������� 2 - ������ � ����� �����.
    //�� ��������� ��� ����� ���� ��������� �� ������� �� root.
    TBorNode(const std::shared_ptr<TBorNode> &root, unsigned &globID, const unsigned &_begin,
             const std::shared_ptr<unsigned> &end) {
      id = globID++;
      last = end;
      begin = _begin;
      url = root;
    }

    //����������� � ���������� � ����� end-��, ��� �����. ����� ��� ������� ���� �� ��� ����.
    TBorNode(const std::shared_ptr<TBorNode> &root, unsigned &globID, const unsigned &_begin, const unsigned &end) {
      id = globID++;
      last.reset(new unsigned(end));
      begin = _begin;
      url = root;
      leaf = false;
    }

    TBorNode() = delete;

    //destructors
    ~TBorNode() = default;

    //functions
    //� ����� ��� ������ ������ � ����. ����� ������ ��� �������
    void PrintNode(const std::string &text) {
      for (int i = begin; i <= *last; ++i) {
        (text[i] == FANTOM_CHAR1 || text[i] == FANTOM_CHAR2) ? std::cout << " " : std::cout << text[i];
      }
      std::cout << "|";
    }

    //����� ��� ������� ���������� ��������. � ���������� ����,
    // � ������� ������ ����������� ������ ����� � ��������� �� ����������� �����.
    void Insert(const std::pair<char, std::shared_ptr<TBorNode>> &tmp_pair) {
      next.insert(tmp_pair);
      leaf = false;
    }

    //� �������� ���� �� �������. �����, ����� ��������� ������ ���� �� ��������.
    // ������ ������������ � � Next
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
    end.reset(new unsigned int(0));
    root.reset(new TBorNode(root, globID, 0, end));
  };

  ~TBoris() = default;

  void SetText(const std::string &str1, const std::string &str2) {
    texts = str1 + FANTOM_CHAR1 + str2 + FANTOM_CHAR2;
  }

//--------------------------------ITERATOR-------------------------------
  class TIterator {
  private:
    int reminder = 0;
    int activeLen = 0;
    std::shared_ptr<TBorNode> prevNode;
    std::shared_ptr<TBorNode> activeNode;

  public:
    TIterator() = delete;

    TIterator(std::shared_ptr<TBorNode> node) {
      activeNode = node;
      prevNode = nullptr;
    }

    ~TIterator() = default;

    //� �������� �� ��������� ����
    void Next(const char &c) {
      if (activeNode == nullptr) {
        throw std::logic_error("Trying to call nullptr in Next");
      }

      auto tmp = activeNode->GetNodeElem(c);
      if (tmp != nullptr) {
        activeNode = tmp;
        return;
      }
      throw std::invalid_argument("Cannot find elem in Next");
    }

    //� �������� �� ���� ������
    void GoThrowURL() {
      if (activeNode->url == nullptr) {
        throw std::logic_error("Trying to call nullptr in URL");
      }

      //TODO �������� ��� ������, ��������� � ���, ��� prevNode = activNode->url
      prevNode = activeNode;
      activeNode = activeNode->url;
    }

    //� ����� ��� ����, ����� ��������� ���� �� 2. � _ ���� ��������� ������������ ��� �������� ����� ����.
    //����� ���� begin - ������ ������� ������ ��������, splitter - ������ ������� ����������� ��������.
    //����� ����� ��������, ��� ����� ������ � ������� �� ��� �� ����, ��� � ���. �������� �������� ���.
    void Split(const std::shared_ptr<TBorNode> &_root, unsigned int &_globID, const std::string &_texts,
               const std::shared_ptr<unsigned> &_end,
               const unsigned int &begin, const unsigned &splitter) {

      //TODO �������� ��������� ������ � ����������� �� � �����. ����� �������� ������ ��� �����������.
      //������ �������
      auto oldNode = std::make_shared<TBorNode>(_root, _globID, splitter + 1, _end);
      oldNode->Copy(activeNode);
      activeNode->last.reset(new unsigned(splitter));
      activeNode->leaf = false;
      activeNode->Insert(std::pair<char, std::shared_ptr<TBorNode>>(_texts[splitter + 1],
                                                                    oldNode));
      //����� �������
      activeNode->Insert(std::pair<char, std::shared_ptr<TBorNode>>(_texts[begin],
                                                                    std::make_shared<TBorNode>(_root, _globID, begin,
                                                                                               _end)));
    }

    //���������� �������
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

  unsigned int &GetID() {
    return globID;
  }

  std::string GetTexts() const {
    return texts;
  }

  std::shared_ptr<unsigned> GetEnd() const {
    return end;
  }


  //� ����� ��� ����������������� end
  void IncEnd() {
    ++*end;
  }

  void Test() {
    *end = texts.size() - 2;
    root->Insert(
            std::pair<char, std::shared_ptr<TBorNode>>(texts[0], std::make_shared<TBorNode>(root, globID, 0, end)));

    TestPrinter();
  }


};


#endif //SUFF_TREE_LAB5_MAIN_H
