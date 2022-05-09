//
// Created by Frank on 4/1/22.
//
#include "bullet.h"
using namespace std;

void Bullet::fly() {
    this->pos.y -= 4;
}

Bullet::Bullet(char type, const Shape &sp, const Position &pos) : type(type), sp(sp),
                                                                                   pos(pos) {}

bool Bullet::getIsCrashed() const {
    return isCrashed;
}

short Bullet::getIndex() const {
    return index;
}
