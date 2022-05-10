//
// Created by Frank on 4/4/22.
//
#include "fuel_tank.h"

void FuelTank::checkIfHit(vector<Bullet> &bullets, short &fuel) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].getPosition().x >= (this->getPos().x - this->sp.width) &&
            bullets[i].getPosition().x <= (this->getPos().x + this->sp.width) && (bullets[i].getPosition().y - bullets[i].getSp().length)<=
                                                                                 (this->getPos().y +
                                                                                  this->sp.length)){
            this->hitPoint--;
            bullets[i].setCrash();
            if(this->hitPoint == 0){
                this->setIsDestroy(true);
                fuel = fuel + this->fuelVolume;
            }
        }
    }
}

void FuelTank::movement(int videoFd, vector<FuelTank> &fuelTankList, vector<short> &spriteIndexList,
                        GameScenario gameScenario) {
    for (int i = 0; i < fuelTankList.size(); i++)
    {
        fuelTankList[i].pos.y += 2;
        if(fuelTankList[i].pos.y ==  (480 << 1) + 1){
            fuelTankList[i].disappear();
            WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
                                       fuelTankList[i].getIndex());
            spriteIndexList.push_back(fuelTankList[i].getIndex());
            fuelTankList.erase(fuelTankList.begin()+i);
        } else{
            fuelTankList[i].move(gameScenario.boundaries[(gameScenario.getScreenHeader() - fuelTankList[i].getPos().y + 480 + SPRITE_Y) % 480], 5);
            WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
                                       fuelTankList[i].getIndex());
        }
    }
}

FuelTank::FuelTank(char type, char hitPoint, const Shape &sp, bool isDestroy,
                   char fuelVolume, short index) : Sprite(type, hitPoint, sp, isDestroy, index), fuelVolume(fuelVolume) {}
