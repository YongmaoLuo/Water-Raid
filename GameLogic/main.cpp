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


// type of different sprite
#define SPRITE_PLANE 0
#define SPRITE_HELI 1
#define SPRITE_BATTLE 2
#define SPRITE_FUEL 3
#define SPRITE_BULLET 4
#define SPRITE_BALLOON 6

// center coordinate related to upper left corner
#define SPRITE_X 16
#define SPRITE_Y 16

// the struct used to store different kinds of sprites

int main(){
    vector<Bullet> bulletList;
    vector<Battleship> battleList;
    vector<EnemyPlane> enemyList;
    vector<FuelTank> fuelTankList;
    Position tempPos; tempPos.x=320; tempPos.y=(480<<1)+1;
    Shape tempShape; tempShape.width=16;tempShape.length=16;
    Airplane airplane = Airplane(SPRITE_PLANE,80,tempPos,tempShape,0);
    GameScenario gameScenario(30,640,60);


    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[]="/dev/vga_ball";
    int videoFd,xboxFd;

    if((xboxFd== open(xbox,O_RDWR))==-1){
        fprintf(stderr,"could not open %s\n",xbox);
    }

    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr,"could not open %s\n", waterVideo);
        return -1;
    }


    double duration=1/gameScenario.getFrequency();
    clock_t execute=clock();
    clock_t reduceFuelClock=clock();
    gameScenario.setChangeClock();
    while(1){
        WaterDriver::initBackground();

        // wait the button on the xbox controller to be pressed
        /* wait to be finished */


        // the plane enter the screen
        while(airplane.getPos().y>(300<<1)+1){
            WaterDriver::writePosition(videoFd,airplane.getPos(),SPRITE_PLANE,0);
            tempPos=airplane.getPos();
            tempPos.y-=2;
            airplane.setPos(tempPos);
            usleep(40000);
        }
        while(1){
            if(double(clock()-execute)/CLOCKS_PER_SEC>=duration){
                execute=clock();
                gameScenario.updateBackground(videoFd);

                //receive control signal from xbox
                airplane.receivePos(xboxFd,videoFd,xbox);

                // determine if the plane has crashed
                // plane is always located at y=300
                BoundaryInRow boundaryAheadOfPlane=gameScenario.boundaries[gameScenario.getScreenHeader()+180-SPRITE_Y];
                bool isCrashed=airplane.isCrashed(videoFd,boundaryAheadOfPlane,enemyList,battleList);
                if(isCrashed)
                    break;
                // if the plane bumped into the fuel tank
                airplane.addFuel(videoFd,fuelTankList);

                // check if the airplane is about to emit a bullet
                airplane.fire(videoFd,bulletList);

                // reduce fuel
                if(double(clock()-reduceFuelClock)/CLOCKS_PER_SEC>=1){
                    airplane.reduceFuel(videoFd);
                }

            }

        }
    }



    return 0;
}

