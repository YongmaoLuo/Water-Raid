//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_GAME_Scenario_H
#define WATER_RAID_GAME_Scenario_H

#include "string.h"
#include "common_data_structure.h"
#include "time.h"
#include "stdlib.h"

#define INCREASE_WIDTH 0
#define DECREASE_WIDTH 1
#define DOUBLE_RIVER 2
#define SINGLE_RIVER 3

class GameScenario
{
private:
    short minimumWidth;
    short maximumWidth;
    short frequency; // how many lines the plane flies over per second
    short screenHeader;

public:
    BoundaryInRow boundaries[240];/* background register */
    void updateBackground();
    GameScenario(short minimumWidth, short maximumWidth, short frequency);

    BoundaryInRow get_boundaries(){
        BoundaryInRow temp;
        temp.river1_left=boundaries[0].river1_left;
        temp.river1_right=boundaries[0].river1_right;
        temp.river2_left=boundaries[0].river2_left;
        temp.river2_right=boundaries[0].river2_right;
        return temp;
    }

};

#endif