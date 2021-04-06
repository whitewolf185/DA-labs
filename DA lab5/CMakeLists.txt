cmake_minimum_required(VERSION 3.16)
project(suff_tree-lab5)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-g")

add_executable(start BORis.h main.cpp)
add_executable(itog test.cpp)