project(Tree)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_library(tree STATIC ${SOURCES})

add_executable(test_tree test/test.c)

target_link_libraries(test_tree tree)