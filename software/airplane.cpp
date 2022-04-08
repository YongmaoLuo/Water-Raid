//
// Created by Yongmao Luo on 4/6/22.
//

#include "airplane.h"

bool Airplane::isCrashed(BoundaryInRow boundary, std::vector<EnemyPlane> enemyPlanes, std::vector<Battleship> battles){
    // collide the boundary
    if(boundary.river2_left!=0)
        if(pos.x<=boundary.river1_left|| pos.x+shape.width>=boundary.river1_right&& this->pos.x<boundary.river2_left||
           pos.x+shape.width>=boundary.river2_right){
            isCrash=true;
            pos.y|=~0x1; // set y[0] to be 1
        }
        else
        if(pos.x<=boundary.river1_left|| pos.x+shape.width>=boundary.river1_right){
            isCrash=true;
            this->pos.y|=~0x1; // set y[0] to be 1
            return isCrash;
        }

    // collide the enemy planes
    for(int i=0;i<enemyPlanes.size();i++){
        if(pos.y>=enemyPlanes[i].getPos().y&&pos.x<=enemyPlanes[i].getPos().x+enemyPlanes[i].sp.width&&pos.x>=enemyPlanes[i].getPos().x){
            isCrash=true;
            pos.y|=~0x1; // set y[0] to be 1
            return isCrash;
        }
    }
    //collide the battleships
    for(int i=0;i<battles.size();i++){
        if(pos.y>=battles[i].getPos().y&&pos.x<=battles[i].getPos().x+battles[i].sp.width&&pos.x>=battles[i].getPos().x){
            isCrash=true;
            pos.y|=~0x1; // set y[0] to be 1
            return isCrash;
        }
    }
    return isCrash;
}

void Airplane::addScore(short score){
    this->scores += score;
}

void Airplane::addFuel(char fuel){
    this->fuel += fuel;
}

void Airplane::Move(int speed) {
    pos.x+=speed*0.01;
}

void Airplane::Fire(Bullet bullet){
    bullet.setPosition(pos);
}

Airplane::Airplane(char type, char fuel, Position pos, Shape shape, char scores, bool isCrash): type(type), fuel(fuel), pos(pos),shape(shape),scores(scores),isCrash(isCrash){}