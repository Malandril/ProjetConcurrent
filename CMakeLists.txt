cmake_minimum_required(VERSION 2.8)
project(ProjetConcurrent)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp Cell.cpp Cell.h)
add_executable(ProjetConcurrent ${SOURCE_FILES})
target_link_libraries(ProjetConcurrent pthread)
