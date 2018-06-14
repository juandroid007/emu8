#z80/libz80
add_library(emu8lib STATIC
	chip8.c
	cpu.c
	cpu.h
	sound.c
	sound.h
)

target_include_directories(emu8lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

TARGET_LINK_LIBRARIES(emu8lib m ${SDL2_LIBRARIES})
