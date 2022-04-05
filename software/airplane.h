//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_AIRPLANE_H
#define WATER_RAID_AIRPLANE_H

#include"common_data_structure.h"
#include "bullet.h"
#include "enemy_plane.h"
#include "fuel_tank.h"
#include "battleship.h"
#include "vector"
class Airplane{
private:
    char type,fuel;
    Position pos;
    Shape shape;
    short scores;
    bool isCrash;
public:
    void Fire(Bullet bullet){
        bullet.setPosition(pos);
    }
    void Move(int speed){
        pos.x+=speed*0.01;
    }
    bool isCrashed(BoundaryInRow boundary, std::vector<EnemyPlane> enemyPlanes, std::vector<Battle> battles){
        // collide the boundary
        if(boundary.river2_left!=0)
            if(pos.x<=boundary.river1_left|| pos.x+shape.width>=boundary.river1_right&& this->pos.x<boundary.river2_left||
                                                   pos.x+shape.width>=boundary.river2_right){
                isCrash=true;
                pos.y|=~0x1; // set y[0] to be 1
            }
        else
            if(pos.x<=boundary.river1_left|| pos.x+shape.width>=boundary.river1_right){
                isCrash=true;
                this->pos.y|=~0x1; // set y[0] to be 1
                return isCrash;
            }

        // collide the enemy planes
        for(int i=0;i<enemyPlanes.size();i++){
            if(pos.y>=enemyPlanes[i].getPos().y&&pos.x<=enemyPlanes[i].getPos().x+enemyPlanes[i].sp.width&&pos.x>=enemyPlanes[i].getPos().x){
                isCrash=true;
                pos.y|=~0x1; // set y[0] to be 1
                return isCrash;
            }
        }
        //collide the battleships
        for(int i=0;i<battles.size();i++){
            if(pos.y>=battles[i].getPos().y&&pos.x<=battles[i].getPos().x+battles[i].sp.width&&pos.x>=battles[i].getPos().x){
                isCrash=true;
                pos.y|=~0x1; // set y[0] to be 1
                return isCrash;
            }
        }
        return isCrash;
    }
    void addScore(short score){
        this->scores += score;
    }
    void addFuel(char fuel){
        this->fuel += fuel;
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