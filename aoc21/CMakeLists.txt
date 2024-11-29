cmake_minimum_required (VERSION 3.21)

add_executable(aoc21 aoc21.cpp)
target_include_directories(aoc21 PUBLIC ../common)
target_link_libraries(aoc21 PRIVATE ctre::ctre)

add_executable(aoc21a aoc21a.cpp)
target_include_directories(aoc21a PUBLIC ../common)
target_link_libraries(aoc21a PRIVATE ctre::ctre)

add_executable(aoc21c aoc21c.cpp)
target_include_directories(aoc21c PUBLIC ../common)
target_link_libraries(aoc21c PRIVATE ctre::ctre)