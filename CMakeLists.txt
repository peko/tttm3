cmake_minimum_required(VERSION 3.6)
project(tttm3)

# DEPENDENCIES

# GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

#GLU
pkg_search_module(GLU REQUIRED glu)

# NUKLEAR GUI
set(NUK dep/nuklear)

# SHAPE LIB
set(SHP dep/shapelib-1.3.0)

# library search path
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig")
set(ENV{LD_LIBRARY_PATH} "ENV{LD_LIBRARY_PATH}:/usr/local/lib")

# build dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#set(CMAKE_BINARY_DIR bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

# Include dirs
include_directories(
        src
        dep/
        dep/glad
        dep/klib
        ${GLFW_INCLUDE_DIRS}
        ${GLU_INCLUDE_DIRS}
        ${NUK} ${NUK}/demo/glfw_opengl2
        $(shp))

set(SOURCE_FILES
        src/main.c
        src/types.c
        src/grid.c
        src/gui.c
        src/shape.c
        src/text.c
        src/wire.c
        src/vbo.c)

add_executable(tttm3 ${SOURCE_FILES})

add_subdirectory(dep/shapelib-1.3.0)
add_subdirectory(dep/glad)
target_link_libraries(
        tttm3
        glad
        ${GLFW_LIBRARIES}
        ${GLU_LIBRARIES}
        shapelib proj m)

