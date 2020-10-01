#ifndef DA_LAB1_T_VECTOR_H
#define DA_LAB1_T_VECTOR_H

#include <iostream>
#include <stdio.h>
namespace N_Vector {


    template<class T>
    class T_Vector {
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


        T_Vector() : size(0), capacity(1) {
            std::cout << "T_Vector was created" << std::endl;
        };

        T_Vector(const unsigned int n) : T_Vector() {
            size = n;
            capacity = n;
        };

        T_Vector(const unsigned int n, T elem): T_Vector(){
            size = n;
            capacity = n;
            Assign(elem);
        }

        unsigned int Get_size(){
            return size;
        }

        void Push_back(const T elem){
            if(capacity <= size){
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

        ~T_Vector(){
            delete [] data;
        }
    };
}
#endif //DA_LAB1_T_VECTOR_H
