//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_FUEL_TANK_H
#define WATER_RAID_FUEL_TANK_H

#include "sprite.h"

class FuelTank : public Sprite
{

private:
    char fuelVolume;

    void checkIfHit(vector<Bullet> bullets, Airplane airplane);

};

#endif //WATER_RAID_FUEL_TANK_H
