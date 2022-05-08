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

// the struct used to store different kinds of sprites
struct {
    struct child{
        short type;
        union{
            Bullet *bullet;
            Battleship *battleship;
            FuelTank *fuelTank;
            EnemyPlane *enemyPlane;
        }u;
    };

    std::vector<child> children;
    int maximumSize;

    void addChild(Bullet *bullet){
        child ch;
        ch.type=SPRITE_BULLET;
        ch.u.bullet=bullet;
        children.push_back(ch);
    }

    void addChild(Battleship *battleship){
        child ch;
        ch.type=SPRITE_BATTLE;
        ch.u.battleship=battleship;
        children.push_back(ch);
    }

    void addChild(FuelTank *fuelTank){
        child ch;
        ch.type=SPRITE_FUEL;
        ch.u.fuelTank=fuelTank;
        children.push_back(ch);
    }

    void addChild(EnemyPlane *enemyPlane){
        child ch;
        ch.type=SPRITE_HELI;
        ch.u.enemyPlane=enemyPlane;
        children.push_back(ch);
    }
}SpritePointerList;

int main(){
    SpritePointerList.maximumSize=8; // we can show at most 9 sprites containing the plane
    Position tempPos; tempPos.x=320; tempPos.y=(480<<1)+1;
    Shape tempShape; tempShape.width=16;tempShape.length=16;
    Airplane airplane = Airplane(SPRITE_PLANE,80,tempPos,tempShape,0);
    GameScenario gameScenario(30,640,60);


    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[]="/dev/vga_ball";
    int videoFd;

    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr,"could not open %s\n", waterVideo);
        return -1;
    }


    double duration=1/gameScenario.getFrequency();
    clock_t execute=clock();
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
                // determine if the plane has crashed
                // plane is always located at y=300
                BoundaryInRow boundaryAheadOfPlane=gameScenario.boundaries[gameScenario.getScreenHeader()+180-SPRITE_Y];
                if(boundaryAheadOfPlane.river2_left==0){
                    if(boundaryAheadOfPlane.river1_left>=airplane.getPos().x-SPRITE_X||
                       boundaryAheadOfPlane.river1_right<=airplane.getPos().x+SPRITE_X){
                        // crashed8
                        // plane disappear
                        tempPos.y=0;
                        tempPos.x=0;
                        WaterDriver::writePosition(videoFd,tempPos,SPRITE_PLANE,0);
                        // create explosion effect
                        WaterDriver::writePosition(videoFd,airplane.getPos(),SPRITE_EXPLODE,0);
                        airplane.setPos(tempPos);
                        break;
                    }
                }else{
                    if(boundaryAheadOfPlane.river2_left>airplane.getPos().x&&
                    boundaryAheadOfPlane.river1_left>=airplane.getPos().x-SPRITE_X||
                       boundaryAheadOfPlane.river1_right<=airplane.getPos().x+SPRITE_X||
                       boundaryAheadOfPlane.river2_left>)
                }
            }

        }
    }



    return 0;
}

