//
// Created by Yongmao Luo on 4/1/22.
//
#ifndef WATER_RAID_GAME_SCENARIO_H
#define WATER_RAID_GAME_SCENARIO_H

#include "string.h"
#include "common_data_structure.h"
#include "time.h"
#include "stdlib.h"

#define INCREASE_WIDTH 0
#define DECREASE_WIDTH 1
#define DOUBLE_RIVER 2
#define SINGLE_RIVER 3

class GameSenario
{
private:
    int minimumWidth;

public:
    BoundaryInRow boundaries[240];/* background register */
    void move_forward(){
        // 20s to change a setting
        clock_t change=clock();
        srand (time(NULL));
        while(1){
            BoundaryInRow newBoundaries;
            if(clock()-change>=20){
                clock_t execute=clock();
                // a round is over, need to change the background
                short temp=rand()%4;
                while(clock()-execute<=5)
                    switch (temp) {
                        case INCREASE_WIDTH:
                            if(boundaries[0].river2_left==0){
                                newBoundaries.river1_left=boundaries[0].river1_left-1;
                            }
                    }
            }

        }
    }

    BoundaryInRow get_boundaries(){
        BoundaryInRow temp;
        temp.river1_left=boundaries[0].river1_left;
        temp.river1_right=boundaries[0].river1_right;
        temp.river2_left=boundaries[0].river2_left;
        temp.river2_right=boundaries[0].river2_right;
        return temp;
    }

    GameSenario(){
        memset(boundaries,0,sizeof(BoundaryInRow)*240);
        minimumWidth=10;
    };

};

#endif