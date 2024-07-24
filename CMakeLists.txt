cmake_minimum_required(VERSION 3.28)

project(all4algorithm)

add_subdirectory(tools)

add_subdirectory(tree)

add_subdirectory(graph)

add_subdirectory(simple_algorithm)

add_subdirectory(2d_process)

target_include_directories(algorithm PRIVATE tools tree/include graph/include)

target_include_directories(discrete_alg PRIVATE tools)

target_link_libraries(algorithm PRIVATE adjacency_list_graph super_tree)
