#include "TVector.h"

//#define DEBUG
//#define TEST_ENTER

const int LEN = 2049;

struct TPair{
    int key;
    char val[LEN];

    TPair(){
        key = 0;
    }

    TPair(int n, char elem[LEN]){
        strcpy(val,elem);
        key = n;
    }

    ~TPair(){

    }
};

void CountingSort(NVector::TVector<TPair> & data, const unsigned int & maxNum){
    NVector::TVector<int> count(maxNum + 1);
    NVector::TVector<TPair> result(data.GetSize());

        count.Assign(0);
        for (int j = 0; j < data.GetSize(); ++j) {
            count[data[j].key]++;
        }

        for (int j = 1; j <= maxNum; ++j) {
            count[j] = count[j] + count[j - 1];
        }

        for(int j = data.GetSize() - 1; j >= 0; --j){
            result[count[data[j].key] - 1] = data[j];
            count[data[j].key]--;
        }

        for (int j = 0; j < data.GetSize(); ++j) {
            data[j] = result[j];
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

    int key;
    char val[LEN];
    int maxNum = -1;
    while(scanf("%d %s", &key, val) > 0){
        TPair initialVal;
        initialVal.key = key;
        strcpy(initialVal.val,val);
        values.PushBack(initialVal);

        maxNum = std::max(maxNum, key);
    }


#ifdef DEBUG
    for (int i = 0; i < values.GetSize(); ++i) {
        for (auto j : values[i].key){
            std::cout << j;
        }
        std::cout << std::endl;
    }
#endif // DEBUG

    if(!values.Empty()){
        CountingSort(values, maxNum);
        for (int i = 0; i < values.GetSize(); ++i) {
            if(values[i].key == 0){
                printf("00000");
            }

            else{
                for (int j = 0; j < 6 - 1 - log10(values[i].key); ++j) {
                    printf("0");
                }
            }
            printf("%d %s\n", values[i].key, values[i].val);
        }
    }
    return 0;
}
