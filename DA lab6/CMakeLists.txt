cmake_minimum_required(VERSION 3.16)
project(long_alg-lab6)

set(CMAKE_CXX_STANDARD 11)

add_executable(start_lab6 "itog.cpp")
add_executable(bench_gmp bench_gmp.cpp)
target_link_libraries(bench_gmp gmp)
add_executable(bench_my main.cpp)