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

void Battleship::movement(int videoFd, vector<Battleship> &battleList, vector<short> &spriteIndexList,
                          GameScenario gameScenario) {
    for (int i = 0; i < battleList.size(); i++)
    {
        battleList[i].pos.y += 2;
        if(battleList[i].pos.y <=  (480 << 1) + 1){
            battleList[i].disappear();
            WaterDriver::writePosition(videoFd, battleList[i].getPos(), SPRITE_BATTLE,
                                       battleList[i].getIndex());
            spriteIndexList.push_back(battleList[i].getIndex());
            battleList.erase(battleList.begin()+i);
        } else{
            battleList[i].move(gameScenario.boundaries[(gameScenario.getScreenHeader() - battleList[i].getPos().y + 480 + SPRITE_Y) % 480], 5);
            WaterDriver::writePosition(videoFd, battleList[i].getPos(), SPRITE_BATTLE,
                                       battleList[i].getIndex());
        }
    }
}
Battleship::Battleship(char type, char hitPoint, const Shape &sp, bool isDestroy,
                       char score, short index) : Sprite(type, hitPoint, sp, isDestroy, index), score(score) {}
