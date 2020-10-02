#include "TVector.h"
#include<string>

#define DEBUG

struct TPair{
    short key[6]{};
    std::string val;
};

void ConvertStringToKey (TPair &tmp, const std::string &str){
    for (int i = 0; i < 6; ++i) {
        tmp.key[i] = str[i] - '0';
    }
}


void CountingSort(NVector::TVector<TPair> & data, const unsigned int & size){
    NVector::TVector<int> count(9);
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < size; ++j) {
            count[data[j].key[i]];
        }
    }

}


int main() {
    NVector::TVector<TPair> values;


    std::string strKey, strVal;
    while(std::cin >> strKey >> strVal){
        TPair initialVal;
        ConvertStringToKey(initialVal, strKey);
        initialVal.val = strVal;

        values.PushBack(initialVal);
    }
#ifdef DEBUG
    for (int i = 0; i < values.GetSize(); ++i) {
        for (auto j : values[i].key){
            std::cout << j;
        }
        std::cout << std::endl;
    }
#endif // DEBUG

    return 0;
}
