#include "TString.h"

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
