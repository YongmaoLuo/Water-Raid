//
// Created by Yongmao Luo on 4/1/22.
//

#include "bullet.h"
#include "bullet.cpp"

#include "game_scenario.h"

#include "battleship.h"
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
#define SPRITE_EXPLODE 5
#define SPRITE_BALLOON 6

// center coordinate related to upper left corner
#define SPRITE_X 16
#define SPRITE_Y 16

#define MINIMUM_RIVER_WIDTH 50
using namespace std;
// the struct used to store different kinds of sprites

int main() {
    vector<Bullet> bulletList;
    vector<Battleship> battleList;
    vector<EnemyPlane> enemyList;
    vector<FuelTank> fuelTankList;
    vector<short> spriteIndexList = {4, 5, 6, 7, 8};
    Position tempPos;
    tempPos.x = 320;
    tempPos.y = (480 << 1) + 1;
    Shape tempShape;
    tempShape.width = 16;
    tempShape.length = 16;
    Airplane airplane = Airplane(SPRITE_PLANE, 80, tempPos, tempShape, 0);
    GameScenario gameScenario(30, 640, 60);

    srand(time(0));

    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[] = "/dev/vga_ball";
    int videoFd;

    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", waterVideo);
        return -1;
    }


    double duration = 1 / gameScenario.getFrequency();
    clock_t execute = clock();
    gameScenario.setChangeClock();
    while (1) {
        WaterDriver::initBackground();

        // wait the button on the xbox controller to be pressed
        /* wait to be finished */


        // the plane enter the screen
        while (airplane.getPos().y > (300 << 1) + 1) {
            WaterDriver::writePosition(videoFd, airplane.getPos(), SPRITE_PLANE, 0);
            tempPos = airplane.getPos();
            tempPos.y -= 2;
            airplane.setPos(tempPos);
            usleep(40000);
        }
        while (1) {
            if (double(clock() - execute) / CLOCKS_PER_SEC >= duration) {
                execute = clock();
                gameScenario.updateBackground(videoFd);
                // determine if the plane has crashed
                // plane is always located at y=300
//                BoundaryInRow boundaryAheadOfPlane = gameScenario.boundaries[gameScenario.getScreenHeader() + 180 -
//                                                                             SPRITE_Y];
                //bullet fly
                for (int i = 0; i < bulletList.size(); i++)
                {
                    bulletList[i].fly();
                    if (bulletList[i].pos.y == 0){
                        bulletList[i].setCrash();
                        WaterDriver::writePosition(videoFd, bulletList[i].getPosition(), SPRITE_BULLET,
                                                   bulletList[i].getIndex());
                        bulletList.erase(bulletList.begin()+i);
                    }
                }

                //enemy plane fly
                for (int i = 0; i < enemyList.size(); i++)
                {
                    enemyList[i].pos.y += 1;
                    if(enemyList[i].pos.y == 480){
                        enemyList[i].disappear();
                        WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
                                                   enemyList[i].getIndex());
                        spriteIndexList.push_back(enemyList[i].getIndex());
                        enemyList.erase(enemyList.begin()+i);
                    } else{
                        enemyList[i].move(gameScenario.boundaries[gameScenario.getScreenHeader() + enemyList[i].pos.y], MINIMUM_RIVER_WIDTH);
                        WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
                                                   enemyList[i].getIndex());
                    }
                }

                //battleship move
                for (int i = 0; i < battleList.size(); i++)
                {
                    battleList[i].pos.y += 1;
                    if(battleList[i].pos.y == 480){
                        battleList[i].disappear();
                        WaterDriver::writePosition(videoFd, battleList[i].getPos(), SPRITE_BATTLE,
                                                   battleList[i].getIndex());
                        spriteIndexList.push_back(battleList[i].getIndex());
                        battleList.erase(battleList.begin()+i);
                    } else{
                        battleList[i].move(gameScenario.boundaries[gameScenario.getScreenHeader() + battleList[i].pos.y], MINIMUM_RIVER_WIDTH);
                        WaterDriver::writePosition(videoFd, battleList[i].getPos(), SPRITE_BATTLE,
                                                   battleList[i].getIndex());
                    }
                }

                //fuel tank move
                for (int i = 0; i < fuelTankList.size(); i++)
                {
                    fuelTankList[i].pos.y += 1;
                    if(fuelTankList[i].pos.y == 480){
                        fuelTankList[i].disappear();
                        WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
                                                   fuelTankList[i].getIndex());
                        spriteIndexList.push_back(fuelTankList[i].getIndex());
                        fuelTankList.erase(fuelTankList.begin()+i);
                    } else{
                        fuelTankList[i].move(gameScenario.boundaries[gameScenario.getScreenHeader() + fuelTankList[i].pos.y], MINIMUM_RIVER_WIDTH);
                        WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
                                                   fuelTankList[i].getIndex());
                    }
                }

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
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[
                                    gameScenario.getScreenHeader() + (rand() % (480 - gameScenario.getScreenHeader()))];
                            enemyPlane.generate(boundaryGenerateInRow, (gameScenario.getScreenHeader() + (rand() %
                                                                                                          (480 -
                                                                                                           gameScenario.getScreenHeader() - airplane.getPos().y))));
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
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[
                                    gameScenario.getScreenHeader() + (rand() % (480 - gameScenario.getScreenHeader()))];
                            battleship.generate(boundaryGenerateInRow, (gameScenario.getScreenHeader() + (rand() %
                                                                                                          (480 -
                                                                                                           gameScenario.getScreenHeader() - airplane.getPos().y))));
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
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[
                                    gameScenario.getScreenHeader() + (rand() % (480 - gameScenario.getScreenHeader()))];
                            fuelTank.generate(boundaryGenerateInRow, (gameScenario.getScreenHeader() + (rand() %
                                                                                                          (480 -
                                                                                                           gameScenario.getScreenHeader() - airplane.getPos().y))));
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

