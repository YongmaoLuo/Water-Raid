//
// Created by Yongmao Luo on 4/6/22.
//

#include "airplane.h"
#include <pthread.h>
#include "driver.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define XBOX_BUTTON_B 305
#define XBOX_BUTTON_X 307
#define XBOX_BUTTON_A 304
#define XBOX_BUTTON_Y 308

#define SPRITE_BULLET 4
#define SPRITE_EXPLODE 5
#define SPRITE_FUEL 3

bool Airplane::isCrashed(int videoFd,BoundaryInRow boundary,
                                 std::vector<EnemyPlane> enemyPlaneList,
                                 std::vector<Battleship> battleList){

    // collide the boundary
    if(boundary.river2_left==0){
        if(boundary.river1_left>=pos.x-shape.width||
           boundary.river1_right<=pos.x+shape.width){
            // plane disappear
            Position tempPos;
            tempPos.y=0;
            tempPos.x=0;
            WaterDriver::writePosition(videoFd,tempPos,type,0);
            // create explosion effect
            WaterDriver::writePosition(videoFd,pos,SPRITE_EXPLODE,0);
            pos=tempPos;
            return true;
        }
    }else{
        if(boundary.river2_left>pos.x&&
           boundary.river1_left>=pos.x-shape.width||
           boundary.river2_left>pos.x&&
           boundary.river1_right<=pos.x+shape.width||
           boundary.river2_left>=pos.x-shape.width||
           boundary.river2_right<=pos.x+shape.width){
            // crashed when there are 2 rivers
            // plane disappear
            Position tempPos;
            tempPos.y=0;
            tempPos.x=0;
            WaterDriver::writePosition(videoFd,tempPos,type,0);
            // create explosion effect
            WaterDriver::writePosition(videoFd,pos,SPRITE_EXPLODE,0);
            pos=tempPos;
            return true;
        }
    }

    // collide the enemy planes
    for(int i=0;i<enemyPlaneList.size();i++){
        if(pos.y-shape.length>=enemyPlaneList[i].getPos().y+enemyPlaneList[i].getShape().length&&
        !(pos.x+shape.width<enemyPlaneList[i].getPos().x-enemyPlaneList[i].getShape().width)&&
        !(pos.x-shape.width>enemyPlaneList[i].getPos().x+enemyPlaneList[i].getShape().width)){
            Position tempPos;
            tempPos.y=0;
            tempPos.x=0;
            WaterDriver::writePosition(videoFd,tempPos,type,0);
            // create explosion effect
            WaterDriver::writePosition(videoFd,pos,SPRITE_EXPLODE,0);
            pos=tempPos;
            return true;
        }
    }
    //collide the battleships
    for(int i=0;i<battleList.size();i++){
        if(pos.y-shape.length>=battleList[i].getPos().y+battleList[i].getShape().length&&
           !(pos.x+shape.width<battleList[i].getPos().x-battleList[i].getShape().width)&&
           !(pos.x-shape.width>battleList[i].getPos().x+battleList[i].getShape().width)){
            Position tempPos;
            tempPos.y=0;
            tempPos.x=0;
            WaterDriver::writePosition(videoFd,tempPos,type,0);
            // create explosion effect
            WaterDriver::writePosition(videoFd,pos,SPRITE_EXPLODE,0);
            pos=tempPos;
            return true;
        }
    }

    return false;
}

void Airplane::addFuel(int videoFd,std::vector<FuelTank> &fuelTankList){
    for(int i=0;i<fuelTankList.size();i++){
        if(pos.y-shape.length>=fuelTankList[i].getPos().y+fuelTankList[i].getShape().length&&
           !(pos.x+shape.width<fuelTankList[i].getPos().x-fuelTankList[i].getShape().width)&&
           !(pos.x-shape.width>fuelTankList[i].getPos().x+fuelTankList[i].getShape().width)){
            // collide with fuelTank
            if(fuel+10<=80)
                fuel+=10;
            else
                fuel=80;
            WaterDriver::writeFuel(videoFd,fuel);
            Position tempPos;tempPos.y=0;
            // remove the fuelTank
            WaterDriver::writePosition(videoFd,tempPos,SPRITE_FUEL,fuelTankList[i].index);
            fuelTankList.erase(fuelTankList.begin()+i);
            break;
        }
    }
    this->fuel += fuel;
    WaterDriver::writeFuel(videoFd,this->fuel);
}

void Airplane::reduceFuel(int videoFd) {
    fuel-=1;
    WaterDriver::writeFuel(videoFd,fuel);
}

void Airplane::fire(int xboxFd,int videoFd,vector<Bullet> &bulletList){

    inputEvent tempInput;
    read(xboxFd, &tempInput, 24);
    if (tempInput.code == XBOX_BUTTON_Y && tempInput.value==1) {
        // press the button to emit bullet
        int numOfBullets;
        if(numOfBullets=bulletList.size()==3)
            return;
        char temp[3]; // 1, 2, 3
        if(numOfBullets>0){
            memset(temp,0,sizeof(temp));
            for(vector<Bullet>::iterator it=bulletList.begin();it!=bulletList.end();it++){
                temp[it->index-1]=1;
            }
        }

        for(int i=0;i<3;i++){
            if(temp[i]=0){
                Shape sp;sp.width=1;sp.length=5;
                Position tempPos; tempPos.x=pos.x; tempPos.y=pos.y-sp.length;
                Bullet bullet=Bullet(SPRITE_BULLET,sp,tempPos);
                bulletList.push_back(bullet);
                WaterDriver::writePosition(videoFd,tempPos,SPRITE_BULLET,i+1);
                break;
            }
        }
    }

}

Airplane::Airplane(char type, char fuel, Position pos, Shape shape, char scores):
type(type), fuel(fuel), pos(pos),shape(shape),scores(scores){
    //mutexPos= PTHREAD_MUTEX_INITIALIZER;
}

Position Airplane::getPos() {
    //pthread_mutex_lock(&mutexPos);
    Position result=pos;
    //pthread_mutex_unlock(&mutexPos);
    return result;
}

void Airplane::setPos(Position change){
    //pthread_mutex_lock(&mutexPos);
    pos=change;
    //pthread_mutex_unlock(&mutexPos);
    return;
}

int Airplane::receivePos(int xboxFd, int videoFd, const char inputDevice[]) {

    inputEvent tempInput;
    read(xboxFd, &tempInput, 24);
    if (tempInput.code == XBOX_BUTTON_X && tempInput.value==1) {
        Position tempPos = getPos();
        tempPos.x -= 1;
        setPos(tempPos);
    } else if (tempInput.code == XBOX_BUTTON_B && tempInput.value==1) {
        Position tempPos = getPos();
        tempPos.x += 1;
        setPos(tempPos);
        return 0;
    }
    WaterDriver::writePosition(videoFd,pos,type,0);
}