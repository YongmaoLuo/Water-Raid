//
// Created by Yongmao Luo on 4/1/22.
//

// The background of the game
// Copyright (C) 2022  Yongmao Luo

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
    short minimumWidth; // the minimum width of the river
    short maximumWidth; // the maximum with of the river
    double frequency; // how many lines the plane flies over per second
    short screenHeader; // the header of the circle queue
    short states; // the state of the state machine
    int singleRiverWidth; // when we double the river, we need to record the former width of the river
    bool firstTimeDouble; // indicator for first time the state becomes DOUBLE_RIVER
    clock_t change; // clock used to adjust the frequency of randomly select new state

public:
    BoundaryInRow boundaries[480];/* background register */
    void updateBackground(int videoFd); // randomly generate new boundaries by maintaining a state machine
    void initBackground(int videoFd); // at the start of each round of game, flash the background to the same
    double getFrequency();
    void setChangeClock();
    int getScreenHeader();
    GameScenario(int videoFd,short minimumWidth, short maximumWidth, short frequency);

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