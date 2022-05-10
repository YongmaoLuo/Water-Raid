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
    void checkIfHit(vector<Bullet> &bullets);

    FuelTank(char type, char hitPoint, const Shape &sp, bool isDestroy, char fuelVolume, short index, bool canMove);

};

#endif //WATER_RAID_FUEL_TANK_H
