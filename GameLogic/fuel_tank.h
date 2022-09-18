//
// Created by Frank on 4/1/22.
//

// Fuel Tank object
// Copyright (C) 2022  Yongmao Luo, Frank Wang

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

//    static void movement(int videoFd,vector<FuelTank> &enemyList, vector<short> &spriteIndexList, GameScenario gameScenario);
};

#endif //WATER_RAID_FUEL_TANK_H
