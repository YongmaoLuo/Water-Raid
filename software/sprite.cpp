//
// Created by Frank on 4/1/22.
//
#include "sprite.h"
#include "bullet.h"
#include<ctime>
#include<iostream>
#include<vector>

using namespace std;

void Sprite::generate(BoundaryInRow boundary) {

    srand(time(0));

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

    //hard code the y coordinate of its born position to 100
    this->pos.y = 100;
}

void Sprite::move(BoundaryInRow boundary) {

    srand(time(0));

    //Attention: the width of every branch of the river must greater than 20
    short forward = this->pos.x + 20;
    short backward = this->pos.x - 20;

    if (boundary.river2_left == 0) {
        if (rand() % 2) {
            if (forward <= boundary.river1_right)
                this->pos.x = forward;
            else
                this->pos.x = backward;
        } else {
            if (backward >= boundary.river1_left)
                this->pos.x = backward;
            else
                this->pos.x = forward;
        }
    } else {
        if (this->getPos().x <= boundary.river1_right) {
            if (rand() % 2) {
                if (forward <= boundary.river1_right)
                    this->pos.x = forward;
                else
                    this->pos.x = boundary.river2_left + 20;
            } else {
                if (backward >= boundary.river1_left)
                    this->pos.x = backward;
                else
                    this->pos.x = forward;
            }
        } else {
            if (rand() % 2) {
                if (forward <= boundary.river2_right)
                    this->pos.x = forward;
                else
                    this->pos.x = backward;
            } else {
                if (backward >= boundary.river2_left)
                    this->pos.x = backward;
                else
                    this->pos.x = boundary.river1_right - 20;
            }
        }
    }
}

void Sprite::disappear(char id, vector<Sprite> sprites) {
    for(int i = 0; i < sprites.size(); i++)
    {
        if(sprites[i].id == id){
            sprites[i].pos.y |=~0x1;
        }
    }
}

