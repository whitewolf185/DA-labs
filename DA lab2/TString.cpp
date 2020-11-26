#include "TString.h"

//----------------stirng cpp----------------------
NMyString::TString::TString(const char *buffer) {
  if (buffer == nullptr) {
    size = 0;
    buf = nullptr;
    return;
  }
  size = StrLen(buffer);
  buf = new char[size + 1];
  for (int i = 0; i < size; ++i) {
    buf[i] = buffer[i];
  }
  LowerCase();
}

NMyString::TString &NMyString::TString::operator=(const char *value) {
  if (value == nullptr) {
    size = 0;
    buf = nullptr;
    return *this;
  }

  size = StrLen(value);
  buf = new char[size + 1];
  if (size) {
    strcpy(buf, value);
  }
  LowerCase();
  return *this;
}


NMyString::TString::TString(const NMyString::TString &obj) {
  size = obj.size;
  buf = new char[size + 1];
}
//------------------end of string cpp-------------------------