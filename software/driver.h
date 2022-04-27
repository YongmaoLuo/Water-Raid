//
// Created by Yongmao Luo on 4/8/22.
//

#ifndef WATER_RAID_DRIVER_H
#define WATER_RAID_DRIVER_H

#include "common_data_structure.h"
#include "sprite.h"
#include "airplane.h"

class WaterDriver{
public:
    // video
    static void writeBoundary(BoundaryInRow boundary);
    static void writeSprite(Sprite sprite);
    static void writeBullet(Bullet bullet);
    static void writePlane(Airplane plane);
    static void writeFuel(int fuel);
    static void writeScore(int score);
    // audio

    //control device
    static int readPosition();
    static int readFire();
};

#endif //WATER_RAID_DRIVER_H
