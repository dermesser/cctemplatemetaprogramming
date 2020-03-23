project("CTMP")
cmake_minimum_required(VERSION 3.16)

add_compile_options(-std=c++17)

add_executable(example src/example.cc)

add_executable(binary src/binary.cc)
