cmake_minimum_required(VERSION 3.16)
project(DA-labs)

#set(CMAKE_CXX_FLAGS "-pedantic -Wall")
set(CMAKE_CXX_STANDARD 11)

add_executable(DA_lab1 "DA lab1/main.cpp")
add_executable(benchmark "DA lab2/benchmark.cpp" "DA lab2/patricia.cpp" "DA lab2/patricia.h")
add_executable(DA_lab2
        #     "DA lab2/TPatricia.cpp"
        #     "DA lab2/TPatricia.h"
        "DA lab2/main.cpp")
#add_executable(test_DA2 "DA lab2/string_test.cpp" "DA lab2/TString.cpp" "DA lab2/TString.h")