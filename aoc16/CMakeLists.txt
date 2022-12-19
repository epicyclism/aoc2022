cmake_minimum_required (VERSION 3.21)

add_executable(aoc16 aoc16.cpp)
target_include_directories(aoc16 PUBLIC ../common)
target_link_libraries(aoc16 PRIVATE ctre::ctre)

add_executable(aoc16o aoc16o.cpp)
target_include_directories(aoc16o PUBLIC ../common)
target_link_libraries(aoc16o PRIVATE ctre::ctre)
