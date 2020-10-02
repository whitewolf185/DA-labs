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

        void Assign(const T elem){
            for (unsigned int i = 0; i < size; ++i){
                data[i] = elem;
            }
        }

    public:


        TVector() : size(0), capacity(1) {
            std::cout << "TVector was created" << std::endl;
        };

        TVector(const unsigned int n) : TVector() {
            size = n;
            capacity = n;
            data = nullptr;
        };

        TVector(const unsigned int n, T elem): TVector(){
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
                data = new T[1];
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
                std::cout << "CAUTION: size was reached" << std::endl;
                return data[size - 1];
            }
            return data[iterator];
        }

        ~TVector(){
            delete [] data;
        }
    };
}
#endif //DA_LAB1_T_VECTOR_H
