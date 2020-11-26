//
// Created by matve on 26.11.2020.
//

#ifndef DA_LABS_TSTRING_H
#define DA_LABS_TSTRING_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>


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

    char &operator[](const unsigned int &iterator) const {
      if (iterator > size) {
        std::cout << "CAUTION: size was reached\niterator is " << iterator << std::endl;
        return buf[size - 1];
      }
      return buf[iterator];
    }

    friend bool operator==(const NMyString::TString &lhs, const NMyString::TString rhs) {
      if (lhs.size == rhs.size) {
        for (int i = 0; i < lhs.size + 1; ++i) {
          if (lhs[i] != rhs[i]) {
            return false;
          }
          return true;
        }
      }
      return false;
    }

    friend bool operator!=(const NMyString::TString &lhs, const NMyString::TString &rhs) {
      return !(lhs == rhs);
    }


    /*friend std::istream &operator>>(std::istream &cin, NMyString::TString &obj) {
      char buffer[MAX_KEY];
      cin.getline(buffer, sizeof(buffer));
      obj.buf = buffer;
      obj.size = StrLen(buffer);

      return cin;
    }*/

    friend std::ostream &operator<<(std::ostream &cout, const NMyString::TString &obj) {
      for (auto i : obj) {
        cout << i;
      }
      return cout;
    }

    void LowerCase() {
      for (int i = 0; i < size + 1; ++i) {
        buf[i] = buf[i] >= 'A' && buf[i] <= 'Z' ? buf[i] - 'A' + 'a' : buf[i];
      }
    }

    void Initialise() {

    }
  };
}
//----------------end of string-------------------

#endif //DA_LABS_TSTRING_H
