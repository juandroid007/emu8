#z80/
cmake_minimum_required(VERSION 2.8)

project(emu8)

INCLUDE(FindPkgConfig)

add_definitions( "-m64 -O3 -fomit-frame-pointer" )

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

add_subdirectory(src)
add_subdirectory(lib)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
