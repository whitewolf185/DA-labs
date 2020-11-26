#include <iostream>
#include <fstream>
#include <cstring>

//подсчет количества символов в строке
static unsigned int StrLen(const char *str) {
  return (*str) ? StrLen(++str) + 1 : 0;
}

// проверка на равенство строк
static inline bool Equal(char *a, char *b) {
  if (a == nullptr || b == nullptr) {
    return false;
  }
  return (strcmp(a, b) == 0);
}

// получить bit-ый по счету бит слева
static inline int GetBit(const char *key, int bit) {
  if (bit < 0) {
    bit = 0;
  }
  return (key[bit / 8u] >> (7u - (bit % 8u))) & 1u;
}

static inline int FirstDifBit(char *a, char *b) {
  if (a == nullptr || b == nullptr) {
    return 0;
  }

  int i = 0;

  while (a[i] == b[i]) {
    i++;
  }
  i *= 8;

  while (GetBit(a, i) == GetBit(b, i)) {
    i++;
  }

  return i;
}

static inline void LowerCase(char *str) {
  for (int i = 0; i < StrLen(str); ++i) {
    str[i] = str[i] >= 'A' && str[i] <= 'Z' ? str[i] - 'A' + 'a' : str[i];
  }
}

//----------------NODE-----------------
const int MAX_LEN = 257;
//typedef unsigned long long TValue;
//typedef char TKey;

namespace NPatricia {
  template<class T>
  class TNode {
  private:
    int id = -1;
    int bit{};

    T *key;
    unsigned long long value{};
  public:
    TNode *left;
    TNode *right;

    //------тут будут геттеры и сеттеры--------

    int &GetRvalID() {
      return id;
    }

    int GetID() {
      return id;
    }

    void SetID(int iDiter) {
      id = iDiter;
    }

    unsigned long long &GetRvalVal() {
      return value;
    }

    unsigned long long GetVal() {
      return value;
    }

    void SetVal(unsigned long long valuer) {
      value = valuer;
    }

    T *GetKey() {
      return key;
    }

    void SetKey(T *keyer) {
      key = keyer;
    }

    int &GetRvalBit() {
      return bit;
    }

    int GetBit() {
      return bit;
    }

    void SetBit(int bitter) {
      bit = bitter;
    }

    // --------конец геттеров и сеттеров---------

    void Initialize(int b, T *k, unsigned long long v, TNode *l, TNode *r) {
      bit = b;
      if (k) {
        key = new char[StrLen(k) + 1];
        strcpy(key, k);
      }
      else {
        key = k;
      }
      value = v;
      left = l;
      right = r;
    }

    TNode() {
      Initialize(-1, nullptr, 0, this, this);
    }

    TNode(int b, T *k, unsigned long long v) {
      Initialize(b, k, v, this, this);
    }

    TNode(int b, T *k, unsigned long long v, TNode *l, TNode *r) {
      Initialize(b, k, v, l, r);
    }

    ~TNode() {
      delete[] key;
    }
  };
}
//--------------end of Node-------------


//-----------Patricia-------------
namespace NPatricia {
  template<class T>
  struct TPatricia {
    TNode<T> *root;
    int size;

    TPatricia() {
      root = new TNode<T>();
      size = 0;
    }

    void CleanUp(TNode<T> *node) {
      if (node->left->GetBit() > node->GetBit()) {
        CleanUp(node->left);
      }
      if (node->right->GetBit() > node->GetBit()) {
        CleanUp(node->right);
      }
      delete node;
    }

    ~TPatricia() {
      CleanUp(root);
    }

    TNode<T> *Find(char *key) {
      TNode<T> *p = root;
      TNode<T> *q = root->left;

      while (p->GetBit() < q->GetBit()) {
        p = q;
        q = (GetBit(key, q->GetBit()) ? q->right : q->left);
      }

      if (!Equal(key, q->GetKey())) {
        return 0;
      }

      return q;
    }

