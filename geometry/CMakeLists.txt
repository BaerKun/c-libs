cmake_minimum_required(VERSION 3.28)
project(geometry C)

if (WIN32)
    set(OpenCV_DIR "C:\\_myLibs\\opencv-4.10\\build")
endif ()

find_package(OpenCV REQUIRED core imgproc highgui)

add_library(graphical_debug STATIC src/graphical.cpp)
target_link_directories(graphical_debug PUBLIC ${OpenCV_DIRS})
target_include_directories(graphical_debug PRIVATE include)
target_link_libraries(graphical_debug PRIVATE ${OpenCV_LIBS})

file(GLOB SOURCES "src/*.c")

add_library(geometry_libs STATIC ${SOURCES})
target_include_directories(geometry_libs PUBLIC include)

add_executable(test_geometry tests/tests.c)

if(UNIX)
    # math.h
    target_link_libraries(test_geometry PRIVATE m)
endif ()
target_link_libraries(test_geometry PRIVATE geometry_libs)
target_link_libraries(test_geometry PRIVATE graphical_debug)