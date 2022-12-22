cmake_minimum_required (VERSION 3.21)

add_executable(aoc22_2018 aoc22.cpp)
target_include_directories(aoc22_2018 PUBLIC ../../common)
target_link_libraries(aoc22_2018 PRIVATE ctre::ctre)