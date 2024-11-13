cmake_minimum_required(VERSION 3.28)
project(GGB C)

if (WIN32)
    set(OpenCV_DIR "C:\\_myLibs\\opencv-4.10\\build")
endif ()

find_package(OpenCV REQUIRED core imgproc highgui)

add_library(graphical STATIC src/graphical.cpp)
target_link_directories(graphical PUBLIC ${OpenCV_DIRS})
target_include_directories(graphical PRIVATE include)
target_link_libraries(graphical PRIVATE ${OpenCV_LIBS})

file(GLOB SOURCES "src/*.c")

add_library(ggb_core STATIC ${SOURCES})
target_include_directories(ggb_core PUBLIC include)

add_executable(ggb main.c)

if(UNIX)
    # math.h
    target_link_libraries(ggb PRIVATE m)
endif ()

target_link_libraries(ggb PRIVATE ggb_core)
target_link_libraries(ggb PRIVATE graphical)