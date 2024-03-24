cmake_minimum_required(VERSION 3.27)
project(Graph)

set(CMAKE_C_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH /Users/bearkun/Desktop/C-codes/graph/bin)

set(LIBRARY_OUTPUT_PATH /Users/bearkun/Desktop/C-codes/graph/lib)

include_directories(include)

link_directories(lib)

add_subdirectory(src)

add_subdirectory(example)

add_executable(test1 tests/test.c)

add_executable(test2 tests/test2.c)

add_executable(test3 tests/test3.c)

target_link_libraries(test1 PRIVATE adjacency_list_graph)

target_link_libraries(test2 PRIVATE adjacency_list_graph)

target_link_libraries(test3 PRIVATE matrix_graph)


