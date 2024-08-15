cmake_minimum_required(VERSION 3.27)
project(Tree)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_subdirectory(src)

add_executable(test test/test.c)

target_link_libraries(test tree)