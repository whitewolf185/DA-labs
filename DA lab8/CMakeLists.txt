cmake_minimum_required(VERSION 3.16)
project(greedy-lab8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")

add_executable(main main.cpp)
add_executable(bench benchmark.cpp)