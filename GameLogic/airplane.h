//
// Created by Yongmao Luo on 4/1/22.
//

// The airplane object
// Copyright (C) 2022  Yongmao Luo

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
}InputEvent;

class Airplane{
private:
    char type; // what type of sprite it is
    Position pos; // the position of the plane
    Shape shape; // the shape of the sprite
    InputEvent xboxInput; // the input data from xbox
    bool buttonXOn,buttonBOn; // help to determine if the user keeps pressing the two buttons
public:
    int scores,fuel;
    void fire(int xboxFd,int videoFd,vector<Bullet> &bulletList); // Fire a bullet
    bool isCrashed(int videoFd,BoundaryInRow boundary); // if it crashes on the boundary
    bool isCrashed(int videoFd,
                   std::vector<EnemyPlane> enemyPlaneList,
                   std::vector<Battleship> battleList); // if the plane crashes on some enemy sprites
    void addScore(int videoFd,int score); // add scores to the plane
    void addFuel(int videoFd,std::vector<FuelTank> &fuelTankList, std::vector<short> &spriteIndexList); // add fuel if the plane bumps into the fuel tank
    int reduceFuel(int videoFd); // when time flies, the plane should consume more fuels
    Position getPos(); // get the position of the plane
    void setPos(Position); // set the position of the plane
    void receiveFromXbox(int xboxFd); // receive control signals from the Xbox
    void calPos(int videoFd); // calculate the new position based on the received data
    bool startGame(); // If we press button A, the game starts
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