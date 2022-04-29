//
// Created by Frank on 4/4/22.
//
#include "fuel_tank.h"

void FuelTank::checkIfHit(vector<Bullet> bullets, short &fuel) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].getPosition().x >= this->getPos().x &&
            bullets[i].getPosition().x <= (this->getPos().x + this->sp.width) && bullets[i].getPosition().y ==
                                                                                 (this->getPos().y +
                                                                                  this->sp.length)){
            this->hitPoint--;
            if(this->hitPoint == 0){
                this->setIsDestroy(true);
                airplane.addFuel(this->fuelVolume);
            }
        }
    }
}

FuelTank::FuelTank(char type, char hitPoint, char id, const Shape &sp, bool isDestroy, const Position &pos,
                   char fuelVolume) : Sprite(type, hitPoint, id, sp, isDestroy, pos), fuelVolume(fuelVolume) {}
