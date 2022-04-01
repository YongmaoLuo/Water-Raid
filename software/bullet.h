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
    shape sp;
    bool isCrashed;


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

    void setCrash(){
        isCrashed= true;
    }

    void clearCrash(){
        isCrashed= false;
    }



    void fly();

};

#endif //WATER_RAID_BULLET_H

