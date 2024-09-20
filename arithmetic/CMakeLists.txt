project(arithmetic)

add_executable(arithmetic polynomial.c)

if (UNIX)
    target_link_libraries(arithmetic PUBLIC m)
endif ()