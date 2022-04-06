//
// Created by Frank on 4/1/22.
//

#ifndef WATER_RAID_ENEMY_PLANE_H
#define WATER_RAID_ENEMY_PLANE_H

#include "sprite.h"

class EnemyPlane: public Sprite
{
private:
    char score;

    void checkIfHit(vector<Bullet> bullets, Airplane airplane);
};

#endif //WATER_RAID_ENEMY_PLANE_H
