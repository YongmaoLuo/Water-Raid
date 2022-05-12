//
// Created by Yongmao Luo on 4/6/22.
//

#include "airplane.h"

#include "driver.h"
#include <stdio.h>


#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define XBOX_BUTTON_B 305
#define XBOX_BUTTON_X 307
#define XBOX_BUTTON_A 304
#define XBOX_BUTTON_Y 308

#define SPRITE_BULLET 4
#define SPRITE_EXPLODE 5
#define SPRITE_FUEL 3
#define PLANE_LEFT 7
#define PLANE_RIGHT 8

#define MAXFUEL 75
#define MINFUEL 5

#define SHOOT_AUDIO 0
#define HIT_AUDIO 1
#define EXPLODE_AUDIO 2

bool Airplane::isCrashed(int videoFd, BoundaryInRow boundary) {
    // collide the boundary
    if(boundary.river2_left==0){
        if((boundary.river1_left>=pos.x-shape.width)||
                (boundary.river1_right<=pos.x+shape.width)){
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
        if((boundary.river2_left>pos.x&&
           boundary.river1_left>=pos.x-shape.width)||
                (boundary.river2_left>pos.x&&
           boundary.river1_right<=pos.x+shape.width)||
                (boundary.river2_left<pos.x&&
                boundary.river2_left>=pos.x-shape.width)||
                (boundary.river2_left<pos.x&&
                boundary.river2_right<=pos.x+shape.width)){
            printf("river1 left:%d",boundary.river1_left);
                printf("river1 right:%d",boundary.river1_right);
                printf("river2 left:%d",boundary.river2_left);
                printf("river2 right:%d",boundary.river2_right);
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
    return false;
}

bool Airplane::isCrashed(int videoFd,
                                 std::vector<EnemyPlane> enemyPlaneList,
                                 std::vector<Battleship> battleList){

    // collide the enemy planes
    for(int i=0;i<enemyPlaneList.size();i++){
        if(pos.y-shape.length<=enemyPlaneList[i].getPos().y+enemyPlaneList[i].getShape().length&&
        pos.y+shape.length>=enemyPlaneList[i].getPos().y-enemyPlaneList[i].getShape().length&&
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
        if(pos.y-shape.length<=battleList[i].getPos().y+battleList[i].getShape().length&&
        pos.y+shape.length>=battleList[i].getPos().y-battleList[i].getShape().length&&
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

void Airplane::addFuel(int videoFd,std::vector<FuelTank> &fuelTankList, std::vector<short> &spriteIndexList){
    for(int i=0;i<fuelTankList.size();i++){
        if(pos.y-shape.length<=fuelTankList[i].getPos().y+fuelTankList[i].getShape().length&&
                pos.y+shape.length>=fuelTankList[i].getPos().y-fuelTankList[i].getShape().length&&
           !(pos.x+shape.width<fuelTankList[i].getPos().x-fuelTankList[i].getShape().width)&&
           !(pos.x-shape.width>fuelTankList[i].getPos().x+fuelTankList[i].getShape().width)){
            // collide with fuelTank
            if(fuel+10<=MAXFUEL)
                fuel+=10;
            else
                fuel=MAXFUEL;
            WaterDriver::writeFuel(videoFd,fuel);
            Position tempPos;tempPos.y=0;
            // remove the fuelTank
            WaterDriver::writePosition(videoFd,tempPos,SPRITE_FUEL,fuelTankList[i].index);
            spriteIndexList.push_back(fuelTankList[i].getIndex());
            fuelTankList.erase(fuelTankList.begin()+i);
            break;
        }
    }
    WaterDriver::writeFuel(videoFd,this->fuel);
}

int Airplane::reduceFuel(int videoFd) {
    if(fuel>MINFUEL){
        fuel-=1;
        WaterDriver::writeFuel(videoFd,fuel);
        return 0;
    }else{
        return -1;
    }

}

void Airplane::fire(int xboxFd,int videoFd,vector<Bullet> &bulletList){

    if (xboxInput.code == XBOX_BUTTON_Y && xboxInput.value==1) {
        // press the button to emit bullet
        int numOfBullets=bulletList.size();
        if(numOfBullets==3)
            return;
        WaterDriver::playAudio(videoFd,SHOOT_AUDIO);
        char temp[4]; // 1, 2, 3
        memset(temp,0,sizeof(temp));
        if(numOfBullets>0){
            for(vector<Bullet>::iterator it=bulletList.begin();it!=bulletList.end();it++){
                temp[it->index]=1;
            }
        }
        

        for(int i=1;i<=3;i++){
            if(temp[i]==0){
                Shape sp;sp.width=1;sp.length=5;
                Position tempPos; tempPos.x=pos.x; tempPos.y=pos.y-sp.length*2;
                Bullet bullet=Bullet(SPRITE_BULLET,sp,tempPos);
                bullet.index=i;
                bulletList.push_back(bullet);
                WaterDriver::writePosition(videoFd,tempPos,SPRITE_BULLET,i);
                break;
            }
        }
    }

}

Airplane::Airplane(char type, char fuel, Position pos, Shape shape, char scores):
type(type), fuel(fuel), pos(pos),shape(shape),scores(scores){
    //mutexPos= PTHREAD_MUTEX_INITIALIZER;
    buttonBOn=buttonXOn= false;
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



void Airplane::receiveFromXbox(int xboxFd) {

    int flags= fcntl(xboxFd,F_GETFL,0);
    fcntl(xboxFd,F_SETFL,flags|O_NONBLOCK);
    read(xboxFd, &xboxInput, 24);

}

void Airplane::calPos(int videoFd) {
    if (xboxInput.code == XBOX_BUTTON_X && xboxInput.value==1) {
        buttonXOn=true;
    } else if(xboxInput.code == XBOX_BUTTON_X && xboxInput.value==0){
        buttonXOn=false;
    }else if (xboxInput.code == XBOX_BUTTON_B && xboxInput.value==1) {
        buttonBOn=true;
    }else if(xboxInput.code == XBOX_BUTTON_B && xboxInput.value==0){
        buttonBOn=false;
    }

    if(buttonXOn){
        Position tempPos = getPos();
        tempPos.x -= 2;
        setPos(tempPos);
        WaterDriver::writePosition(videoFd,pos,PLANE_LEFT,0);
    }else if(buttonBOn){
        Position tempPos = getPos();
        tempPos.x += 2;
        setPos(tempPos);
        WaterDriver::writePosition(videoFd,pos,PLANE_RIGHT,0);
    }else{
        WaterDriver::writePosition(videoFd,pos,type,0);
    }
}

void Airplane::addScore(int videoFd,int score) {
    int tempScores=this->scores+score;
    if(tempScores>999){
        this->scores=999;
    }else{
        this->scores=tempScores;
    }
    WaterDriver::writeScore(videoFd, this->scores);
}

bool Airplane::startGame() {
    if(xboxInput.code==XBOX_BUTTON_A&&xboxInput.value==1){
        return true;
    }
    return false;
}