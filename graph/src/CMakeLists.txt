file(GLOB SOURCES "utils/*.c")

add_library(utils SHARED ${SOURCES})

file(GLOB SOURCES "adjacency_list/*.c")

add_library(adjacency_list_graph SHARED ${SOURCES})

target_link_libraries(adjacency_list_graph PRIVATE utils)

file(GLOB SOURCES "matrix/*.c")

add_library(matrix_graph SHARED ${SOURCES})

#target_link_libraries(matrix_graph PRIVATE utils)