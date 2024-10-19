project(Arithmetic)

file(GLOB SOURCES "src/*.c" )

add_library(arithmetic ${SOURCES})

target_include_directories(arithmetic PRIVATE include)

if (UNIX)
    target_link_libraries(arithmetic PUBLIC m)
endif ()

add_executable(test_arithmetic test.c)

target_link_libraries(test_arithmetic PRIVATE arithmetic)

target_include_directories(test_arithmetic PRIVATE include)