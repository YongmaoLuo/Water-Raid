//
// Created by Yongmao Luo on 4/8/22.
//

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
