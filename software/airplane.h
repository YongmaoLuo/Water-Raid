//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_AIRPLANE_H
#define WATER_RAID_AIRPLANE_H

#include"common_data_structure.h"
#include "bullet.h"
class Airplane{
private:
    char type,fuel;
    position pos;
    int scores;
    bool isCrash;
public:
    void fire(Bullet bullet){
        bullet.setPosition(pos);
    }
    void move(int speed){
        pos.x+=speed*0.01;
    }
    bool isCrashed(){
        return isCrash;
    }
    void addScore(int score){
        scores+=score;
    }
    Airplane(){
        type=0;
        fuel=100;
        pos.x=300;
    }
};

#endif