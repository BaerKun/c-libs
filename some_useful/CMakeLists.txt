add_executable(useful compress.c)

target_include_directories(useful PRIVATE ../tree/include)

target_link_libraries(useful PRIVATE tree)

