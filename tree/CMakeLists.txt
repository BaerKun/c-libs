project(Tree)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.c" )

add_library(tree STATIC ${SOURCES})

add_executable(tree_test test/test.c)

target_link_libraries(tree_test tree)