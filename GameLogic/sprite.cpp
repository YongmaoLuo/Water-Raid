//
// Created by Frank on 4/1/22.
//
#include "sprite.h"
#include "bullet.h"
#include<ctime>
#include<iostream>
#include<vector>

using namespace std;

void Sprite::generate(BoundaryInRow boundary, short y) {

    srand(time(0));
    if (this->type == 3 || this->type == 4) {
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
    } else {
        short new_pos_x = rand() % 640;
        this->pos.x = new_pos_x;
    }

    this->pos.y = (y << 1) + 1;
}

void Sprite::move(BoundaryInRow boundary, short minimumWidth) {

    srand(time(0));
    short forward = this->pos.x + this->getShape().length + minimumWidth;
    short backward = this->pos.x - this->getShape().length - minimumWidth;

    if (this->type == 3 || this->type == 4) {
        //Attention: minimumWidth is the minimum width of every branch of the river

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
                        this->pos.x = boundary.river2_left + minimumWidth;
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
                        this->pos.x = boundary.river1_right - minimumWidth;
                }
            }
        }
    } else {
        if (rand() % 2) {
            if (forward <= 640 - this->sp.width)
                this->pos.x = forward;
            else
                this->pos.x = backward;
        } else {
            if (backward >= 0)
                this->pos.x = backward;
            else
                this->pos.x = forward;
        }
    }
}

void Sprite::disappear() {
    this->pos.y = 0;
}

Shape Sprite::getShape() {
    return sp;
}

