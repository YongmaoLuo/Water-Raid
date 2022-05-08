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
    short index;
    void checkIfHit(vector<Bullet> bullets, short &fuel);

    FuelTank(char type, char hitPoint, char id, const Shape &sp, bool isDestroy, const Position &pos, char fuelVolume);
};

#endif //WATER_RAID_FUEL_TANK_H
