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

#define MAXFUEL 75

// the struct used to store different kinds of sprites

int main(){
    vector<Bullet> bulletList;
    vector<Battleship> battleList;
    vector<EnemyPlane> enemyList;
    vector<FuelTank> fuelTankList;
    Airplane airplane;

    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[]="/dev/vga_ball";
    int videoFd,xboxFd;

    if((xboxFd= open(xbox,O_RDWR))==-1){
        fprintf(stderr,"could not open %s\n",xbox);
        return -1;
    }

    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr,"could not open %s\n", waterVideo);
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
        tempPos.y = (480 << 1) + 1;
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
                airplane.receivePos(xboxFd, videoFd);

                // determine if the plane has crashed
                // plane is always located at y=300
                BoundaryInRow boundaryAheadOfPlane = gameScenario.boundaries[
                        (gameScenario.getScreenHeader() + 180 - SPRITE_Y) % 480];

//                printf("river1 left:%d",boundaryAheadOfPlane.river1_left);
//                printf("river1 right:%d",boundaryAheadOfPlane.river1_right);
//                printf("river2 left:%d",boundaryAheadOfPlane.river2_left);
//                printf("river2 right:%d",boundaryAheadOfPlane.river2_right);

//                for(int i=0;i<4;i++){
//                    printf("boundary1 left:%d\n",boundaryAheadOfPlane.river1_left);
//                    printf("")
//                }
                isCrashed = airplane.isCrashed(videoFd, boundaryAheadOfPlane);
                if (isCrashed)
                    break;
//                // if the plane bumped into the fuel tank
//                airplane.addFuel(videoFd,fuelTankList);
//
//                // check if the airplane is about to emit a bullet
//                airplane.fire(xboxFd,videoFd,bulletList);
//
//                // reduce fuel
                if ((clock() - reduceFuelClock) / CLOCKS_PER_SEC >= 1) {
                    reduceFuelClock = clock();
                    int temp = airplane.reduceFuel(videoFd);
                    if (temp == -1)
                        break;
               }

            }

        }
    }



    return 0;
}

