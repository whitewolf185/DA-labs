//#define TRY_BOR

#ifdef TRY_BOR
#include "BORis.h"
#endif

#ifndef TRY_BOR

#include <iostream>
#include <memory>

#endif

int main() {
#ifdef TRY_BOR
  TBoris a;
  try{
    std::string str1 = "hello";
    std::string str2 = "hi";
    a.SetText(str1,str2);
    a.Test();
    TBoris::TIterator iter(a.GetRoot());

    iter.Next('h');
    iter.Split(a.GetRoot(), a.GetID(),a.GetTexts(),a.GetEnd(),
               a.GetTexts().size() - 1,3);

    iter.PrintNode(a.GetTexts());
    iter.Next(FANTOM_CHAR2);
    iter.PrintNode(a.GetTexts());
  }
  catch (const std::invalid_argument& e){
    std::cout << e.what();
  }
  catch (const std::logic_error& e){
    std::cout << e.what();
  }
#endif

#ifndef TRY_BOR
  int *tmp = new int(1);
  std::unique_ptr<int> a(new int(2345));
  std::unique_ptr<int> b = std::move(a);
  a.reset(tmp);
  ++*a;

  std::cout << "a = " << *a << std::endl;
  std::cout << "b = " << *b << std::endl;
#endif

  return 0;
}