cmake_minimum_required(VERSION 3.28)
project(geometry C)

if (WIN32)
    set(OpenCV_DIR "C:\\_myLibs\\opencv-4.10\\build")
endif ()

find_package(OpenCV)

include_directories(include)

add_library(graphical_debug STATIC src/graphical.cpp)
target_link_directories(graphical_debug PRIVATE ${OpenCV_DIRS})
target_link_libraries(graphical_debug PRIVATE ${OpenCV_LIBS})

file(GLOB SOURCES "src/*.c")

add_library(geometry_libs STATIC ${SOURCES})

add_executable(geometry_exe main.c tests/tests.c)
# math.h
target_link_libraries(geometry_exe PRIVATE m)
target_link_libraries(geometry_exe PRIVATE geometry_libs)
target_link_libraries(geometry_exe PRIVATE graphical_debug)