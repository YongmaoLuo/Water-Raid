//
// Created by Frank on 4/1/22.
//

// Bullet object
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

#ifndef WATER_RAID_BULLET_H
#define WATER_RAID_BULLET_H

#include "common_data_structure.h"
#include <vector>

#define SPRITE_PLANE 0
#define SPRITE_HELI 1
#define SPRITE_BATTLE 2
#define SPRITE_FUEL 3
#define SPRITE_BULLET 4
#define SPRITE_BALLOON 6

class Bullet
{
private:
    char type;
    Shape sp;
    bool isCrashed;



public:
    Position pos;
    short index;

    short getIndex() const;

    const Shape &getSp() const {
        return sp;
    }

    void setSp(const Shape &sp) {
        Bullet::sp = sp;
    }

    Shape getShape(){
        return sp;
    }

    const Position &getPosition() const {
        return pos;
    }

    void setPosition(const Position &pos) {
        Bullet::pos = pos;
    }

    void setCrash(){
        isCrashed = true;
        this->pos.y = 0;
    }

    char getType(){
        return type;
    }

    bool getIsCrashed() const;

    Bullet(char type, const Shape &sp, const Position &pos);

    static void fly(int videoFd,std::vector<Bullet> &);

};

#endif //WATER_RAID_BULLET_H

