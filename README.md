# aoc2022
Advent Of Code 2022

aoc0 is a placeholder, currently a solution to Day 22 from 2021.

Each new day of 2022 will get it's own directory. Builds should be good on MacOS, Windows and Linux. Using Monterey, MSVC 2022 and Ubuntu 22.04.

For reference,

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/home/paul/vcpkg/scripts/buildsystems/vcpkg.cmake

(vcpkg path according to situation and whether anything like Boost is actually needed...)
