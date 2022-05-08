//
// Created by Yongmao Luo on 4/8/22.
//

#ifndef WATER_RAID_DRIVER_H
#define WATER_RAID_DRIVER_H

#include "common_data_structure.h"
#include "../VideoDriver/water_video.h"
#include "sprite.h"
#include "airplane.h"

class WaterDriver{
public:
    // video
    static void initBackground();
    static void writeBoundary(int videoFd, BoundaryInRow boundary);
    static void writePosition(int videoFd,Position position,int type, int index);
    static void writeFuel(int videoFd,int fuel);
    static void writeScore(int videoFd,int score);
    // audio


};

#endif //WATER_RAID_DRIVER_H