    TNode<T> *Insert(char *key, unsigned long long value) {
      TNode<T> *p = root;
      TNode<T> *q = root->left;
      while (p->GetBit() < q->GetBit()) {
        p = q;
        q = (GetBit(key, q->GetBit()) ? q->right : q->left);
      }

      if (Equal(key, q->GetKey())) {
        return 0;
      }

      int lBitPos = FirstDifBit(key, q->GetKey());

      p = root;
      TNode<T> *x = root->left;

      while (p->GetBit() < x->GetBit() && x->GetBit() < lBitPos) {
        p = x;
        x = (GetBit(key, x->GetBit()) ? x->right : x->left);
      }

      try {
        q = new TNode<T>();
      }
      catch (const std::bad_alloc &e) {
        std::cout << "ERROR: fail to allocate the requested storage space\n";
        return 0;
      }

      q->Initialize(lBitPos, key, value,
                    (GetBit(key, lBitPos) ? x : q),
                    (GetBit(key, lBitPos) ? q : x));

      if (GetBit(key, p->GetBit())) {
        p->right = q;
      }
      else {
        p->left = q;
      }

      size++;
      return q;
    }

    void KVCopy(TNode<T> *src, TNode<T> *dest) {
      if (StrLen(dest->GetKey()) < StrLen(src->GetKey())) {
        delete[] dest->GetKey();
        dest->SetKey(new char[StrLen(src->GetKey()) + 1]);
      }
      strcpy(dest->GetKey(), src->GetKey());

      dest->SetVal(src->GetVal());
    }

    bool Delete(char *k) {
      // прадед удаляемого узла pp, родитель p и сам сын t (сына предстоит удалить)
      TNode<T>
              *p,
              *t,
              *pp = nullptr;

      p = root;
      t = (p->left);

      // найдем pp, p и t
      while (p->GetBit() < t->GetBit()) {
        pp = p;
        p = t;
        t = (GetBit(k, t->GetBit()) ? t->right : t->left);
      }

      // если ключа искомого-то и нет -- выходим
      if (!Equal(k, t->GetKey())) {
        return false;
      }

      TNode<T> *x, *r;
      char *key;

      // если p == t, то у t есть селфпоинтер. в таком случае достаточно лишь
      // переподвесить к родителю (pp) "реальный" указатель t, который не селфпоинтер
      if (p != t) {
        // иначе же, кладем ключ и знач. p в t, чтобы далее удалять именно p, а не t
        KVCopy(p, t);

        key = p->GetKey();
        r = p;
        x = (GetBit(key, p->GetBit()) ? p->right : p->left);

        // ищем того, кто на p бекпоинтерит (будет лежать в r; а х, по сути, будет в точности равняться p)
        while (r->GetBit() < x->GetBit()) {
          r = x;
          x = (GetBit(key, x->GetBit()) ? x->right : x->left);
        }

        // и вместо бекпоинтера на p, будем бекпоинтерить на t
        if (GetBit(key, r->GetBit())) {
          r->right = t;
        }
        else {
          r->left = t;
        }
      }

      // остается подвесить к родителю p (pp) "реальный" указатель p, который не селфпоинтер
      TNode<T> *ch = (GetBit(k, p->GetBit()) ? p->left : p->right);
      if (GetBit(k, pp->GetBit())) {
        pp->right = ch;
      }
      else {
        pp->left = ch;
      }

      delete p;

      size--;

      return true;
    }

    void Save(std::ofstream &file);


    void Enumerate(TNode<T> *node, TNode<T> **nodes, int &index) {
      // важно, что index передается по ссылке: айди узлов не будут повторяться
      node->SetID(index);
      nodes[index] = node;
      ++index;
      if (node->left->GetBit() > node->GetBit()) {
        Enumerate(node->left, nodes, index);
      }
      if (node->right->GetBit() > node->GetBit()) {
        Enumerate(node->right, nodes, index);
      }
    }

    void Load(std::ifstream &file);
  };
}
//---------end of patricia-----------

//--------------------------Save------------------------------
template<>
void NPatricia::TPatricia<char>::Save(std::ofstream &file) {
  // подаем размер дерева
  file.write((const char *) &(size), sizeof(int));

  // пронумеровка узлов, инициализация массива указателей
  int index = 0;
  TNode<char> **nodes;
  try {
    nodes = new TNode<char> *[size + 1];
  }
  catch (const std::bad_alloc &e) {
    std::cout << "ERROR: fail to allocate the requested storage space\n";
    return;
  }
  Enumerate(root, nodes, index);

  // теперь просто последовательно (как при обходе в Enumerate)
  // подаем всю инфу об узлах, но вместо указателей left/right подаем
  // айди узлов (каковы они были при обходе в Enumerate) left/right
  TNode<char> *node;
  for (int i = 0; i < (size + 1); ++i) {
    node = nodes[i];
    file.write((const char *) &(node->GetRvalVal()), sizeof(unsigned long long));
    file.write((const char *) &(node->GetRvalBit()), sizeof(int));
    int len = node->GetKey() ? StrLen(node->GetKey()) : 0;
    file.write((const char *) &(len), sizeof(int));
    file.write(node->GetKey(), sizeof(char) * len);
    file.write((const char *) &(node->left->GetRvalID()), sizeof(int));
    file.write((const char *) &(node->right->GetRvalID()), sizeof(int));
  }
  delete[] nodes;
}
//----------------------------end of Save------------------------------


