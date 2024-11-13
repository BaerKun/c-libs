cmake_minimum_required(VERSION 3.27)

project(Algorithm)

include_directories(include)

file(GLOB SOURCES "*.c")

add_library(algorithm STATIC ${SOURCES})
target_include_directories(algorithm PRIVATE tree/include graph/include)
target_link_libraries(algorithm PRIVATE adjacency_list_graph tree)

add_executable(simple_alg_test tests/test_macro.c)

target_link_libraries(simple_alg_test algorithm)