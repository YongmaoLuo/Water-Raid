//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_GAME_SENARIO_H
#define WATER_RAID_GAME_SENARIO_H

#include "string.h"
#include "common_data_structure.h"

class Game_senario
{
private:
    void backgound_logic(){

    }
    int minimumWidth;

public:
    /* background register */
    short boundaries[240][4];
    void move_forward(){

    }

    BoundaryInRow get_boundaries(){
        BoundaryInRow temp;
        temp.river1_left=boundaries[0][0];
        temp.river1_right=boundaries[0][1];
        temp.river2_left=boundaries[0][2];
        temp.river2_right=boundaries[0][3];
        return temp;
    }

    Game_senario(){
        memset(boundaries,0,sizeof(short)*240*4);
        minimumWidth=10;
    };

};

#endif