//--------------------------------Load------------------------------------
template<>
void NPatricia::TPatricia<char>::Load(std::ifstream &file) {
  // считываем размер
  int n;
  file.read((char *) &n, sizeof(int));
  size = n;
  // если он нуль - выходим
  if (!size) {
    return;
  }

  TNode<char> **nodes = new TNode<char> *[size + 1];
  // рут уже инициализировался, когда мы пишем создали new Trie()
  // незачем этого делать повторно
  nodes[0] = root;
  for (int i = 1; i < (size + 1); ++i) {
    // а вот прочие узлы надо инитнуть
    nodes[i] = new TNode<char>();
  }

  // поля узлов, которые нам предстоит считывать
  int bit;
  int len;
  char *key = 0;
  unsigned long long value;
  int idLeft, idRight;

  for (int i = 0; i < (size + 1); ++i) {
    file.read((char *) &(value), sizeof(unsigned long long));
    file.read((char *) &(bit), sizeof(int));
    file.read((char *) &(len), sizeof(int));
    if (len) {
      key = new char[len + 1];
      key[len] = 0;
    }
    file.read(key, len);
    // поскольку считываем в том же порядке, что и писали в Load-e
    // айди узлов-сыновей будут сохранять свой порядок, и дерево соберется таким же
    file.read((char *) &(idLeft), sizeof(int));
    file.read((char *) &(idRight), sizeof(int));
    nodes[i]->Initialize(bit, key, value, nodes[idLeft], nodes[idRight]);
    delete[] key;
  }

  delete[] nodes;
}
//------------------------end of Load-------------------------------------



int main() {
  // оптимизация
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  // потоки i/o -- файлы для сериализации/десериализации, хранения/считывания конструкции патриции
  std::ofstream fout;
  std::ifstream fin;

  char input[MAX_LEN];
  unsigned long long value;

  // основное дерево patric
  NPatricia::TPatricia<char> *patric;
  try {
    patric = new NPatricia::TPatricia<char>();
  }
  catch (const std::bad_alloc &e) {
    std::cout << "ERROR: fail to allocate the requested storage space\n";
    exit(0);
  }

  NPatricia::TNode<char> *node;

  while ((std::cin >> input)) {
    if (!std::strcmp(input, "+")) {
      std::cin >> input;
      LowerCase(input);
      std::cin >> value;

      std::cout << (patric->Insert(input, value) ? "OK" : "Exist");
      std::cout << '\n';
    }
    else if (!std::strcmp(input, "-")) {
      std::cin >> input;
      LowerCase(input);

      std::cout << (patric->Delete(input) ? "OK" : "NoSuchWord");
      std::cout << '\n';
    }
    else if (!std::strcmp(input, "!")) {
      std::cin >> input;
      if (!std::strcmp(input, "Save")) {
        std::cin >> input;
        fout.open(input, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!fout.is_open()) {
          std::cout << "ERROR: can't create file\n";
          continue;
        }

        patric->Save(fout);
        std::cout << "OK\n";

        fout.close();
      }
      else if (!std::strcmp(input, "Load")) {
        std::cin >> input;
        fin.open(input, std::ios::in | std::ios::binary);
        if (!fin.is_open()) {
          std::cout << "ERROR: can't open file\n";
          continue;
        }

        delete patric;
        patric = new NPatricia::TPatricia<char>();
        patric->Load(fin);

        std::cout << "OK\n";

        fin.close();
      }
    }

    else {
      LowerCase(input);
      node = patric->Find(input);
      if (!node) {
        std::cout << "NoSuchWord";
      }
      else {
        std::cout << "OK: " << node->GetVal();
      }
      std::cout << '\n';
    }
  }

  delete patric;

  return 0;
}