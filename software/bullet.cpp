//
// Created by Frank on 4/1/22.
//
#include "bullet.h"
using namespace std;

void Bullet::fly() {
    this->pos.y -= 1;
}

Bullet::Bullet(char type, const shape &sp, bool isCrashed, const position &pos) : type(type), sp(sp),
                                                                                  isCrashed(isCrashed), pos(pos) {}

bool Bullet::getIsCrashed() const {
    return isCrashed;
}
