cmake_minimum_required(VERSION 3.27)
project(Graph)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_subdirectory(src)

add_subdirectory(example)