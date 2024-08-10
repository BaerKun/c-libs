cmake_minimum_required(VERSION 3.27)

project(Algorithm)

include_directories(include)

file(GLOB SOURCES "*.c")

add_library(algorithm STATIC ${SOURCES})

add_executable(tests tests/test_macro.c)