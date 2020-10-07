#include "TVector.h"
#include <vector>
#include <ctime>
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


bool operator< (TPair lhs, TPair rhs){
    return lhs.key < rhs.key;
}

int main(){
    NVector::TVector<TPair> values;
    std::vector<TPair> stl_values;

    

    return 0;
}
