//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_BATTLESHIP_H
#define WATER_RAID_BATTLESHIP_H

#include "sprite.h"

class Battleship : public Sprite
{
private:
    char score;

    void checkIfHit(vector<Bullet> bullets, Airplane airplane);
};

#endif //WATER_RAID_BATTLESHIP_H
