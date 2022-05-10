//
// Created by Yongmao Luo on 4/1/22.
//

#include "bullet.h"

#include "game_scenario.h"

#include "battleship.h"
#include "enemy_plane.h"
#include "airplane.h"
#include "driver.h"
#include "common_data_structure.h"
#include "sprite.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include<ctime>
#include<iostream>

// type of different sprite
#define SPRITE_PLANE 0
#define SPRITE_HELI 1
#define SPRITE_BATTLE 2
#define SPRITE_FUEL 3
#define SPRITE_BULLET 4
#define SPRITE_BALLOON 6

// center coordinate related to upper left corner
#define SPRITE_X 15
#define SPRITE_Y 15

#define MINIMUM_RIVER_WIDTH 50
#define MAXFUEL 75

#define SHOOT_AUDIO 0
#define HIT_AUDIO 1
#define EXPLODE_AUDIO 2

using namespace std;

// the struct used to store different kinds of sprites

int main() {
    vector<Bullet> bulletList;
    vector<Battleship> battleList;
    vector<EnemyPlane> enemyList;
    vector<short> spriteIndexList = {4,5,6,7,8};
    vector<FuelTank> fuelTankList;
    Airplane airplane;

    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[]="/dev/water_video";
    int videoFd,xboxFd;

//    if((xboxFd= open(xbox,O_RDWR))==-1){
//        fprintf(stderr,"could not open %s\n",xbox);
//        return -1;
//    }

    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", waterVideo);
        return -1;
    }

    GameScenario gameScenario(videoFd,50,640,60);
    double duration=1/gameScenario.getFrequency();
    clock_t execute=clock();
    clock_t reduceFuelClock=clock();
    gameScenario.setChangeClock();


    while(1) {

        gameScenario.initBackground(videoFd);
        WaterDriver::initBackground(videoFd);

        // wait the button on the xbox controller to be pressed
        /* wait to be finished */

        bool isCrashed;
        Position tempPos;
        tempPos.x = 320;
        tempPos.y = (400 << 1) + 1;
        Shape tempShape;
        tempShape.width = SPRITE_X;
        tempShape.length = SPRITE_Y;
        airplane = Airplane(SPRITE_PLANE, MAXFUEL, tempPos, tempShape, 0);

        // the plane enter the screen
        while (airplane.getPos().y > (300 << 1) + 1) {
            WaterDriver::writePosition(videoFd, airplane.getPos(), SPRITE_PLANE, 0);
            tempPos = airplane.getPos();
            tempPos.y -= 2;
            airplane.setPos(tempPos);
            usleep(5000);
        }
        while (1) {
            if (double(clock() - execute) / CLOCKS_PER_SEC >= duration) {
                execute = clock();
                gameScenario.updateBackground(videoFd);

                //receive control signal from xbox
                //airplane.receivePos(xboxFd, videoFd);
                airplane.calPos(videoFd);

                // determine if the plane has crashed
                // plane is always located at y=300
                BoundaryInRow boundaryAheadOfPlane = gameScenario.boundaries[
                        (gameScenario.getScreenHeader() -300+480 + SPRITE_Y) % 480];

                isCrashed = airplane.isCrashed(videoFd, boundaryAheadOfPlane);
                if (isCrashed){
                    WaterDriver::playAudio(videoFd,EXPLODE_AUDIO);
                    usleep(40000);
                    WaterDriver::stopAudio(videoFd,EXPLODE_AUDIO);
                    break;
                }
//                // if the plane bumped into the fuel tank
//                airplane.addFuel(videoFd,fuelTankList);
//
//                // check if the airplane is about to emit a bullet
                //airplane.fire(xboxFd,videoFd,bulletList);
                //printf("bullet list size: %d\n",bulletList.size());
//
//                // reduce fuel
                if ((clock() - reduceFuelClock) / CLOCKS_PER_SEC >= 1) {
                    reduceFuelClock = clock();
                    int temp = airplane.reduceFuel(videoFd);
                    if (temp == -1)
                        break;
                }

                //bullet fly
                Bullet::fly(videoFd,bulletList);

                //enemy plane fly
                EnemyPlane::fly(videoFd, enemyList, spriteIndexList, gameScenario);

                //battleship move
                Battleship::movement(videoFd, battleList, spriteIndexList, gameScenario);

                //fuel tank move
                FuelTank::movement(videoFd, fuelTankList, spriteIndexList, gameScenario);

                //check the enemy plane to see if hit
                for (int i = 0; i < enemyList.size(); i++)
                {
                    enemyList[i].checkIfHit(bulletList, airplane.scores);
                }
                //destroy and release the index
                for (int i = 0; i < enemyList.size(); i++)
                {
                    if(enemyList[i].getIsDestroy()){
                        enemyList[i].disappear();
                        WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
                                                   enemyList[i].getIndex());
                        spriteIndexList.push_back(enemyList[i].getIndex());
                        enemyList.erase(enemyList.begin()+i);
                    }
                }

                //check the battleship to see if hit
                for (int i = 0; i < battleList.size(); i++)
                {
                    battleList[i].checkIfHit(bulletList, airplane.scores);
                }
                //destroy and release the index
                for (int i = 0; i < battleList.size(); i++)
                {
                    if(battleList[i].getIsDestroy()){
                        battleList[i].disappear();
                        WaterDriver::writePosition(videoFd, battleList[i].getPos(), SPRITE_BATTLE,
                                                   battleList[i].getIndex());
                        spriteIndexList.push_back(battleList[i].getIndex());
                        battleList.erase(battleList.begin()+i);
                    }
                }

                //check the fuel tank to see if hit
                for (int i = 0; i < fuelTankList.size(); i++)
                {
                    fuelTankList[i].checkIfHit(bulletList, airplane.fuel);
                }
                //destroy and release the index
                for (int i = 0; i < fuelTankList.size(); i++)
                {
                    if(fuelTankList[i].getIsDestroy()){
                        fuelTankList[i].disappear();
                        WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
                                                   fuelTankList[i].getIndex());
                        spriteIndexList.push_back(fuelTankList[i].getIndex());
                        fuelTankList.erase(fuelTankList.begin()+i);
                    }
                }

                // update bullet (for aimed)
                for (int i = 0; i < bulletList.size(); i++)
                {
                    if (bulletList[i].getIsCrashed()){
                        WaterDriver::writePosition(videoFd, bulletList[i].getPosition(), SPRITE_BULLET,
                                                   bulletList[i].getIndex());
                        bulletList.erase(bulletList.begin()+i);
                    }
                }
                //if possible, randomly generate sprite
                while (!spriteIndexList.empty()) {
                    switch (rand() % 3) {
                        case 0: {    //for generate enemy plane
                            Shape newShape;
                            newShape.width = SPRITE_X;
                            newShape.length = SPRITE_Y;
                            EnemyPlane enemyPlane = EnemyPlane(2, 1, newShape, false, 2, spriteIndexList[0]);
                            spriteIndexList.erase(spriteIndexList.begin());
                            enemyList.push_back(enemyPlane);
                            short randomRow = rand() % 284;
                            short rowIndex = (gameScenario.getScreenHeader() - randomRow + 480 + SPRITE_Y) % 480;
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                            enemyPlane.generate(boundaryGenerateInRow, randomRow);
                            WaterDriver::writePosition(videoFd, enemyPlane.getPos(), SPRITE_HELI,
                                                       enemyPlane.getIndex());
                            break;
                        }
                        case 1: {   //for generate enemy battleship
                            Shape newShape;
                            newShape.width = SPRITE_X;
                            newShape.length = SPRITE_Y;
                            Battleship battleship = Battleship(3, 2, newShape, false, 3, spriteIndexList[0]);
                            spriteIndexList.erase(spriteIndexList.begin());
                            battleList.push_back(battleship);
                            short randomRow = rand() % 284;
                            short rowIndex = (gameScenario.getScreenHeader() - randomRow + 480 + SPRITE_Y) % 480;
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                            battleship.generate(boundaryGenerateInRow, randomRow);
                            WaterDriver::writePosition(videoFd, battleship.getPos(), SPRITE_BATTLE,
                                                       battleship.getIndex());
                            break;
                        }

                        case 2: {//for generate enemy fuel_tank
                            Shape newShape;
                            newShape.width = SPRITE_X;
                            newShape.length = SPRITE_Y;
                            FuelTank fuelTank = FuelTank(4, 1, newShape, false, 2, spriteIndexList[0]);
                            spriteIndexList.erase(spriteIndexList.begin());
                            fuelTankList.push_back(fuelTank);
                            short randomRow = rand() % 284;
                            short rowIndex = (gameScenario.getScreenHeader() - randomRow + 480 + SPRITE_Y) % 480;
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                            fuelTank.generate(boundaryGenerateInRow, randomRow);
                            WaterDriver::writePosition(videoFd, fuelTank.getPos(), SPRITE_FUEL,
                                                       fuelTank.getIndex());
                            break;
                        }
                    }
                }
            }

        }
    }
    return 0;
}

