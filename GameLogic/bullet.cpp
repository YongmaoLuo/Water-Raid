//
// Created by Frank on 4/1/22.
//
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
