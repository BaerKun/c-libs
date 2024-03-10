file(GLOB SOURCES "utils/*.c" "*.c" )

add_library(graph STATIC ${SOURCES})