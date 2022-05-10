//
// Created by Frank on 4/4/22.
//

#include "battleship.h"
#include "driver.h"

void Battleship::checkIfHit(vector<Bullet> &bullets,int videoFd, int &planeScore) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].getPosition().x >= (this->getPos().x - this->sp.width) &&
            bullets[i].getPosition().x <= (this->getPos().x + this->sp.width) && (bullets[i].getPosition().y - bullets[i].getSp().length) <=
                                                                                 (this->getPos().y +
                                                                                  this->sp.length)){
            this->hitPoint--;
            bullets[i].setCrash();
            if(this->hitPoint == 0){
                this->setIsDestroy(true);
                planeScore+=score;
                WaterDriver::writeScore(videoFd,planeScore);
            }
        }
    }
}

Battleship::Battleship(char type, char hitPoint, const Shape &sp, bool isDestroy,
                       char score, short index,bool canMove) : Sprite(type, hitPoint, sp, isDestroy, index,canMove), score(score) {}
