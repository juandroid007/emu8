#z80/
cmake_minimum_required(VERSION 2.8)

project(emu8)

add_definitions( "-g -lSDL -lSDL_mixer" )

add_subdirectory(src)
add_subdirectory(lib)
