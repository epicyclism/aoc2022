cmake_minimum_required (VERSION 3.21)

add_executable(aoc4 aoc4.cpp)
target_include_directories(aoc4 PUBLIC ../common)
target_link_libraries(aoc4 PRIVATE ctre::ctre)

add_executable(aoc4_c aoc4.c)