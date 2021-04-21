cmake_minimum_required(VERSION 3.16)
project(DP-lab7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")

add_executable(lab7 main.cpp)
add_executable(bench benchmark.cpp)