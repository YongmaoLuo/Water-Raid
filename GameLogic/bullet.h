//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_BULLET_H
#define WATER_RAID_BULLET_H

#include "common_data_structure.h"
#include <vector>

#define SPRITE_PLANE 0
#define SPRITE_HELI 1
#define SPRITE_BATTLE 2
#define SPRITE_FUEL 3
#define SPRITE_BULLET 4
#define SPRITE_BALLOON 6

class Bullet
{
private:
    char type;
    Shape sp;
    bool isCrashed;



public:
    Position pos;
    short index;

    short getIndex() const;

    const Shape &getSp() const {
        return sp;
    }

    void setSp(const Shape &sp) {
        Bullet::sp = sp;
    }

    Shape getShape(){
        return sp;
    }

    const Position &getPosition() const {
        return pos;
    }

    void setPosition(const Position &pos) {
        Bullet::pos = pos;
    }

    void setCrash(){
        isCrashed = true;
        this->pos.y = 0;
    }

    char getType(){
        return type;
    }

    bool getIsCrashed() const;

    Bullet(char type, const Shape &sp, const Position &pos);

    static void fly(int videoFd,std::vector<Bullet> &);

};

#endif //WATER_RAID_BULLET_H

