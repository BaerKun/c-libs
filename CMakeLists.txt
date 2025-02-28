cmake_minimum_required(VERSION 3.6)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(c_libs)

include_directories(utils)
add_subdirectory(tree)
add_subdirectory(graph)
add_subdirectory(arithmetic)
add_subdirectory(simple_algorithm)
add_subdirectory(geometry)

# test
add_subdirectory(test)