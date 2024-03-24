file(GLOB SOURCES "utils/*.c" "cursor/*.c" )

add_library(cursor SHARED ${SOURCES})