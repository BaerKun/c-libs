cmake_minimum_required(VERSION 3.27)
project(Graph)

set(CMAKE_C_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH /Users/bearkun/Desktop/C-codes/graph/bin)

set(LIBRARY_OUTPUT_PATH /Users/bearkun/Desktop/C-codes/graph/lib)

include_directories(include)

link_directories(lib)

add_subdirectory(src)

add_subdirectory(example)

add_executable(test tests/test.c)

target_link_libraries(test PRIVATE graph)

