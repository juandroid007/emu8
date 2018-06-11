#z80/libz80
add_library(emu8lib STATIC
	chip8.c
	cpu.c
	cpu.h
)

target_include_directories(emu8lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

TARGET_LINK_LIBRARIES(emu8lib ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
