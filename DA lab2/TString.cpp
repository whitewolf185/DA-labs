#include "TString.h"

//----------------stirng cpp----------------------
NMyString::TString::TString(const char *buffer) {
  size = StrLen(buffer);
  buf = new char[size + 1];
  for (int i = 0; i < size; ++i) {
    buf[i] = buffer[i];
  }
  LowerCase();
}

NMyString::TString &NMyString::TString::operator=(const char *value) {
  size = StrLen(value);
  buf = new char[size + 1];
  if (size) {
    strcpy(buf, value);
  }

  return *this;
}


NMyString::TString::TString(const NMyString::TString &obj) {
  size = obj.size;
  buf = new char[size + 1];
}
//------------------end of string cpp-------------------------