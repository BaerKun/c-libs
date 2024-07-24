cmake_minimum_required(VERSION 3.27)

project(Algorithm)

include_directories(include)

file(GLOB SOURCES "*.c")

add_library(algorithm STATIC ${SOURCES})

add_executable(alg_test tests/test1.c)

target_link_libraries(alg_test PRIVATE algorithm)