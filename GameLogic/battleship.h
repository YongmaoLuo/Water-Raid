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
    void checkIfHit(vector<Bullet> &bullets,int videoFd, int &planeScore);

    Battleship(char type, char hitPoint, const Shape &sp, bool isDestroy, char score, short index,bool canMove);
};

#endif //WATER_RAID_BATTLESHIP_H
