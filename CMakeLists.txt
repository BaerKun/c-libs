cmake_minimum_required(VERSION 3.6)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(c_libs)

include_directories(utils)
add_subdirectory(utils)
add_subdirectory(tree)
add_subdirectory(graph)
add_subdirectory(arithmetic)
add_subdirectory(simple_algorithm)
add_subdirectory(some_useful)
add_subdirectory(geometry)

#add_executable(tmp tmp.c)
#target_link_libraries(tmp PRIVATE m)