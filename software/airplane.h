//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_AIRPLANE_H
#define WATER_RAID_AIRPLANE_H

#include"common_data_structure.h"

class Airplane{
private:
    char type,fuel;
    position pos;
    int score;
    bool isCrash;
    void fire();
    void move();
    void ifCrash();
    void addScore();
};

#endif