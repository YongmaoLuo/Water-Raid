//
// Created by Frank on 4/1/22.
//
#include "sprite.h"
#include "bullet.h"
#include<ctime>
#include<iostream>
#include<vector>

#define SPRITE_PLANE 0
#define SPRITE_HELI 1
#define SPRITE_BATTLE 2
#define SPRITE_FUEL 3
#define SPRITE_BALLOON 6

using namespace std;

void Sprite::generate(BoundaryInRow boundary, short y) {

    srand(time(0));
    if (this->type == SPRITE_FUEL || this->type == SPRITE_BATTLE) {
        if (boundary.river2_left == 0) {
            short new_pos_x = (rand() % (boundary.river1_right - boundary.river1_left)) + boundary.river1_left;
            this->pos.x = new_pos_x;
        } else {
            if (rand() % 2) {
                short new_pos_x = (rand() % (boundary.river1_right - boundary.river1_left)) + boundary.river1_left;
                this->pos.x = new_pos_x;
            } else {
                short new_pos_x = (rand() % (boundary.river2_right - boundary.river2_left)) + boundary.river2_left;
                this->pos.x = new_pos_x;
            }
        }
    } else if(this->type==SPRITE_HELI) {
        short new_pos_x = rand() % 640;
        this->pos.x = new_pos_x;
    }

    this->pos.y = (y << 1) + 1;
}

void Sprite::move(BoundaryInRow boundary, short minimumWidth) {

    if(this->canMove== false){
        return;
    }

    srand(time(0));
    short forward = this->pos.x  + minimumWidth;
    short backward = this->pos.x - minimumWidth;

    if (this->type == SPRITE_FUEL || this->type == SPRITE_BATTLE) {
        //Attention: minimumWidth is the minimum width of every branch of the river

        if (boundary.river2_left == 0) {
            if(pos.x<=boundary.river1_left&&left){
                left=false;
            }else if(pos.x>=boundary.river1_right&&!left){
                left=true;
            }

            if(left){
                pos.x=backward;
            }else{
                pos.x=forward;
            }
        } else {
            if (this->getPos().x < (boundary.river1_right+boundary.river2_left)/2) {
                // on left river
                if(pos.x<=boundary.river1_left&&left){
                    left=false;
                }else if(pos.x>=boundary.river1_right&&!left){
                    left=true;
                }

                if(left){
                    pos.x=backward;
                }else{
                    pos.x=forward;
                }
            } else {
                // on right river
                if(pos.x<=boundary.river2_left&&left){
                    left=false;
                }else if(pos.x>=boundary.river2_right&&!left){
                    left=true;
                }

                if(left){
                    pos.x=backward;
                }else{
                    pos.x=forward;
                }
            }
        }
    } else if(this->type==SPRITE_HELI) {
        // helicopter can fly through the whole screen
        if(pos.x>=640-sp.width&&!left){
            left= true;
        }else if(pos.x<=sp.width){
            left= false;
        }

        if(left){
            pos.x=backward;
        }else{
            pos.x=forward;
        }
    }

}

void Sprite::disappear() {
    this->pos.y = 0;
}

Shape Sprite::getShape() {
    return sp;
}

