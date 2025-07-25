cmake_minimum_required(VERSION 2.15)
project(badlang)

set(SOURCES
	src/main.c
)

add_compile_options(-g)
add_executable(badl ${SOURCES})