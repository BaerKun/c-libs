cmake_minimum_required(VERSION 3.28)

project(all4algorithm)

include_directories(tools)

add_subdirectory(tools)

add_subdirectory(tree)

add_subdirectory(graph)

add_subdirectory(simple_algorithm)

add_subdirectory(some_useful)

add_subdirectory(2d_process)

target_include_directories(algorithm PRIVATE tree/include graph/include)

target_link_libraries(algorithm PRIVATE adjacency_list_graph super_tree)
