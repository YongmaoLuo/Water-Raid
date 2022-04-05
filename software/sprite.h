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
    Shape sp;

    void generate(BoundaryInRow boundary);
    void generate(short boundary[]);
    void generate();

    void disappear(char id, vector<Sprite> sprites);
    void disappear();

    void move(BoundaryInRow boundary);
    void move(short boundary[]);
    void move();

    void checkIfHit(vector<Bullet> bullets);
    void checkIfHit();

public:
    bool isDestory;
    bool isHit;

    Position pos;

    Sprite(char type, char hitPoint, char id, const Shape &sp, bool isDestory, const Position &pos) : type(type),
    Sprite(char type, char hitPoint, char id, const Shape &sp, bool isHit, const Position &pos) : type(type),
                                                                                                  hitPoint(hitPoint),
                                                                                                  id(id), sp(sp),
                                                                                                  isDestory(isDestory),
                                                                                                  isHit(isHit),
                                                                                                  pos(pos) {}

    bool getIsDestory() const {
        return isDestory;
    bool getIsHit() const {
        return isHit;
    }

    void setIsDestory(bool isDestory) {
        Sprite::isDestory = isDestory;
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
