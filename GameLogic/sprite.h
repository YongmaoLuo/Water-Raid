//
// Created by Frank on 4/1/22.
//

// The father class of the objects
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

#ifndef WATER_RAID_SPRITE_H
#define WATER_RAID_SPRITE_H

#define SPRITE_PLANE 0
#define SPRITE_HELI 1
#define SPRITE_BATTLE 2
#define SPRITE_FUEL 3
#define SPRITE_BULLET 4

#define SPRITE_X 12
#define SPRITE_Y 12

#include "common_data_structure.h"
#include "bullet.h"
#include "game_scenario.h"
#include "driver.h"
#include<vector>

using namespace std;

class Sprite
{
protected:
    char type;
    char hitPoint;
    Shape sp;
    bool left = false;
    bool canMove;

public:
    short index;
    bool isDestroy;

    Position pos;

    Sprite(char type, char hitPoint, const Shape &sp, bool isDestroy, short index, bool canMove) : type(type),
                                                                                                  hitPoint(hitPoint),
                                                                                                  sp(sp),
                                                                                                  isDestroy(isDestroy),
                                                                                                  index(index),canMove(canMove){}



    bool getIsDestroy() const {
        return isDestroy;
    }

    void setIsDestroy(bool isDestroy) {
        Sprite::isDestroy = isDestroy;
    }

    const Position &getPos() const {
        return pos;
    }

    Shape getShape();

    void setPos(const Position &pos) {
        Sprite::pos = pos;
    }

    void generate(BoundaryInRow boundary, short y);

    short getIndex() const {
        return index;
    }

    char getType(){
        return type;
    }

    //start from 4 to 8

    void disappear();

    void move(int videoFd, BoundaryInRow boundary, short minimumWidth);
};

#endif //WATER_RAID_SPRITE_H
