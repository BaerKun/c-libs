cmake_minimum_required(VERSION 3.27)
project(OpenGL)

set(CMAKE_C_STANDARD 11)

include_directories(glad/include)

include_directories(glfw/include)

add_subdirectory(glad)

add_subdirectory(glfw)

add_executable(OpenGL main.c)

target_link_libraries(OpenGL glad)
target_link_libraries(OpenGL glfw)
