//
// Created by Yongmao Luo on 4/8/22.
//

// upper level function call
// Copyright (C) 2022  Yongmao Luo

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef WATER_RAID_DRIVER_H
#define WATER_RAID_DRIVER_H

#include "common_data_structure.h"
#include "../VideoDriver/water_video.h"

class WaterDriver{
public:
    // video
    static void initBackground(int videoFd); // set up the fuel gauge and scoreboard
    static void writeBoundary(int videoFd, BoundaryInRow boundary); // write boundary for each row
    static void writePosition(int videoFd,Position position,int type, int index); // write position for each sprite
    static void writeFuel(int videoFd,int fuel); // adjust the indicator of the fuel gauge
    static void writeScore(int videoFd,int score); // change the scores in the scoreboard
    // audio
    static void playAudio(int audioFd,int index); // play audio of different sound effect

};

#endif //WATER_RAID_DRIVER_H
