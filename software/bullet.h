//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_BULLET_H
#define WATER_RAID_BULLET_H

#include "common_data_structure.h"

#define MAX_BULLET_NUM

class Bullet
{
private:
    char type;
    Shape sp;
    bool isCrashed;


public:
    Position pos;

    const Shape &getSp() const {
        return sp;
    }

    void setSp(const Shape &sp) {
        Bullet::sp = sp;
    }

    const Position &getPosition() const {
        return pos;
    }

    void setPosition(const Position &pos) {
        Bullet::pos = pos;
    }

    void setCrash(){
        isCrashed = true;
        this->pos.y |=~0x1;
    }

    bool getIsCrashed() const;

    Bullet(char type, const Shape &sp, bool isCrashed, const Position &pos);


    void fly();

};

#endif //WATER_RAID_BULLET_H

