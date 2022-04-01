//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_BULLET_H
#define WATER_RAID_BULLET_H

#include "common_data_structure.h"

class Bullet
{
private:
    char type;
    shape sp;


public:
    position pos;

    const shape &getSp() const {
        return sp;
    }

    void setSp(const shape &sp) {
        Bullet::sp = sp;
    }

    const position &getPosition() const {
        return pos;
    }

    void setPosition(const position &pos) {
        Bullet::pos = pos;
    }

    Bullet(char type, const shape &sp, const position &pos) : type(type), sp(sp), pos(pos) {}

    void fly();

};

#endif //WATER_RAID_BULLET_H

