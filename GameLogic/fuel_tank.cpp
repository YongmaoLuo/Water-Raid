//
// Created by Frank on 4/4/22.
//

// Fuel Tank Object
// Copyright (C) 2022  Yongmao Luo,Frank Wang

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "fuel_tank.h"
#include "driver.h"

#define HIT_AUDIO 1

void FuelTank::checkIfHit(vector<Bullet> &bullets,int videoFd) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].getPosition().x >= (this->getPos().x - this->sp.width) &&
            bullets[i].getPosition().x <= (this->getPos().x + this->sp.width) && (bullets[i].getPosition().y - bullets[i].getSp().length)<=
                                                                                 (this->getPos().y +
                                                                                  this->sp.length)){
            this->hitPoint--;
            WaterDriver::playAudio(videoFd,HIT_AUDIO);
            bullets[i].setCrash();
            WaterDriver::writePosition(videoFd,bullets[i].getPosition(),bullets[i].getType(),bullets[i].getIndex());
            bullets.erase(bullets.begin()+i);
            if(this->hitPoint == 0){
                this->setIsDestroy(true);
            }
        }
    }
}

//void FuelTank::movement(int videoFd, vector<FuelTank> &fuelTankList, vector<short> &spriteIndexList,
//                        GameScenario gameScenario) {
//    for (int i = 0; i < fuelTankList.size(); i++)
//    {
//        fuelTankList[i].pos.y += 2;
//        if(fuelTankList[i].pos.y ==  (480 << 1) + 1){
//            fuelTankList[i].disappear();
//            WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
//                                       fuelTankList[i].getIndex());
//            spriteIndexList.push_back(fuelTankList[i].getIndex());
//            fuelTankList.erase(fuelTankList.begin()+i);
//        } else{
//            fuelTankList[i].move(gameScenario.boundaries[(gameScenario.getScreenHeader() - fuelTankList[i].getPos().y + 480 + SPRITE_Y) % 480], 5);
//            WaterDriver::writePosition(videoFd, fuelTankList[i].getPos(), SPRITE_FUEL,
//                                       fuelTankList[i].getIndex());
//        }
//    }
//}

FuelTank::FuelTank(char type, char hitPoint, const Shape &sp, bool isDestroy,
                   char fuelVolume, short index,bool canMove) : Sprite(type, hitPoint, sp, isDestroy, index,canMove), fuelVolume(fuelVolume) {}
