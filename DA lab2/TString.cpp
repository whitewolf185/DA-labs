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


NMyString::TString::TString(const NMyString::TString &obj) {
  size = obj.size;
  buf = new char[size + 1];
  *buf = *obj.buf;
  }
//------------------end of string cpp-------------------------