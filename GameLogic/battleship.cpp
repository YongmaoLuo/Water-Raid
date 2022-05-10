//
// Created by Frank on 4/4/22.
//

#include "battleship.h"

void Battleship::checkIfHit(vector<Bullet> &bullets, short &score) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].getPosition().x >= (this->getPos().x - this->sp.width) &&
            bullets[i].getPosition().x <= (this->getPos().x + this->sp.width) && (bullets[i].getPosition().y - bullets[i].getSp().length) <=
                                                                                 (this->getPos().y +
                                                                                  this->sp.length)){
            this->hitPoint--;
            bullets[i].setCrash();
            if(this->hitPoint == 0){
                this->setIsDestroy(true);
                score+=this->score;

            }
        }
    }
}

Battleship::Battleship(char type, char hitPoint, const Shape &sp, bool isDestroy,
                       char score, short index) : Sprite(type, hitPoint, sp, isDestroy, index), score(score) {}
