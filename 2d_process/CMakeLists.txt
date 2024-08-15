cmake_minimum_required(VERSION 3.28)
project(2d_process C)

find_package(OpenCV)

add_library(graphical_debug STATIC graphical_debug.cpp)

target_link_directories(graphical_debug PUBLIC ${OpenCV_DIRS})

target_link_libraries(graphical_debug PRIVATE ${OpenCV_LIBS})

add_executable(discrete_alg main.c)

# math.h
target_link_libraries(discrete_alg PRIVATE m)

target_link_libraries(discrete_alg PRIVATE graphical_debug)