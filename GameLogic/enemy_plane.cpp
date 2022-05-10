//
// Created by Frank on 4/4/22.
//

#include "enemy_plane.h"
#include "bullet.h"


void EnemyPlane::checkIfHit(vector<Bullet> &bullets, short &score) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].getPosition().x >= (this->getPos().x - this->sp.width)   &&
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

void EnemyPlane::fly(int videoFd,vector<EnemyPlane> &enemyList, vector<short> &spriteIndexList, GameScenario gameScenario) {
    for (int i = 0; i < enemyList.size(); i++)
    {
        enemyList[i].pos.y += 2;
        if(enemyList[i].pos.y <= (480 << 1) + 1){
            enemyList[i].disappear();
            WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
                                       enemyList[i].getIndex());
            spriteIndexList.push_back(enemyList[i].getIndex());
            enemyList.erase(enemyList.begin()+i);
        } else{
            enemyList[i].move(gameScenario.boundaries[(gameScenario.getScreenHeader() -enemyList[i].getPos().y + 480 + SPRITE_Y) % 480], 5);
            WaterDriver::writePosition(videoFd, enemyList[i].getPos(), SPRITE_HELI,
                                       enemyList[i].getIndex());
        }
    }
}

EnemyPlane::EnemyPlane(char type, char hitPoint, const Shape &sp, bool isDestroy,
                       char score, short index) : Sprite(type, hitPoint, sp, isDestroy, index), score(score) {}

