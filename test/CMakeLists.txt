add_executable(test main.c)
target_include_directories(test PRIVATE ../arithmetic/include)
target_link_libraries(test PRIVATE arithmetic)