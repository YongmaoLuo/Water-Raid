cmake_minimum_required(VERSION 3.21)
project(Water_Raid C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(software)

add_executable(Water_Raid
        software/airplane.h
        software/common_data_structure.h
        software/game_senario.h software/game_senario.cpp software/airplane.cpp software/main.cpp)
