#include "TVector.h"
#include<string>

//#define DEBUG
#define TEST_ENTER

const short MAXKEY = 10;

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
    NVector::TVector<int> count(MAXKEY);
    NVector::TVector<TPair> result(size);
    for (int i = 0; i < 6; ++i) {
        count.Assign(0);
        for (int j = 0; j < size; ++j) {
            count[data[j].key[5-i]]++;
        }

        for (int j = 1; j < MAXKEY; ++j) {
            count[j] = count[j] + count[j - 1];
        }

        for(int j = size - 1; j >= 0; --j){
            result[count[data[j].key[5-i]] - 1] = data[j];
            count[data[j].key[5-i]]--;
        }

        for (int j = 0; j < size; ++j) {
            data[j] = result[j];
        }

    }

}


#ifdef TEST_ENTER

void EnterKey(const TPair &data){
    for (int i = 0; i < 6; ++i) {
        std::cout << data.key[i];
    }
}

#endif // TEST_ENTER


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

    CountingSort(values, values.GetSize());
    freopen("out.txt", "w", stdout);
    for (int i = 0; i < values.GetSize(); ++i) {
        /*printf("%d%d%d%d%d%d %s", values[i].key[0], values[i].key[0], values[i].key[0], values[i].key[0],
               values[i].key[0], values[i].key[0], values[i].val)*/

        EnterKey(values[i]);
        std::cout << '\t' << values[i].val << '\n';
    }
    fclose(stdout);
    return 0;
}
