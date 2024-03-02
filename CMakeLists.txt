cmake_minimum_required(VERSION 3.27)
project(Project C)
project(Text C)

set(CMAKE_C_STANDARD 11)

add_executable(Project
        main.c
        tools/Queue.c
        tools/AonGraph.c
)

add_executable(Text
        Test.c)