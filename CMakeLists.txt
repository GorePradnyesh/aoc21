cmake_minimum_required(VERSION 3.20)

project(AOC21)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


add_executable(aoc main.cpp)

add_subdirectory(P8)