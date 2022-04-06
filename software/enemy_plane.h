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

public:
    void checkIfHit(vector<Bullet> bullets, Airplane airplane);

    EnemyPlane(char type, char hitPoint, char id, const Shape &sp, bool isDestroy, const Position &pos, char score);
};

#endif //WATER_RAID_ENEMY_PLANE_H
