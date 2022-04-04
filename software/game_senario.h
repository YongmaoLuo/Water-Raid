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
    // boundaries[i][0]
    BoundaryInRow boundaries[240];
    void move_forward(){

    }

    BoundaryInRow get_boundaries(){
        BoundaryInRow temp;
        temp.river1_left=boundaries[0].river1_left;
        temp.river1_right=boundaries[0].river1_right;
        temp.river2_left=boundaries[0].river2_left;
        temp.river2_right=boundaries[0].river2_right;
        return temp;
    }

    Game_senario(){
        memset(boundaries,0,sizeof(BoundaryInRow)*240);
        minimumWidth=10;
    };

};

#endif