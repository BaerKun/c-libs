cmake_minimum_required(VERSION 3.27)
project(Project C)
project(Text C)

set(CMAKE_C_STANDARD 11)

add_executable(Project
        calculate24.c)

add_executable(Text main.c
        tools/Graph.c
        tools/Queue.c)