#include <iostream>
#include "T_Vector.h"

int main() {
    N_Vector::T_Vector<int> a(5, 11);
    a.Push_back(45);

    std::cout << a.Get_size() << "   " << a[a.Get_size() - 1] << "  " << a[a.Get_size() - 2] << std::endl;

    return 0;
}
