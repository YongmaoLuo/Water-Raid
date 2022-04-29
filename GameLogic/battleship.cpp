//
// Created by Frank on 4/4/22.
//

#include "battleship.h"

void Battleship::checkIfHit(vector<Bullet> bullets, short &score) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].getPosition().x >= this->getPos().x &&
            bullets[i].getPosition().x <= (this->getPos().x + this->sp.width) && bullets[i].getPosition().y ==
                                                                                 (this->getPos().y +
                                                                                  this->sp.length)){
            this->hitPoint--;
            if(this->hitPoint == 0){
                this->setIsDestroy(true);
                score+=this->score;

            }
        }
    }
}

Battleship::Battleship(char type, char hitPoint, char id, const Shape &sp, bool isDestroy, const Position &pos,
                       char score) : Sprite(type, hitPoint, id, sp, isDestroy, pos), score(score) {}
