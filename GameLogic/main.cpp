//
// Created by Yongmao Luo on 4/1/22.
//

#include "bullet.h"
#include "bullet.cpp"

#include "game_scenario.h"

#include "battleship.h"
#include "airplane.h"
#include "driver.h"
#include "common_data_structure.h"
#include "sprite.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


#define MAX_SPRITES_NUM 8
#define MAX_BULLET_NUM 5

int main(){
//    Shape bulletSP = {2, 5};
//    Position bulletPOS = {10, 10};
//
//    Bullet *bullets = new Bullet[MAX_BULLET_NUM] {
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//            Bullet(1, bulletSP, false, bulletPOS),
//    };
//    vector<Bullet> vBullet(bullets, bullets + 5);
//
//    vBullet[0].fly();
//    vBullet[1].fly();
//    vBullet[2].fly();
//
//    Shape btsSP = {5, 5};
//    Position btsPOS = {5, 4};
//    Battleship bts = Battleship(3, 3, 0, btsSP, false, btsPOS, 3);

    Airplane airplane = Airplane();
    static const char xbox[] = "/dev/input/event0";
    static const char waterVideo[]="/dev/vga_ball";
    int videoFd;
//    bts.checkIfHit(vBullet, airplane);

//    printf("%s\n", bts.getIsDestroy());
//    while(1){
//        airplane.receivePos(inputDevice);
//        Position tempPos= airplane.getPos();
//        printf("x: %d\n",tempPos.x);
//        printf("y: %d\n",tempPos.y);
//        printf("\n");
//    }
    if ((videoFd = open(waterVideo, O_RDWR)) == -1) {
        fprintf(stderr,"could not open %s\n", waterVideo);
        return -1;
    }
    GameScenario gameScenario(30,640,60);

    gameScenario.updateBackground(videoFd);
//    BoundaryInRow boundary;
//    boundary.river1_left=40;
//    boundary.river1_right=100;
//    boundary.river2_left=200;
//    boundary.river2_right=400;
//    int left=1, right=1;
//    while(1){
//        if(boundary.river1_left<=20){
//            left=-left;
//            right=-right;
//        }else if(boundary.river1_left>=50){
//            left=-left;
//            right=-right;
//        }
//        boundary.river1_left-=left;
//        boundary.river1_right+=right;
//        boundary.river2_left-=left;
//        boundary.river2_right+=right;
//        printf("%d\n",boundary.river1_left);
//
//        WaterDriver::writeBoundary(videoFd,boundary);
//        usleep(40000);
//    }

    return 0;
}
