cmake_minimum_required (VERSION 3.21)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(aoc13 aoc13.cpp)
target_include_directories(aoc13 PUBLIC ../common)
target_link_libraries(aoc13 PRIVATE nlohmann_json::nlohmann_json)
