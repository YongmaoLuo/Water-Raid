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
#include "vector"
class Airplane{
private:
    char type,fuel;
    Position pos;
    Shape shape;
    int scores;
    bool isCrash;
public:
    void Fire(Bullet bullet){
        bullet.setPosition(pos);
    }
    void Move(int speed){
        pos.x+=speed*0.01;
    }
    bool isCrashed(BoundaryInRow boundary, std::vector<EnemyPlane> enemyPlanes, std::vector<Battle> battles){
        if(boundary.river1_left!=boundary.river2_left)
            if(this->pos.x<=boundary.river1_left|| this->pos.y>=boundary.river1_right)
        return isCrash;
    }
    void addScore(int score){
        scores+=score;
    }
    char getType(){
        return this->type;
    }
    char getFuel(){
        return this->fuel;
    }
    Airplane(){
        type=0;
        fuel=100;
        pos.x=300;
        pos.y=300;
        scores=0;
        shape.length=3;
        shape.width=5;
        isCrash= false;
    }
};

#endif