#include <cstdio>
#include <iostream>
#include <fstream>

//-----------------string--------------------
#define MAX_KEY 257
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
      delete[] buf;
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

    TString &operator=(const TString &obj) noexcept {
      CleanUp();

      size = obj.size;
      buf = new char[StrLen(obj.buf)];
      buf = obj.buf;

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

    friend std::ostream &operator<<(std::ostream &cout, const NMyString::TString &obj) {
      for (auto i : obj) {
        cout << i;
      }
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
}

NMyString::TString &NMyString::TString::operator=(const char *value) {
  size = StrLen(value);
  buf = new char[size + 1];
  for (int i = 0; i < size; ++i) {
    buf[i] = value[i];
  }

  return *this;
}

NMyString::TString::TString(const NMyString::TString &obj) {
  size = obj.size;
  buf = new char[size + 1];
}
//------------------end of string cpp-------------------------


//---------------------TPatricia.h---------------------------
namespace NPatricia {

  size_t GetBitFromString(const NMyString::TString &tmpString);

  struct TData {
    unsigned long long key;
    unsigned long long digits;
    //NMyString::TString Str;

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
    int id = -1;

    TNode() : bit(0), left(this), right(this), val(0) {}

    TNode(const T &value, unsigned long long bites) : val(value), bit(bites), left(this), right(this) {}

    void Initialize(const T value, unsigned long long bites, TNode<T> *L, TNode<T> *R) {
      val = value;
      bit = bites;
      left = L;
      if (right != nullptr) {
        right = R;
      }
    }

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
    unsigned int size = 0;

    bool GetBit(const T &value, unsigned long long bit);

    unsigned long long FirstDiff(const T &lhs, const T &rhs) {
      unsigned long long i;
      for (i = 1; GetBit(lhs, i) == GetBit(rhs, i); ++i) {}
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

    //void PrintDefinitions(TNode<TData> *node, std::ofstream &out);

    //void PrintRelations(TNode<TData> *node, std::ofstream &out);

    void Save(std::ofstream &file);

    void Load(std::ifstream &file);

    void Enumerate(TNode<T> *node, TNode<T> **nodes, int &index) {
      // важно, что index передается по ссылке: айди узлов не будут повторяться
      node->id = index;
      nodes[index] = node;
      ++index;
      if (node->left->bit > node->bit) {
        Enumerate(node->left, nodes, index);
      }
      if (node->right != nullptr && node->right->bit > node->bit) {
        Enumerate(node->right, nodes, index);
      }
    }

    TPatricia() : header(nullptr) {}

    ~TPatricia() {
      if (header != nullptr) {
        CleanUp(header);
      }
    }

    void CleanUp(TNode<T> *node) {
      if (node->left->bit > node->bit) {
        CleanUp(node->left);
      }
      if (node->right != nullptr && node->right->bit > node->bit) {
        CleanUp(node->right);
      }
      delete node;
    }
  };

}
//---------------------end of TPatricia.h--------------------

//--------------------TPatricia.cpp------------------------

template<>
void NPatricia::TPatricia<NPatricia::TData>::Save(std::ofstream &file) {
  {

    // подаем размер дерева
    file.write((const char *) &(size), sizeof(int));
    //если нечего сохранять - не сохраняем
    if (!size) {
      return;
    }
    // пронумеровка узлов, инициализация массива указателей
    int index = 0;
    TNode<TData> **nodes;
    try {
      nodes = new TNode<TData> *[size + 1];
    }
    catch (const std::bad_alloc &e) {
      std::cout << "ERROR: fail to allocate the requested storage space\n";
      return;
    }
    Enumerate(header, nodes, index);

    // теперь просто последовательно (как при обходе в enumerate)
    // подаем всю инфу об узлах, но вместо указателей left/right подаем
    // айди узлов (каковы они были при обходе в enumerate) left/right
    TNode<TData> *node;
    for (int i = 0; i < (size); ++i) {
      node = nodes[i];
      file.write((const char *) &(node->val), sizeof(TData));
      file.write((const char *) &(node->bit), sizeof(unsigned long long));
      //file.write((const char *) &(node->val.key), sizeof(unsigned long long));
      file.write((const char *) &(node->left->id), sizeof(int));
      if (node->right != nullptr) {
        file.write((const char *) &(node->right->id), sizeof(int));
      }
      else {
        int tmp = -2;
        file.write((const char *) &(tmp), sizeof(int));
      }
    }
    delete[] nodes;
  }
}

template<>
void NPatricia::TPatricia<NPatricia::TData>::Load(std::ifstream &file) {
  // считываем размер
  int n;
  file.read((char *) &n, sizeof(unsigned int));
  size = n;
  // если он нуль - выходим
  if (!size) {
    return;
  }

  TNode<TData> **nodes = new TNode<TData> *[size + 1];
  // рут уже инициализировался, когда мы пишем создали new TPatricia()
  // незачем этого делать повторно
  for (int i = 0; i < (size); ++i) {
    // а вот прочие узлы надо инитнуть
    nodes[i] = new TNode<TData>(NPatricia::TData(), 0);
  }
  header = nodes[0];
  header->right = nullptr;

  // поля узлов, которые нам предстоит считывать
  int bit;
  TData value;
  int idLeft, idRight;

  for (int i = 0; i < (size); ++i) {
    file.read((char *) &(value), sizeof(TData));
    file.read((char *) &(bit), sizeof(unsigned long long));
    // поскольку считываем в том же порядке, что и писали в Load-e
    // айди узлов-сыновей будут сохранять свой порядок, и дерево соберется таким же
    file.read((char *) &(idLeft), sizeof(int));
    file.read((char *) &(idRight), sizeof(int));
    if (idRight == -2) {
      nodes[i]->Initialize(value, bit, nodes[idLeft], nullptr);
    }
    else {
      nodes[i]->Initialize(value, bit, nodes[idLeft], nodes[idRight]);
    }
  }

  delete[] nodes;
}

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
    ++size;
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
  ++size;
}


template<class T>
void NPatricia::TPatricia<T>::Erase(T &value) {

  //пустая патриция
  if (header == nullptr) {
    printf("NoSuchWord\n");
    return;
  }


  auto pred = header;
  auto cur = header->left;


  while (cur->bit > pred->bit) {
    pred = cur;
    if (GetBit(value, cur->bit)) {
      cur = cur->right;
    }
    else {
      cur = cur->left;
    }
  }

  //в патриции нет удаляемого файла
  if (cur->val != value) {
    printf("NoSuchWord\n");
    return;
  }


  //если удалить надо header
  if (pred == header) {
    delete[] cur;
    header = nullptr;
    printf("OK\n");
    if (size) {
      --size;
    }
    return;
  }

  //флаг на то удаляем мы header или нет
  int h = 0;
  if (cur == header) {
    h = 1;
    cur->val = pred->val;
    cur = pred;
  }

  //случай с ссылкой на себя
  if (((cur->left == cur) && (pred == cur)) || ((cur->right == cur) && (pred == cur))) {
    pred = header;
    while ((pred->left != cur) && (pred->right != cur)) {
      if (GetBit(value, pred->bit)) {
        pred = pred->right;
      }
      else {
        pred = pred->left;
      }
    }
  }

  if (cur->left == cur) {
    if (pred->right == cur) {
      pred->right = cur->right;
      delete[] cur;
      printf("OK\n");
      if (size) {
        --size;
      }
      return;
    }
    else {
      pred->left = cur->right;
      delete[] cur;
      printf("OK\n");
      if (size) {
        --size;
      }
      return;
    }
  }

  if (cur->right == cur) {
    if (pred->right == cur) {
      pred->right = cur->left;
      delete[] cur;
      printf("OK\n");
      if (size) {
        --size;
      }
      return;
    }
    else {
      pred->left = cur->left;
      delete[] cur;
      printf("OK\n");
      if (size) {
        --size;
      }
      return;
    }
  }


  //удаляем не header
  if (h == 1) {

    auto pred_head = header;

    //указание на cur сверху
    while ((pred_head->left != cur) && (pred_head->right != cur)) {

      if (GetBit(cur->val, pred_head->bit)) {
        pred_head = pred_head->right;
      }
      else {
        pred_head = pred_head->left;
      }
    }

    pred = cur;

    //обратная ссылка на cur
    while ((pred->left != cur) && (pred->right != cur)) {
      if (GetBit(cur->val, pred->bit)) {
        pred = pred->right;
      }
      else {
        pred = pred->left;
      }
    }


    if (cur->right == header) {
      if (pred_head->right == cur) {
        pred_head->right = cur->left;
      }
      else {
        pred_head->left = cur->left;
      }

      if (pred->left == cur) {
        pred->left = cur->right;
      }
      else {
        pred->right = cur->right;
      }
    }
    else {
      if (pred_head->right == cur) {
        pred_head->right = cur->right;
      }
      else {
        pred_head->left = cur->right;
      }

      if (pred->left == cur) {
        pred->left = cur->left;
      }
      else {
        pred->right = cur->left;
      }
    }

    delete[] cur;
    printf("OK\n");
    if (size) {
      --size;
    }
    return;

  }

  pred = cur;
  auto pred_pred = pred;

  while ((pred->left != cur) && (pred->right != cur)) {
    pred_pred = pred;
    if (GetBit(cur->val, pred->bit)) {
      pred = pred->right;
    }
    else {
      pred = pred->left;
    }
  }

  auto newpred = pred;

  while ((newpred->left != pred) && (newpred->right != pred)) {
    if (GetBit(pred->val, newpred->bit)) {
      newpred = newpred->right;
    }
    else {
      newpred = newpred->left;
    }
  }

  if (pred->left == cur) {
    if (newpred->left == pred) {
      newpred->left = cur;
    }
    else {
      newpred->right = cur;
    }
    if (pred_pred->right == pred) {
      pred_pred->right = pred->right;
    }
    else {
      pred_pred->left = pred->right;
    }
  }

  else {
    if (newpred->left == pred) {
      newpred->left = cur;
    }
    else {
      newpred->right = cur;
    }
    if (pred_pred->right == pred) {
      pred_pred->right = pred->left;
    }
    else {
      pred_pred->left = pred->left;
    }
  }

  cur->val = pred->val;

  delete[] pred;

  printf("OK\n");
  if (size) {
    --size;
  }
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
  NPatricia::TPatricia<NPatricia::TData> *patric;
  unsigned long long curVal;

  try {
    patric = new NPatricia::TPatricia<NPatricia::TData>();
  }
  catch (std::bad_alloc &e) {
    std::cout << "ERROR: fail to allocate the requested storage space\n";
    std::terminate();
  }

  std::ofstream fout;
  std::ifstream fin;

  while (scanf("%s", input) > 0) {
    //input
    if (input[0] == '+') {
      scanf("%s%llu", input, &curVal);
      NMyString::TString inputStr = input;
      NPatricia::TData inData(inputStr, curVal);
      patric->Insert(inData);
    }

      //erase
    else if (input[0] == '-') {
      scanf("%s", input);
      NMyString::TString inputStr = input;
      NPatricia::TData inData(input);
      patric->Erase(inData);
      //patric.Print(inputStr);
    }

      //save and load
    else if (input[0] == '!') {
      scanf("%s", input);

      //load
      if (input[0] == 'L') {
        scanf("%s", input);
        NMyString::TString inputStr = input;
        fin.open(input, std::ios::in | std::ios::binary);
        if (!fin.is_open()) {
          std::cout << "ERROR: can't open file\n";
          continue;
        }

        delete patric;
        patric = new NPatricia::TPatricia<NPatricia::TData>();
        patric->Load(fin);

        std::cout << "OK\n";

        fin.close();
      }

        //save
      else {
        scanf("%s", input);
        fout.open(input, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!fout.is_open()) {
          std::cout << "ERROR: can't create file\n";
          continue;
        }

        patric->Save(fout);
        std::cout << "OK\n";

        fout.close();
      }
    }

      //find
    else {
      NMyString::TString inputStr = input;
      NPatricia::TData inData(input);
      patric->FinalFind(inData);
    }
  }
  //delete patric;
  return 0;
}
