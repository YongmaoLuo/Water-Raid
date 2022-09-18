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

#include "bullet.h"
#include "driver.h"
using namespace std;

void Bullet::fly(int videoFd,vector<Bullet> &bulletList) {
    for (int i = 0; i < bulletList.size(); i++)
    {
        Position tempPos=bulletList[i].getPosition();tempPos.y-=8;
        bulletList[i].setPosition(tempPos);
        WaterDriver::writePosition(videoFd,bulletList[i].getPosition(),SPRITE_BULLET,
                                   bulletList[i].getIndex());
        if (bulletList[i].pos.y <= (0 << 1) + 1){
            bulletList[i].setCrash();
            WaterDriver::writePosition(videoFd, bulletList[i].getPosition(), SPRITE_BULLET,
                                       bulletList[i].getIndex());
            bulletList.erase(bulletList.begin()+i);
        }
    }

}

Bullet::Bullet(char type, const Shape &sp, const Position &pos) : type(type), sp(sp),
                                                                                   pos(pos) {}

bool Bullet::getIsCrashed() const {
    return isCrashed;
}

short Bullet::getIndex() const {
    return index;
}
