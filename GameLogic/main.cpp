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
#define SPRITE_EXPLODE 5
#define SPRITE_BALLOON 6

// center coordinate related to upper left corner
#define SPRITE_X 16
#define SPRITE_Y 16

int main(){
//    Shape bulletSP = {2, 5};
//    Position bulletPOS = {10, 10};
//
//    Bullet *bullets = new Bullet[MAX_BULLET_NUM] {
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//    };
//    vector<Bullet> vBullet(bullets, bullets + 5);
//
//    vBullet[0].fly();
//    vBullet[1].fly();
//    vBullet[2].fly();
//
//    Shape btsSP = {5, 5};
//    Position btsPOS = {5, 4};
//    Battleship bts = Battleship(3, 3, 0, btsSP, false, btsPOS, 3);

    Airplane airplane = Airplane();
    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[]="/dev/vga_ball";
    int videoFd;
//    bts.checkIfHit(vBullet, airplane);

//    printf("%s\n", bts.getIsDestroy());
//    while(1){
//        airplane.receivePos(inputDevice);
//        Position tempPos= airplane.getPos();
//        printf("x: %d\n",tempPos.x);
//        printf("y: %d\n",tempPos.y);
//        printf("\n");
//    }
    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr,"could not open %s\n", waterVideo);
        return -1;
    }
    GameScenario gameScenario(30,640,60);

    double duration=1/gameScenario.getFrequency();
    clock_t execute=clock();
    gameScenario.setChangeClock();
    while(1){
        WaterDriver::initBackground();

        // wait the button on the xbox controller to be pressed
        /* wait to be finished */

        // the plane enter the screen
        Position tempPos; tempPos.x=320; tempPos.y=(480<<1)+1;
        airplane.setPos(tempPos);
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
                // determine if the plane has crashed
                // plane is always located at y=300
                BoundaryInRow boundaryAheadOfPlane=gameScenario.boundaries[gameScenario.getScreenHeader()+180-SPRITE_Y];
                if(boundaryAheadOfPlane.river2_left==0){
                    if(boundaryAheadOfPlane.river1_left>=airplane.getPos().x||
                       boundaryAheadOfPlane.river1_right<=airplane.getPos().x){
                        // crashed
                        // plane disappear
                        tempPos.y=0;
                        tempPos.x=0;
                        airplane.setPos(tempPos);
                        WaterDriver::writePosition(videoFd,airplane.getPos(),SPRITE_PLANE,0);
                        // create explosion effect

                        break;
                    }
                }
            }

        }
    }



    return 0;
}

