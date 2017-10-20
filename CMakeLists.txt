cmake_minimum_required(VERSION 2.8)
project(ProjetConcurrent)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp Cell.cpp Cell.h lol.cpp)
add_executable(ProjetConcurrent ${SOURCE_FILES})