#include <cstdio>
#include <iostream>

//-----------------string--------------------
#define MAX_KEY 300
#define size_t unsigned long long

namespace NMyString {
  class TString {
  private:
    char *buf = nullptr;
    unsigned int size = 0;

    static unsigned int StrLen(const char *str) {
      return (*str) ? StrLen(++str) + 1 : 0;
    }

    void CleanUp() {
      if (buf != nullptr) {
        delete[] buf;
      }
      size = 0;
    }

  public:
    TString() : buf(nullptr), size(0) {}

    TString(const char *buffer);

    TString &operator=(const char *value);

    ~TString() {
      CleanUp();
    }

    TString(const TString &obj);

    TString &operator=(TString &&obj) noexcept {
      CleanUp();

      size = obj.size;
      buf = obj.buf;
      obj = nullptr;

      return *this;
    }

    const char *GetBuf() const {
      return buf;
    }

    unsigned int GetSize() const {
      return size;
    }

    char *begin() const {
      if (buf != nullptr) {
        return &buf[0];
      }
      return nullptr;
    }

    char *end() const {
      if (buf != nullptr) {
        return &buf[size];
      }
      return nullptr;
    }

    char &operator[](const unsigned int &iterator) {
      if (iterator > size) {
        std::cout << "CAUTION: size was reached\niterator is " << iterator << std::endl;
        return buf[size - 1];
      }
      return buf[iterator];
    }

    friend std::istream &operator>>(std::istream &cin, NMyString::TString &obj) {
      char buffer[MAX_KEY];
      cin.getline(buffer, sizeof(buffer));
      obj.buf = buffer;
      obj.size = StrLen(buffer);

      return cin;
    }

    TString &operator+=(const char &rhs) {
      unsigned int sz = this->size + 1;
      char *ts = new char[sz + 1];
      for (int i = 0; i < this->size + 1; ++i) {
        ts[i] = this->buf[i];
      }
      ts[this->size + 1] = rhs;

      delete this->buf;
      this->buf = ts;

      return *this;
    }

    friend std::ostream &operator<<(std::ostream &cout, const NMyString::TString &obj) {
      cout << obj.GetBuf();
      return cout;
    }
  };
}
//----------------end of string-------------------

//----------------stirng cpp----------------------
NMyString::TString::TString(const char *buffer) {
  size = StrLen(buffer);
  buf = new char[size + 1];
  for (int i = 0; i < size; ++i) {
    buf[i] = buffer[i];
  }
  //strncpy_s(buf, size + 1, buffer, size);
}

NMyString::TString &NMyString::TString::operator=(const char *value) {
  size = StrLen(value);
  buf = new char[size + 1];
  for (int i = 0; i < size; ++i) {
    buf[i] = value[i];
  }
  //strncpy_s(buf, size + 1, value, size);

  return *this;
}

NMyString::TString::TString(const NMyString::TString &obj) {
  size = obj.size;
  buf = new char[size + 1];
  //strncpy_s(buf, size + 1, obj.buf, size);
}
//------------------end of string cpp-------------------------


//---------------TPatricia.h---------------------------
namespace NPatricia {

  size_t GetBitFromString(const NMyString::TString &tmpString);

  struct TData {
    unsigned long long key;
    unsigned long long digits;

    TData() : digits(0), key(0) {}

    TData(const NMyString::TString &str, const size_t &dig) : digits(dig) {
      key = GetBitFromString(str);
    }

    explicit TData(const NMyString::TString &str) : digits(0) {
      key = GetBitFromString(str);
    }

    friend std::ostream &operator<<(std::ostream &cout, const TData &outVal) {
      cout << outVal.digits;
      return cout;
    }

    bool operator!=(const TData &rhs) const {
      return this->key != rhs.key;
    }

    friend bool operator!=(const TData &lhs, const TData &rhs) {
      return lhs.key != rhs.key;
    }

    friend bool operator==(const TData &lhs, const TData &rhs) {
      return lhs.key == rhs.key;
    }

    friend bool operator<(const TData &lhs, const TData &rhs) {
      return lhs.key < rhs.key;
    }

    friend bool operator>(const TData &lhs, const TData &rhs) {
      return lhs.key > rhs.key;
    }
  };

  template<class T>
  struct TNode {
    T val;
    unsigned long long bit;
    TNode<T> *left;
    TNode<T> *right;

    TNode(const T &value, unsigned long long bites) : val(value), bit(bites), left(nullptr), right(nullptr) {}

