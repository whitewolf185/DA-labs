#ifndef DA_LAB1_T_VECTOR_H
#define DA_LAB1_T_VECTOR_H

#include <iostream>
#include <stdio.h>
namespace NVector {


    template<class T>
    class TVector {
    private:
        unsigned int size;
        unsigned int capacity;
        T *data;



    public:

        void Assign(const T elem){
            for (unsigned int i = 0; i < size; ++i){
                data[i] = elem;
            }
        }

        TVector() : size(0), capacity(1) {
            //std::cout << "TVector was created" << std::endl;
            //data = NULL;
        };

        TVector(const unsigned int n) /*: TVector()*/ {
            size = n;
            capacity = n;
            data = new T[capacity];
        };

        TVector(const unsigned int n, T elem)/*: TVector()*/{
            size = n;
            capacity = n;
            data = new T[capacity];
            Assign(elem);
        }

        unsigned int GetSize(){
            return size;
        }

        void PushBack(const T &elem){
            if(capacity == 1){
                capacity *= 2;
                data = new T[capacity];
                data[size] = elem;
                size++;
            }

            else if(capacity <= size){
                capacity *= 2;
                T* newData = new T[capacity];
                for (int i = 0; i < size; ++i) {
                    newData[i] = data[i];
                }
                delete[] data;
                data = newData;
                data[size] = elem;
                size++;
            }

            else{
                data[size] = elem;
                size++;
            }
        }

        bool Empty(){
            return !size;
        }

        T &operator[](const unsigned int &iterator){
            if(iterator >= size) {
                std::cout << "CAUTION: size was reached\niterator is " << iterator << std::endl;
                return data[size - 1];
            }
            return data[iterator];
        }

        ~TVector(){
            if(!this->Empty()) {
                delete[] data;
            }
        }
    };
}
#endif //DA_LAB1_T_VECTOR_H
