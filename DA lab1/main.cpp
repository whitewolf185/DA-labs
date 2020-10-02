#include "T_Vector.h"
#include<string>

struct T_Pair{
    short key[6];
    std::string val;
};

void Convert_string_to_key (T_Pair &tmp, const std::string &str){
    for (int i = 0; i < 6; ++i) {
        tmp.key[i] = str[i] - '0';
    }
}

int main() {
    N_Vector::T_Vector<T_Pair> values;


    std::string str_key, str_val;
    while(scanf("%s %s", &str_key, &str_val)){
        T_Pair initial_val;
        Convert_string_to_key(initial_val, str_key);
        initial_val.val = str_val;

        values.Push_back(initial_val);
    }

    for (int i = 0; i < values.Get_size(); ++i) {
        for (auto j : values[i].key){
            std::cout << j;
        }
        std::cout << std::endl;
    }

    return 0;
}
