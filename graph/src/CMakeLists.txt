file(GLOB SOURCES "utils/*.c")

add_library(utils SHARED ${SOURCES})

file(GLOB SOURCES "adjacency_list/*.c")

add_library(adjacency_list_graph STATIC ${SOURCES})

target_link_libraries(adjacency_list_graph PRIVATE utils)

file(GLOB SOURCES "matrix/*.c")

add_library(matrix_graph STATIC ${SOURCES})

file(GLOB SOURCES "only_edge/*.c")

add_library(only_edge_graph STATIC ${SOURCES})

target_link_libraries(only_edge_graph PRIVATE utils)