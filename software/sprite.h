//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_SPRITE_H
#define WATER_RAID_SPRITE_H

#include "common_data_structure.h"

class Sprite
{
protected:
    char type;
    char hitPoint;
    char id;
    Shape sp;

    void generate();

    void disappear();

    void move();

    void checkIfHit();

public:
    bool isHit;

    Position pos;

    Sprite(char type, char hitPoint, char id, const Shape &sp, bool isHit, const Position &pos) : type(type),
                                                                                                  hitPoint(hitPoint),
                                                                                                  id(id), sp(sp),
                                                                                                  isHit(isHit),
                                                                                                  pos(pos) {}

    bool getIsHit() const {
        return isHit;
    }

    void setIsHit(bool isHit) {
        Sprite::isHit = isHit;
    }

    const Position &getPos() const {
        return pos;
    }

    void setPos(const Position &pos) {
        Sprite::pos = pos;
    }


};

#endif //WATER_RAID_SPRITE_H
