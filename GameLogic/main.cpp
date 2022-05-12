//
// Created by Yongmao Luo on 4/1/22.
//

#include "bullet.h"

#include "game_scenario.h"
#include "airplane.h"

#include "battleship.h"
#include "enemy_plane.h"
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
#define SPRITE_X 12
#define SPRITE_Y 12
#define AIRPLANE_X 14
#define AIRPLANE_Y 14

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
    vector<short> spriteIndexList;
    vector<FuelTank> fuelTankList;
    Airplane airplane;

    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[]="/dev/water_video";
    int videoFd,xboxFd;

    if((xboxFd= open(xbox,O_RDWR))==-1){
        fprintf(stderr,"could not open %s\n",xbox);
        return -1;
    }

    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", waterVideo);
        return -1;
    }

    GameScenario gameScenario(videoFd,60,640,60);
    double duration=1/gameScenario.getFrequency();
    clock_t execute=clock();
    clock_t reduceFuelClock=clock();
    clock_t moveClock=clock();
    clock_t counterSurvival;
    gameScenario.setChangeClock();
    bool timeToMove;
    while(1) {

        for (int i = 0; i < bulletList.size(); i++){
            bulletList[i].setCrash();
            WaterDriver::writePosition(videoFd, bulletList[i].getPosition(), SPRITE_BULLET,
                                       bulletList[i].getIndex());
        }

        for (int i = 0; i < battleList.size(); i++){
            battleList[i].disappear();
            WaterDriver::writePosition(videoFd, battleList[i].getPos(), SPRITE_BATTLE,
                                       battleList[i].getIndex());
        }

        for (int i = 0; i < enemyList.size(); i++){
            enemyList[i].disappear();
            WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
                                       enemyList[i].getIndex());
        }

        for (int i = 0; i < fuelTankList.size(); i++){
            fuelTankList[i].disappear();
            WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
                                       fuelTankList[i].getIndex());
        }

        bulletList.clear();
        enemyList.clear();
        battleList.clear();
        fuelTankList.clear();

        gameScenario.initBackground(videoFd);
        WaterDriver::initBackground(videoFd);

        spriteIndexList = {4,5,6,7,8};
        counterSurvival=0;
        timeToMove= false;


        // wait the button on the xbox controller to be pressed
        /* wait to be finished */

        bool isCrashed;
        Position tempPos;
        tempPos.x = 320;
        tempPos.y = (512 << 1) + 1;
        Shape tempShape;
        tempShape.width = AIRPLANE_X;
        tempShape.length = AIRPLANE_Y;
        airplane = Airplane(SPRITE_PLANE, MAXFUEL, tempPos, tempShape, 0);
        WaterDriver::writeScore(videoFd,0);

        // the plane enter the screen
        while (airplane.getPos().y > (400 << 1) + 1) {
            WaterDriver::writePosition(videoFd, airplane.getPos(), SPRITE_PLANE, 0);
            tempPos = airplane.getPos();
            tempPos.y -= 2;
            airplane.setPos(tempPos);
            usleep(5000);
        }

        // wait for user to press button A so we can start the game
        while(!airplane.startGame()){
            airplane.receiveFromXbox(xboxFd);
        }

        int iteration = 35;
        while (1) {
            if (double(clock() - execute) / CLOCKS_PER_SEC >= duration) {

                if(double(clock()-moveClock)/CLOCKS_PER_SEC>0.1){
                    moveClock=clock();
                    timeToMove=true;
                }
                execute = clock();
                gameScenario.updateBackground(videoFd);

                //receive control signal from xbox
                airplane.receiveFromXbox(xboxFd);
                airplane.calPos(videoFd);

                // determine if the plane has crashed
                // plane is always located at y=400
                BoundaryInRow boundaryAheadOfPlane;
                if(gameScenario.getScreenHeader() -400 + SPRITE_Y>=0)
                    boundaryAheadOfPlane = gameScenario.boundaries[
                        gameScenario.getScreenHeader() -400 + SPRITE_Y];
                else
                    boundaryAheadOfPlane = gameScenario.boundaries[
                            gameScenario.getScreenHeader() -400+480 + SPRITE_Y];

                isCrashed = airplane.isCrashed(videoFd, boundaryAheadOfPlane)|| airplane.isCrashed(videoFd,enemyList,battleList);
                if (isCrashed){
                    WaterDriver::playAudio(videoFd,EXPLODE_AUDIO);
                    break;
                }

//                // if the plane bumped into the fuel tank
                airplane.addFuel(videoFd,fuelTankList,spriteIndexList);
//
//                // check if the airplane is about to emit a bullet
                airplane.fire(xboxFd,videoFd,bulletList);
                //printf("bullet list size: %d\n",bulletList.size());
//
//                // reduce fuel
                if ((clock() - reduceFuelClock) / CLOCKS_PER_SEC >= 1) {
                    counterSurvival++;
                    reduceFuelClock = clock();
                    int temp = airplane.reduceFuel(videoFd);
                    if (temp == -1){
                        tempPos.y=0;
                        WaterDriver::writePosition(videoFd,tempPos,airplane.getType(),0);
                        WaterDriver::writePosition(videoFd,airplane.getPos(),SPRITE_EXPLODE,0);
                        break;
                    }
                }

                // add survival scores
                if(counterSurvival>=2){
                    counterSurvival=0;
                    airplane.scores+=1;
                    WaterDriver::writeScore(videoFd,airplane.scores);
                }

                //bullet fly
                Bullet::fly(videoFd,bulletList);


                //enemy plane move
                for (int i = 0; i < enemyList.size(); i++)
                {
                    enemyList[i].pos.y += 2;
                    if(enemyList[i].pos.y >= (512 << 1) + 1){
                        enemyList[i].disappear();
                        spriteIndexList.push_back(enemyList[i].getIndex());
                        WaterDriver::writePosition(videoFd,enemyList[i].pos,enemyList[i].getType(),enemyList[i].getIndex());
                        enemyList.erase(enemyList.begin()+i);
                    } else if(timeToMove){
                        if(gameScenario.getScreenHeader() - int((enemyList[i].getPos().y -1) >> 1)>=0){
                            enemyList[i].move(videoFd,gameScenario.boundaries[
                                                      gameScenario.getScreenHeader() - int((enemyList[i].getPos().y -1) >> 1)], 2);
                        }else
                            enemyList[i].move(videoFd,gameScenario.boundaries[
                                                  (gameScenario.getScreenHeader() - int((enemyList[i].getPos().y -1) >> 1) +
                                                   480 )], 2);
                    }
                    WaterDriver::writePosition(videoFd, enemyList[i].getPos(), enemyList[i].getType(),
                                               enemyList[i].getIndex());
                }

                //battleship move
                for (int i = 0; i < battleList.size(); i++)
                {
                    battleList[i].pos.y += 2;
                    if(battleList[i].pos.y >=  (512 << 1) + 1){
                        battleList[i].disappear();
                        spriteIndexList.push_back(battleList[i].getIndex());
                        WaterDriver::writePosition(videoFd,battleList[i].pos,battleList[i].getType(),battleList[i].getIndex());
                        battleList.erase(battleList.begin()+i);
                    } else if(timeToMove){
                        if((gameScenario.getScreenHeader() - int((battleList[i].getPos().y - 1) >> 1) >=0))
                            battleList[i].move(videoFd,gameScenario.boundaries[(gameScenario.getScreenHeader() - int((battleList[i].getPos().y - 1) >> 1) )],
                                               2);
                        else
                            battleList[i].move(videoFd,gameScenario.boundaries[(gameScenario.getScreenHeader() - int((battleList[i].getPos().y - 1) >> 1) + 480) ],
                                           2);

                    }
                    WaterDriver::writePosition(videoFd, battleList[i].getPos(), battleList[i].getType(),
                                               battleList[i].getIndex());
                }

                //fuel tank move
                for (int i = 0; i < fuelTankList.size(); i++)
                {
                    fuelTankList[i].pos.y += 2;
                    if(fuelTankList[i].pos.y >=  (512 << 1) + 1){
                        fuelTankList[i].disappear();
                        spriteIndexList.push_back(fuelTankList[i].getIndex());
                        WaterDriver::writePosition(videoFd,fuelTankList[i].pos,fuelTankList[i].getType(),fuelTankList[i].getIndex());
                        fuelTankList.erase(fuelTankList.begin()+i);
                    } else if(timeToMove){
                        if(gameScenario.getScreenHeader() - int((fuelTankList[i].getPos().y - 1) >> 1)>=0)
                            fuelTankList[i].move(videoFd,gameScenario.boundaries[(gameScenario.getScreenHeader() - int((fuelTankList[i].getPos().y - 1) >> 1)) ],
                                             2);
                        else
                            fuelTankList[i].move(videoFd,gameScenario.boundaries[(gameScenario.getScreenHeader() - int((fuelTankList[i].getPos().y - 1) >> 1) + 480 )],
                                                 2);
                    }
                    WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), fuelTankList[i].getType(),
                                               fuelTankList[i].getIndex());
                }

                if(timeToMove){
                    timeToMove= false;
                }

                //check the enemy plane to see if hit
                for (int i = 0; i < enemyList.size(); i++)
                {
                    enemyList[i].checkIfHit(bulletList, videoFd,airplane.scores);
                }
                //destroy and release the index
                for (int i = 0; i < enemyList.size(); i++)
                {
                    if(enemyList[i].getIsDestroy()){
                        enemyList[i].disappear();
                        WaterDriver::writePosition(videoFd, enemyList[i].getPos(), enemyList[i].getType(),
                                                   enemyList[i].getIndex());
                        spriteIndexList.push_back(enemyList[i].getIndex());
                        enemyList.erase(enemyList.begin()+i);
                    }
                }

                //check the battleship to see if hit
                for (int i = 0; i < battleList.size(); i++)
                {
                    battleList[i].checkIfHit(bulletList,videoFd, airplane.scores);
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
                    fuelTankList[i].checkIfHit(bulletList,videoFd);
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
                for (int i = 0; i < bulletList.size(); i++) {
                    if (bulletList[i].getIsCrashed()) {
                        WaterDriver::writePosition(videoFd, bulletList[i].getPosition(), SPRITE_BULLET,
                                                   bulletList[i].getIndex());
                        bulletList.erase(bulletList.begin() + i);
                    }
                }
                iteration ++;

                //if possible, randomly generate sprite
                if (!spriteIndexList.empty() && iteration >=35) {
                    iteration = 0;
                    switch (rand() % 7) {
                    case 0: case 6: {    //for generate enemy plane
                            Shape newShape;
                            newShape.width = SPRITE_X;
                            newShape.length = SPRITE_Y;
                            bool canMove= false;
                            if(rand()%5==0){
                                canMove= true;
                            }
                            EnemyPlane enemyPlane = EnemyPlane(SPRITE_HELI, 1, newShape, false, 2, spriteIndexList[0],
                                                               canMove);
                            spriteIndexList.erase(spriteIndexList.begin());
                            short randomRow = 0;
                            short rowIndex = (gameScenario.getScreenHeader() - randomRow) % 480;
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                            enemyPlane.generate(boundaryGenerateInRow, randomRow);
                            enemyList.push_back(enemyPlane);
                            WaterDriver::writePosition(videoFd, enemyPlane.getPos(), SPRITE_HELI,
                                                       enemyPlane.getIndex());
                            break;
                        }
                        case 1: case 4: {   //for generate enemy battleship
                            Shape newShape;
                            newShape.width = SPRITE_X;
                            newShape.length = SPRITE_Y;
                            bool canMove= true;
                            if(rand()%5==0){
                                canMove= false;
                            }
                            Battleship battleship = Battleship(SPRITE_BATTLE, 2, newShape, false, 3, spriteIndexList[0],canMove);
                            spriteIndexList.erase(spriteIndexList.begin());
                            short randomRow = 0;
                            short rowIndex = (gameScenario.getScreenHeader() - randomRow) % 480;
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                            battleship.generate(boundaryGenerateInRow, randomRow);
                            battleList.push_back(battleship);
                            WaterDriver::writePosition(videoFd, battleship.getPos(), SPRITE_BATTLE,
                                                       battleship.getIndex());
                            break;
                        }

                        case 2: {//for generate enemy fuel_tank
                            Shape newShape;
                            newShape.width = SPRITE_X;
                            newShape.length = SPRITE_Y;
                            bool canMove= false;
                            if(rand()%5==0){
                                canMove= true;
                            }
                            FuelTank fuelTank = FuelTank(SPRITE_FUEL, 1, newShape, false, 2, spriteIndexList[0],canMove);
                            spriteIndexList.erase(spriteIndexList.begin());
                            short randomRow = 0;
                            short rowIndex = (gameScenario.getScreenHeader() - randomRow ) % 480;
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                            fuelTank.generate(boundaryGenerateInRow, randomRow);
                            fuelTankList.push_back(fuelTank);
                            WaterDriver::writePosition(videoFd, fuelTank.getPos(), SPRITE_FUEL,
                                                       fuelTank.getIndex());
                            break;
                        }
                        case 3:  case 5:  {    //for generate enemy plane
                                Shape newShape;
                                newShape.width = SPRITE_X;
                                newShape.length = SPRITE_Y;
                                bool canMove= false;
                                if(rand()%5==0){
                                    canMove= true;
                                }
                                EnemyPlane enemyPlane = EnemyPlane(SPRITE_BALLOON, 1, newShape, false, 2, spriteIndexList[0],
                                                                   canMove);
                                spriteIndexList.erase(spriteIndexList.begin());
                                short randomRow = 0;
                                short rowIndex = (gameScenario.getScreenHeader() - randomRow) % 480;
                                BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                                enemyPlane.generate(boundaryGenerateInRow, randomRow);
                                enemyList.push_back(enemyPlane);
                                WaterDriver::writePosition(videoFd, enemyPlane.getPos(), SPRITE_BALLOON,
                                                           enemyPlane.getIndex());
                                break;
                            }
                        printf("size: %d\n",spriteIndexList.size());
                    }
                }
            }
        }
    }
    return 0;
}

