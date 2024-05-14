cmake_minimum_required(VERSION 3.27)
project(Tree)

set(CMAKE_C_STANDARD 11)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(include)

add_subdirectory(src)

add_executable(Test tests/test1.c)

target_link_libraries(Test cursor)