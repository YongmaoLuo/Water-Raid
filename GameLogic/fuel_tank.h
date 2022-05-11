//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_FUEL_TANK_H
#define WATER_RAID_FUEL_TANK_H

#include "sprite.h"
#include <vector>

class FuelTank : public Sprite
{

private:
    char fuelVolume;

public:
    void checkIfHit(vector<Bullet> &bullets,int videoFd);

    FuelTank(char type, char hitPoint, const Shape &sp, bool isDestroy, char fuelVolume, short index, bool canMove);

    static void movement(int videoFd,vector<FuelTank> &enemyList, vector<short> &spriteIndexList, GameScenario gameScenario);
};

#endif //WATER_RAID_FUEL_TANK_H
