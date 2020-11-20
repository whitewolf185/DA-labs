#ifndef DA_LABS_TSTRING_H
#define DA_LABS_TSTRING_H

#include <iostream>

#define MAX_KEY 2048

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
        return &buf[size + 1];
      }
      return nullptr;
    }

    char &operator[](const unsigned int &iterator) {
      if (iterator > size) {
        std::cout << "CAUTION: size was reached\niterator is " << iterator << std::endl;
        return buf[size];
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


#endif //DA_LABS_TSTRING_H
