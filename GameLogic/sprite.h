//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_SPRITE_H
#define WATER_RAID_SPRITE_H

#include "common_data_structure.h"
#include "bullet.h"
#include<vector>

using namespace std;

class Sprite
{
protected:
    char type;
    char hitPoint;
    Shape sp;
    bool left;
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

    //start from 4 to 8

    void disappear();

    void move(BoundaryInRow boundary, short minimumWidth);
};

#endif //WATER_RAID_SPRITE_H
