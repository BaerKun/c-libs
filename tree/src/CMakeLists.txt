file(GLOB SOURCES "utils/*.c" "super_tree/*.c" )

add_library(super_tree STATIC ${SOURCES})