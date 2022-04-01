cmake_minimum_required(VERSION 3.21)
project(Water_Raid CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(software)

add_executable(Water_Raid
        software/game_senario.cpp software/main.cpp)
