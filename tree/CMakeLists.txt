project(Tree)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_subdirectory(src)

add_executable(tree_test test/test.c)

target_link_libraries(tree_test tree)