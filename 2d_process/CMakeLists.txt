cmake_minimum_required(VERSION 3.28)
project(2d_process C)

if(WIN32)
    set(OpenCV_DIR "C:\\_myLibs\\opencv-4.10\\build")
endif()

find_package(OpenCV)

include_directories(include)

add_library(graphical_debug STATIC src/graphical_debug.cpp)
target_link_directories(graphical_debug PUBLIC ${OpenCV_DIRS})
target_link_libraries(graphical_debug PRIVATE ${OpenCV_LIBS})

add_library(other STATIC src/convex_hull.c)

add_executable(discrete_alg main.c src/tools.c tests/tests.c)
# math.h
target_link_libraries(discrete_alg PRIVATE m)
target_link_libraries(discrete_alg PRIVATE graphical_debug)
target_link_libraries(discrete_alg PRIVATE other)