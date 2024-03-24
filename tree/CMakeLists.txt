cmake_minimum_required(VERSION 3.27)
project(Tree)

set(CMAKE_C_STANDARD 11)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(include)

add_subdirectory(src)

add_executable(Test1 tests/test1.c)

add_executable(Test2 tests/test2.c)

add_executable(Test3 tests/test3.c)

target_link_libraries(Test1 cursor)

target_link_libraries(Test2 cursor)

target_link_libraries(Test3 cursor)