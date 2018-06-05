#z80/libz80
add_library(emu8lib STATIC
	chip8.c
)

target_include_directories(emu8lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


