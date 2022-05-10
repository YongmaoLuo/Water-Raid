//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_ENEMY_PLANE_H
#define WATER_RAID_ENEMY_PLANE_H

#include "sprite.h"
#include "bullet.h"
#include <vector>

class EnemyPlane: public Sprite
{
private:
    char score;

public:
    void checkIfHit(vector<Bullet> &bullets,int videoFd, int &planeScore);

    EnemyPlane(char type, char hitPoint, const Shape &sp, bool isDestroy, char score, short index,bool canMove);

    static void fly(int videoFd,vector<EnemyPlane> &enemyList, vector<short> &spriteIndexList, GameScenario gameScenario);
};

#endif //WATER_RAID_ENEMY_PLANE_H
