cmake_minimum_required(VERSION 3.27)

project(Algprithm)

#include_directories(utils)

file(GLOB SOURCES "utils/*.c")

add_library(utilities STATIC ${SOURCES})

file(GLOB SOURCES "*.c")

add_library(algprithm STATIC ${SOURCES})

find_library(TREE cursor PATHS lib)

target_link_libraries(algprithm PRIVATE utilities)

target_link_libraries(algprithm PRIVATE ${TREE})

add_executable(Test1 tests/test1.c)

target_link_libraries(Test1 PRIVATE algprithm)