cmake_minimum_required(VERSION 3.27)
project(Graph)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_subdirectory(src)

add_subdirectory(example)

add_executable(graph_tests tests/test.c)

target_link_libraries(graph_tests PRIVATE only_edge_graph)