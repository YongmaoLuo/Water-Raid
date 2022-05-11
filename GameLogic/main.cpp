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
#define SPRITE_BALLOON 6

// center coordinate related to upper left corner
#define SPRITE_X 14
#define SPRITE_Y 14

#define MINIMUM_RIVER_WIDTH 50
#define MAXFUEL 75

#define SHOOT_AUDIO 2
#define HIT_AUDIO 1
#define EXPLODE_AUDIO 0

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
        tempPos.y = (400 << 1) + 1;
        Shape tempShape;
        tempShape.width = SPRITE_X;
        tempShape.length = SPRITE_Y;
        airplane = Airplane(SPRITE_PLANE, MAXFUEL, tempPos, tempShape, 0);
        WaterDriver::writeScore(videoFd,0);

        // the plane enter the screen
        while (airplane.getPos().y > (300 << 1) + 1) {
            WaterDriver::writePosition(videoFd, airplane.getPos(), SPRITE_PLANE, 0);
            tempPos = airplane.getPos();
            tempPos.y -= 2;
            airplane.setPos(tempPos);
            usleep(5000);
        }

        // wait for user to press button A so we can start the game
        while(!airplane.startGame()){
            airplane.receiveFromXbox(xboxFd, videoFd);
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
                airplane.receiveFromXbox(xboxFd, videoFd);
                airplane.calPos(videoFd);

                // determine if the plane has crashed
                // plane is always located at y=300
                BoundaryInRow boundaryAheadOfPlane = gameScenario.boundaries[
                        (gameScenario.getScreenHeader() -300+480 + SPRITE_Y) % 480];

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
                    if (temp == -1)
                        break;
                }

                // add survival scores
                if(counterSurvival>=2){
                    counterSurvival=0;
                    airplane.scores+=1;
                    WaterDriver::writeScore(videoFd,airplane.scores);
                }

                //bullet fly
                Bullet::fly(videoFd,bulletList);


                //enemy plane fly
                for (int i = 0; i < enemyList.size(); i++)
                {
                    enemyList[i].pos.y += 2;
                    if(enemyList[i].pos.y >= (400 << 1) + 1){
                        enemyList[i].disappear();

                        spriteIndexList.push_back(enemyList[i].getIndex());
                        enemyList.erase(enemyList.begin()+i);
                    } else if(timeToMove){

                        enemyList[i].move(gameScenario.boundaries[
                                                  (gameScenario.getScreenHeader() - int((enemyList[i].getPos().y -1) >> 1) +
                                                   480 + SPRITE_Y) % 480], 2);
                    }
                    WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
                                               enemyList[i].getIndex());
                }

                //battleship move
                for (int i = 0; i < battleList.size(); i++)
                {
                    battleList[i].pos.y += 2;
                    if(battleList[i].pos.y >=  (400 << 1) + 1){
                        battleList[i].disappear();
                        spriteIndexList.push_back(battleList[i].getIndex());
                        battleList.erase(battleList.begin()+i);
                    } else if(timeToMove){
                        battleList[i].move(gameScenario.boundaries[(gameScenario.getScreenHeader() - int((battleList[i].getPos().y - 1) >> 1) + 480 + SPRITE_Y) % 480],
                                           2);

                    }
                    WaterDriver::writePosition(videoFd, battleList[i].getPos(), SPRITE_BATTLE,
                                               battleList[i].getIndex());
                }

                //fuel tank move
                for (int i = 0; i < fuelTankList.size(); i++)
                {
                    fuelTankList[i].pos.y += 2;
                    if(fuelTankList[i].pos.y >=  (400 << 1) + 1){
                        fuelTankList[i].disappear();
                        spriteIndexList.push_back(fuelTankList[i].getIndex());
                        fuelTankList.erase(fuelTankList.begin()+i);
                    } else if(timeToMove){
                        fuelTankList[i].move(gameScenario.boundaries[(gameScenario.getScreenHeader() - int((fuelTankList[i].getPos().y - 1) >> 1) + 480 + SPRITE_Y) % 480],
                                             2);
                    }
                    WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
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
                        WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
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
                    switch (rand() % 3) {
                        case 0: {    //for generate enemy plane
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
                            enemyList.push_back(enemyPlane);
                            short randomRow = 16;
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
                            bool canMove= true;
                            if(rand()%5==0){
                                canMove= false;
                            }
                            Battleship battleship = Battleship(SPRITE_BATTLE, 2, newShape, false, 3, spriteIndexList[0],canMove);
                            spriteIndexList.erase(spriteIndexList.begin());
                            battleList.push_back(battleship);
                            short randomRow = 16;
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
                            bool canMove= false;
                            if(rand()%5==0){
                                canMove= true;
                            }
                            FuelTank fuelTank = FuelTank(SPRITE_FUEL, 1, newShape, false, 2, spriteIndexList[0],canMove);
                            spriteIndexList.erase(spriteIndexList.begin());
                            fuelTankList.push_back(fuelTank);
                            short randomRow = 16;
                            short rowIndex = (gameScenario.getScreenHeader() - randomRow + 480 + SPRITE_Y) % 480;
                            BoundaryInRow boundaryGenerateInRow = gameScenario.boundaries[rowIndex];
                            fuelTank.generate(boundaryGenerateInRow, randomRow);
                            WaterDriver::writePosition(videoFd, fuelTank.getPos(), SPRITE_FUEL,
                                                       fuelTank.getIndex());
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

