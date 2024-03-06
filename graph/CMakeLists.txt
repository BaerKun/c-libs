cmake_minimum_required(VERSION 3.27)
project(Graph C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES "src/*.c" "src/utils/*.c")

add_library(_graph STATIC ${SOURCES})

target_include_directories(_graph PRIVATE include)

add_executable(Test tests/test.c)

target_include_directories(Test PRIVATE include)

target_link_libraries(Test PRIVATE _graph)
