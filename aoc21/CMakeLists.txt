cmake_minimum_required (VERSION 3.21)

add_executable(aoc21 aoc21.cpp)
target_include_directories(aoc21 PUBLIC ../common)
target_link_libraries(aoc21 PRIVATE ctre::ctre)

add_executable(aoc21a aoc21a.cpp)
target_include_directories(aoc21a PUBLIC ../common)
target_link_libraries(aoc21a PRIVATE ctre::ctre)

add_executable(aoc21b aoc21b.cpp)
target_include_directories(aoc21b PUBLIC ../common)
target_link_libraries(aoc21b PRIVATE ctre::ctre)