    friend std::ostream &operator<<(std::ostream &cout, const TNode<T> &node) {
      cout << node.val;
      return cout;
    }

    ~TNode() = default;
  };

  template<class T>
  class TPatricia {
  private:
    TNode<T> *header;

    bool GetBit(const T &value, unsigned long long bit);

    unsigned long long FirstDiff(const T &lhs, const T &rhs) {
      unsigned long long i;
      for (i = 0; GetBit(lhs, i) == GetBit(rhs, i); ++i) {}
      return i;
    }

    TNode<T> *Find(const T &);

  public:

    void FinalFind(const T &value) {
      auto finder = Find(value);
      if (finder == nullptr) {
        std::cout << "NoSuchWord\n";
        return;
      }
      std::cout << "OK: " << *finder << '\n';
    }

    void Insert(const T &);

    void Erase(T &);

    TPatricia() : header(nullptr) {}

    ~TPatricia() = default;
  };

}
//---------------------end of TPatricia.h--------------------

//--------------------TPatricia.cpp------------------------
template<class T>
bool NPatricia::TPatricia<T>::GetBit(const T &value, unsigned long long bit) {
  return ((1u << (bit - 1)) & value);
}

template<>
bool NPatricia::TPatricia<NPatricia::TData>::GetBit(const TData &value, unsigned long long bit) {
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
void NPatricia::TPatricia<T>::Insert(const T &value) {
  if (header == nullptr) {
    header = new TNode<T>(value, 0);
    header->left = header;
    std::cout << "OK\n";
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
    std::cout << "Exist\n";
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
  NPatricia::TNode<T> *elem;
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
    elem = iter;
  }
  else {
    elem = nullptr;
  }

  if (elem == nullptr) {
    std::cout << "NoSuchWord\n";
    return;
  }

  //удаление header
  if (header->val == value && (header->left == nullptr || header->left->bit == header->bit)) {
    delete[] header;
    header = nullptr;
    std::cout << "OK\n";
    return;
  }

  if (elem == header) {
    header->val = prev->val;
    Erase(prev->val);
    return;
  }

  iter = header->left;
  prev = header;


  //-----------случай с ссылкой на себя--------------
  if ((elem->right != nullptr && elem->right->bit == elem->bit) ||
      (elem->left != nullptr && elem->left->bit == elem->bit)) {
    while (iter->bit > prev->bit && iter != elem) {
      prev = iter;
      if (GetBit(value, iter->bit)) {
        iter = iter->right;
      }
      else {
        iter = iter->left;
      }
    }

    if (iter->right != nullptr && iter->bit == iter->right->bit) {
      if (prev->right != nullptr && prev->right->val == iter->val) {
        prev->right = iter->left;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
      if (prev->left != nullptr && prev->left->val == iter->val) {
        prev->left = iter->left;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
    }

    else if (iter->left != nullptr && iter->bit == iter->left->bit) {
      if (prev->right != nullptr && prev->right->val == iter->val) {
        prev->right = iter->right;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
      if (prev->left != nullptr && prev->left->val == iter->val) {
        prev->left = iter->right;
        delete[] iter;
        std::cout << "OK\n";
        return;
      }
    }
    std::cout << "ERROR: something goes wrong" << std::endl;
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

  while (iter->bit > prev->bit && Q != iter) {
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

size_t NPatricia::GetBitFromString(const NMyString::TString &tmpString) {
  size_t result = 0;

  for (char i : tmpString) {
    if (i >= 'A' && i <= 'Z') {
      result = result * 26 + i - 'A' + 1;
    }
    else if (i >= 'a' && i <= 'z') {
      result = result * 26 + i - 'a' + 1;
    }
  }
  return result;
}
//----------------------end of TPatricia.cpp----------------------




int main() {
  char input[MAX_KEY];
  NPatricia::TPatricia<NPatricia::TData> patric;
  unsigned long long curVal;

  while (scanf("%s", input) > 0) {
    //input
    if (input[0] == '+') {
      scanf("%s%llu", input, &curVal);
      NMyString::TString inputStr = input;
      NPatricia::TData inData(inputStr, curVal);
      patric.Insert(inData);
    }

      //erase
    else if (input[0] == '-') {
      scanf("%s", input);
      NMyString::TString inputStr = input;
      NPatricia::TData inData(input);
      patric.Erase(inData);
    }

      //find
    else {
      NMyString::TString inputStr = input;
      NPatricia::TData inData(input);
      patric.FinalFind(inData);
    }
  }
  return 0;
}
