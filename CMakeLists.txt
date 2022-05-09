cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
project(Water_Raid CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(.)
include_directories(GameLogic)
include_directories(VideoDriver)

add_executable(Water_Raid
        GameLogic/main.cpp GameLogic/driver.cpp GameLogic/game_scenario.cpp
        GameLogic/sprite.cpp GameLogic/enemy_plane.cpp GameLogic/fuel_tank.cpp GameLogic/battleship.cpp GameLogic/bullet.cpp)
