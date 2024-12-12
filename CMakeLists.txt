cmake_minimum_required(VERSION 3.28)

project(all4algorithm)

include_directories(utils)

add_subdirectory(utils)

add_subdirectory(tree)

add_subdirectory(graph)

add_subdirectory(arithmetic)

add_subdirectory(simple_algorithm)

add_subdirectory(some_useful)

add_subdirectory(geometry)

 add_executable(tmp tmp.c)