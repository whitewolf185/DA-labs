#ifndef DA_LABS_TSTRING_H
#define DA_LABS_TSTRING_H

#include <string.h>
#include <iostream>

namespace NMyString {
  class TString {
  private:
    char *buf = nullptr;
    unsigned int size = 0;

    unsigned int StrLen(const char *str) {
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

    TString(const char *buffer) {
      size = StrLen(buffer);
      buf = new char[size + 1];
      strncpy_s(buf, size + 1, buffer, size);
    }

    TString &operator=(const char *value) {
      size = StrLen(value);
      buf = new char[size + 1];
      strncpy_s(buf, size + 1, value, size);

      return *this;
    }

    ~TString() {
      CleanUp();
    }

    TString(const TString &obj) {
      size = obj.size;
      buf = new char[size + 1];
      strncpy_s(buf, size + 1, obj.buf, size);
    }

    TString &operator=(TString &&obj) {
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

    char *begin() {
      if (buf != nullptr) {
        return &buf[0];
      }
      return nullptr;
    }

    char *end() {
      if (buf != nullptr) {
        return &buf[size + 1];
      }
      return nullptr;
    }
  };
}

std::ostream &operator<<(std::ostream &cout, const NMyString::TString &obj) {
  cout << obj.GetBuf();
  return cout;
}

#endif //DA_LABS_TSTRING_H
