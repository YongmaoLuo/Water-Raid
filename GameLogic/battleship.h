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

public:
    short index;
    void checkIfHit(vector<Bullet> bullets, short &);

    Battleship(char type, char hitPoint, char id, const Shape &sp, bool isDestroy, const Position &pos, char score);
};

#endif //WATER_RAID_BATTLESHIP_H
