//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_AIRPLANE_H
#define WATER_RAID_AIRPLANE_H

#include"common_data_structure.h"
#include "bullet.h"
#include "enemy_plane.h"
#include "fuel_tank .h"
#include "battleship.h"
class Airplane{
private:
    char type,fuel;
    Position pos;
    int scores;
    bool isCrash;
public:
    void Fire(Bullet bullet){
        bullet.setPosition(pos);
    }
    void Move(int speed){
        pos.x+=speed*0.01;
    }
    bool isCrashed(BoundaryInRow boundary, EnemyPlane enemyPlanes[]){
        return isCrash;
    }
    void addScore(int score){
        scores+=score;
    }
    Airplane(){
        type=0;
        fuel=100;
        pos.x=300;
        pos.y=300;
        scores=0;
        isCrash= false;
    }
};

#endif