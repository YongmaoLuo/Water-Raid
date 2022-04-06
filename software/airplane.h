//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_AIRPLANE_H
#define WATER_RAID_AIRPLANE_H

#include "common_data_structure.h"
#include "bullet.h"
#include "enemy_plane.h"
#include "fuel_tank.h"
#include "battleship.h"
#include <vector>

class Airplane{
private:
    char type,fuel;
    Position pos;
    Shape shape;
    short scores;
    bool isCrash;
public:
    void Fire(Bullet bullet);
    void Move(int speed);
    bool isCrashed(BoundaryInRow boundary, std::vector<EnemyPlane> enemyPlanes, std::vector<Battleship> battles);
    void addScore(short score);
    void addFuel(char fuel);
    Airplane(char type, char fuel, Position pos, Shape shape, char scores, bool isCrash);

    char getType(){
        return this->type;
    }
    char getFuel(){
        return this->fuel;
    }
    char getScores(){
        return this->scores;
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