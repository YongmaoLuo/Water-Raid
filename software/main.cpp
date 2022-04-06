//
// Created by Yongmao Luo on 4/1/22.
//

#include "bullet.h"
#include "bullet.cpp"

#include "game_scenario.h"

#include "battleship.h"
#include "airplane.h"
#include "enemy_plane.h"
#include "fuel_tank.h"
#include "sprite.h"


#define MAX_SPRITES_NUM 8
#define MAX_BULLET_NUM 5

int main(){
    Shape bulletSP = {2, 5};
    Position bulletPOS = {10, 10};

    Bullet *bullets = new Bullet[MAX_BULLET_NUM] {
            Bullet(1, bulletSP, false, bulletPOS),
            Bullet(1, bulletSP, false, bulletPOS),
            Bullet(1, bulletSP, false, bulletPOS),
            Bullet(1, bulletSP, false, bulletPOS),
            Bullet(1, bulletSP, false, bulletPOS),
    };
    vector<Bullet> vBullet(bullets, bullets + 5);

    vBullet[0].fly();
    vBullet[1].fly();
    vBullet[2].fly();

    Shape btsSP = {5, 5};
    Position btsPOS = {5, 4};
    Battleship bts = Battleship(3, 3, 0, btsSP, false, btsPOS, 3);

    Airplane airplane = Airplane();
    bts.checkIfHit(vBullet, airplane);

    printf("%s\n", bts.getIsDestroy());
    printf("%d\n", airplane.getScores());

    return 0;
}

