#z80/src
add_executable(emu8
	main.c
)

target_link_libraries(emu8 emu8lib)

install(TARGETS emu8 DESTINATION bin)
