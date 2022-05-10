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
    void checkIfHit(vector<Bullet> &bullets, short &);

    Battleship(char type, char hitPoint, const Shape &sp, bool isDestroy, char score, short index);

    static void movement(int videoFd,vector<Battleship> &enemyList, vector<short> &spriteIndexList, GameScenario gameScenario);
};

#endif //WATER_RAID_BATTLESHIP_H
