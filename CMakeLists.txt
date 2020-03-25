project("CTMP")
cmake_minimum_required(VERSION 3.16)
add_compile_options(-std=c++17)

add_executable(example src/example.cc)

add_executable(binary src/binary.cc)

add_executable(replace_type src/replace_type.cc)

add_executable(fac src/fac.cc)
