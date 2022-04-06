//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_SPRITE_H
#define WATER_RAID_SPRITE_H

#include "common_data_structure.h"
#include "bullet.h"
#include "airplane.h"
#include<vector>

using namespace std;

class Sprite
{
protected:
    char type;
    char hitPoint;
    char id;
    Shape sp;

    void generate(BoundaryInRow boundary);

    void disappear(char id, vector<Sprite> sprites);

    void move(BoundaryInRow boundary);

    virtual void checkIfHit(vector<Bullet> bullets, Airplane airplane);

public:
    bool isDestroy;

    Position pos;

    Sprite(char type, char hitPoint, char id, const Shape &sp, bool isDestroy, const Position &pos) : type(type),
                                                                                                  hitPoint(hitPoint),
                                                                                                  id(id), sp(sp),
                                                                                                  isDestroy(isDestroy),
                                                                                                  pos(pos) {}

    bool getIsDestroy() const {
        return isDestroy;
    }

    void setIsDestroy(bool isDestroy) {
        Sprite::isDestroy = isDestroy;
    }

    const Position &getPos() const {
        return pos;
    }

    void setPos(const Position &pos) {
        Sprite::pos = pos;
    }
};

#endif //WATER_RAID_SPRITE_H
