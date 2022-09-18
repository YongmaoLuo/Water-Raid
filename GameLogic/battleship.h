//
// Created by Frank on 4/1/22.
//

// Battleship object
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
