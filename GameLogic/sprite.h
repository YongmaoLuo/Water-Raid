//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_SPRITE_H
#define WATER_RAID_SPRITE_H

#define SPRITE_PLANE 0
#define SPRITE_HELI 1
#define SPRITE_BATTLE 2
#define SPRITE_FUEL 3
#define SPRITE_BULLET 4

#define SPRITE_X 14
#define SPRITE_Y 14

#include "common_data_structure.h"
#include "bullet.h"
#include "game_scenario.h"
#include "driver.h"
#include<vector>

using namespace std;

class Sprite
{
protected:
    char type;
    char hitPoint;
    Shape sp;
    bool left = true;
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

    char getType(){
        return type;
    }

    //start from 4 to 8

    void disappear();

    void move(BoundaryInRow boundary, short minimumWidth);
};

#endif //WATER_RAID_SPRITE_H
