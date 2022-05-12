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

#define BOAT_LEFT 9
#define HELI_LEFT 10

using namespace std;

void Sprite::generate(BoundaryInRow boundary, short y) {

    srand(time(0));
    if (this->type == SPRITE_FUEL || this->type == SPRITE_BATTLE) {
        if (boundary.river2_left == 0) {
            short width=(boundary.river1_right - boundary.river1_left);
            short randomNumber = rand() % width;
            short new_pos_x = randomNumber + boundary.river1_left;
            this->pos.x = new_pos_x;
            if(this->pos.x>boundary.river1_right||this->pos.x<boundary.river1_left){
                if(this->type==SPRITE_FUEL)
                    printf("type: FUEL\n");
                else
                    printf("type: BATTLE\n");
                printf("!!!position x: %d\n",this->pos.x);
            }

        } else {
            if (rand() % 2) {
                short randomNumber = rand() % (boundary.river1_right - boundary.river1_left);
                short new_pos_x = randomNumber + boundary.river1_left;
                this->pos.x = new_pos_x;
                if(this->pos.x>boundary.river1_right||this->pos.x<boundary.river1_left){
                    if(this->type==SPRITE_FUEL)
                        printf("type: FUEL\n");
                    else
                        printf("type: BATTLE\n");
                    printf("!!!position x: %d\n",this->pos.x);
                }
            } else {
                short randomNumber = rand() % (boundary.river2_right - boundary.river2_left);
                short new_pos_x = randomNumber + boundary.river2_left;
                this->pos.x = new_pos_x;
                if(this->pos.x>boundary.river2_right||this->pos.x<boundary.river2_left){
                    if(this->type==SPRITE_FUEL)
                        printf("type: FUEL\n");
                    else
                        printf("type: BATTLE\n");
                    printf("!!!position x: %d\n",this->pos.x);
                }
            }
        }
    } else {
        short new_pos_x = rand() % 640;
        this->pos.x = new_pos_x;
    }

    this->pos.y = (y << 1) + 1;

//    if((this->type == SPRITE_FUEL || this->type == SPRITE_BATTLE) && this->pos.x < boundary.river1_left || (this->pos.x > boundary.river1_right && boundary.river2_left == 0) || (this->pos.x > boundary.river2_right && boundary.river2_left != 0)){
//        printf("generate: \n");
//        printf("pos: %d\n", pos.x);
//        printf("move: %d, %d, %d, %d\n", boundary.river1_left, boundary.river1_right, boundary.river2_left, boundary.river2_right);
//    }
}

void Sprite::move(int videoFd, BoundaryInRow boundary, short minimumWidth) {

    if(this->canMove== false){
        return;
    }

    srand(time(0));
    short forward = this->pos.x + this->sp.width + minimumWidth;
    short backward = this->pos.x - this->sp.width - minimumWidth;

    if (this->type == SPRITE_FUEL || this->type == SPRITE_BATTLE || this->type==BOAT_LEFT) {
        //Attention: minimumWidth is the minimum width of every branch of the river

        if (boundary.river2_left == 0) {
            if(backward<=boundary.river1_left&&left){
                left=false;
                if(this->type==BOAT_LEFT)
                    this->type=SPRITE_BATTLE;
            }else if(forward>=boundary.river1_right&&!left){
                left=true;
                if(this->type==SPRITE_BATTLE)
                    this->type=BOAT_LEFT;
            }

            if(left){
                this->pos.x=backward;
            }else{
                this->pos.x=forward;
            }
        } else {
            if (this->getPos().x < boundary.river1_right) {
                // on left river
                if(backward<=boundary.river1_left&&left){
                    left=false;
                    if(this->type==BOAT_LEFT)
                        this->type=SPRITE_BATTLE;
                }else if(forward>=boundary.river1_right&&!left){
                    left=true;
                    if(this->type==SPRITE_BATTLE)
                        this->type=BOAT_LEFT;
                }

                if(left){
                    this->pos.x=backward;
                }else{
                    this->pos.x=forward;
                }
            } else {
                // on right river
                if(backward<=boundary.river2_left&&left){
                    left=false;
                    if(this->type==BOAT_LEFT)
                        this->type=SPRITE_BATTLE;
                }else if(forward>=boundary.river2_right&&!left){
                    left=true;
                    if(this->type==SPRITE_BATTLE)
                        this->type=BOAT_LEFT;
                }

                if(left){
                    this->pos.x=backward;
                }else{
                    this->pos.x=forward;
                }
            }
        }
    } else {
        // helicopter can fly through the whole screen
        if(forward>=640-sp.width&&!left){
            left= true;
            if(this->type==SPRITE_HELI)
                this->type=HELI_LEFT;
        }else if(backward<=sp.width){
            left= false;
            if(this->type==HELI_LEFT)
                this->type=SPRITE_HELI;
        }

        if(left){
            this->pos.x=backward;
        }else{
            this->pos.x=forward;
        }
    }

}

void Sprite::disappear() {
    this->pos.y = 0;
}

Shape Sprite::getShape() {
    return sp;
}

