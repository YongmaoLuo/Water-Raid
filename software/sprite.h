//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_SPRITE_H
#define WATER_RAID_SPRITE_H

#include "common_data_structure.h"
#include "bullet.h"
#include<vector>;

using namespace std;

class Sprite
{
protected:
    char type;
    char hitPoint;
    char id;

    void generate(short boundary[]);
    void generate();

    void disappear(char id, vector<Sprite> sprites);
    void disappear();

    void move(short boundary[]);
    void move();

    void checkIfHit(vector<Bullet> bullets);
    void checkIfHit();

public:
    bool isHit;

    Position pos;

    Shape sp;

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

    Position getPos() {
        return pos;
    }

public:

    void setPos(const Position &pos) {
        Sprite::pos = pos;
    }
};

#endif //WATER_RAID_SPRITE_H
