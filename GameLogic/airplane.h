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
#include <pthread.h>

typedef struct {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
}inputEvent;

class Airplane{
private:
    //pthread_mutex_t mutexPos;
    char type;
    Position pos;
    Shape shape;
    inputEvent tempInput;
    bool buttonXOn,buttonBOn;
public:
    short scores,fuel;
    void fire(int xboxFd,int videoFd,vector<Bullet> &bulletList);
    bool isCrashed(int videoFd,BoundaryInRow boundary);
    bool isCrashed(int videoFd,BoundaryInRow boundary,
                   std::vector<EnemyPlane> enemyPlaneList,
                   std::vector<Battleship> battleList);
    void addFuel(int videoFd,std::vector<FuelTank> &fuelTankList);
    int reduceFuel(int videoFd);
    Position getPos();
    void setPos(Position);
    void receivePos(int xboxFd,int videoFd);
    void calPos(int videoFd);
    Airplane(char type, char fuel, Position pos, Shape shape, char scores);

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
        //mutexPos= PTHREAD_MUTEX_INITIALIZER;
        type=0;
        fuel=100;
        pos.x=320;
        pos.y=480;
        scores=0;
        shape.length=3;
        shape.width=5;
    }
};

#endif