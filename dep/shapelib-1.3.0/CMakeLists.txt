project(shapelib)

include_directories(contrib)

file(GLOB src "*.h" "*.c")
file(GLOB contrib "contrib/*.c" "contrib/*.h")
message(STATUS ${src})
message(STATUS ${contrib})

add_library(shapelib ${src} ${contrib})