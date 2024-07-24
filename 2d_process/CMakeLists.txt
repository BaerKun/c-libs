cmake_minimum_required(VERSION 3.28)
project(2d_process C)

add_executable(discrete_alg main.c)

# math.h
target_link_libraries(discrete_alg PUBLIC m)