//
// Created by Yongmao Luo on 4/6/22.
//

#include "game_scenario.h"
#include "driver.h"

void GameScenario::updateBackground() {
    // 20s to change a setting
    double duration=2/double(frequency);
    clock_t change=clock();
    short counter=0;
    short riverInitalWidth;
    BoundaryInRow newBoundaries;
    srand (time(NULL));
    while(1){
        // change game background every 20s
        if((clock()-change)/CLOCKS_PER_SEC>=20){
            change=clock();
            counter++;
            clock_t execute=clock();
            // a round is over, need to change the background
            short temp=rand()%4;

            switch (temp) {
                case INCREASE_WIDTH:
                    while(counter<5){
                        if(clock()-execute>=duration){// run at the setup frequency
                            execute=clock();
                            counter++;
                            if(boundaries[screenHeader].river2_left==0){
                                if(boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left<=maximumWidth/2-2){
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left-1;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                    newBoundaries.river2_left=newBoundaries.river2_right=0;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader-1)%240;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(newBoundaries);
                                }
                            }else{// ensure the middle of the two river is at least 4
                                if(boundaries[screenHeader].river1_right+5<boundaries[screenHeader].river2_left && boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left<=maximumWidth/2-2 && boundaries[screenHeader].river2_right-boundaries[screenHeader].river2_left<=maximumWidth/2-2){
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left-1;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                    newBoundaries.river2_left=boundaries[screenHeader].river2_left-1;
                                    newBoundaries.river2_right=boundaries[screenHeader].river2_right+1;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader-1)%240;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(newBoundaries);
                                }
                            }
                        }

                    }
                    counter=0;
                    break;
                case DECREASE_WIDTH:
                    while(counter<5) {
                        if(clock()-execute>=duration){
                            execute=clock();
                            counter++;
                            if (boundaries[screenHeader].river2_left == 0) {
                                if (boundaries[screenHeader].river1_right - boundaries[screenHeader].river1_left >= minimumWidth + 2) {
                                    newBoundaries.river1_left = boundaries[screenHeader].river1_left + 1;
                                    newBoundaries.river1_right = boundaries[screenHeader].river1_right - 1;
                                    newBoundaries.river2_left = newBoundaries.river2_right = 0;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader-1)%240;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(newBoundaries);
                                }
                            } else {
                                if (boundaries[screenHeader].river1_right - boundaries[screenHeader].river1_left >= minimumWidth + 2 &&
                                    boundaries[screenHeader].river2_right - boundaries[screenHeader].river2_left >= minimumWidth - 2) {
                                    // ensure the middle of the two river is at least 4
                                    newBoundaries.river1_left = boundaries[screenHeader].river1_left + 1;
                                    newBoundaries.river1_right = boundaries[screenHeader].river1_right - 1;
                                    newBoundaries.river2_left = boundaries[screenHeader].river2_left + 1;
                                    newBoundaries.river2_right = boundaries[screenHeader].river2_right - 1;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader-1)%240;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(newBoundaries);
                                }
                            }
                        }

                    }
                    counter=0;
                    break;
                case DOUBLE_RIVER:
                    if(boundaries[screenHeader].river2_left==0){ // now we only have one river
                        short initialWidth=boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left;
                        while(1){
                            if(clock()-execute>=duration){
                                execute=clock();
                                if(initialWidth*2+4<=maximumWidth/2){
                                    // when the width is not enough to be divided
                                    if (boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left<(initialWidth)*2+4){
                                        newBoundaries.river1_left=boundaries[screenHeader].river1_left-1;
                                        newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                        newBoundaries.river2_left=newBoundaries.river2_right=0;
                                        // move down the boundary matrix
                                        screenHeader=(screenHeader-1)%240;
                                        boundaries[screenHeader]=newBoundaries;
                                        WaterDriver::writeBoundary(newBoundaries);
                                    }else{
                                        newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                        newBoundaries.river2_right=boundaries[screenHeader].river2_right;
                                        newBoundaries.river1_right=newBoundaries.river1_left+initialWidth;
                                        newBoundaries.river2_left=newBoundaries.river1_right-initialWidth;
                                        screenHeader=(screenHeader-1)%240;
                                        boundaries[screenHeader]=newBoundaries;
                                        WaterDriver::writeBoundary(newBoundaries);
                                        break;
                                    }

                                }
                            }
                        }

                    }
                    break;
                case SINGLE_RIVER:
                    if(boundaries[screenHeader].river2_left!=0){ // now we have two rivers
                        short initialWidth=boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left;
                        while(1){
                            if(clock()-execute>=duration){
                                execute=clock();
                                // when there are still
                                if (boundaries[screenHeader].river2_left-boundaries[screenHeader].river1_right>0){
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                    newBoundaries.river2_left=boundaries[screenHeader].river2_left-1;
                                    newBoundaries.river2_right=boundaries[screenHeader].river2_right;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader-1)%240;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(newBoundaries);
                                }else if(boundaries[screenHeader].river2_left!=0){
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river2_right;
                                    newBoundaries.river2_left=newBoundaries.river2_right=0;
                                    screenHeader=(screenHeader-1)%240;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(newBoundaries);
                                }else{
                                    if(boundaries[screenHeader].river1_right=boundaries[screenHeader].river1_left>=minimumWidth+2){
                                        newBoundaries.river1_left=boundaries[screenHeader].river1_left+1;
                                        newBoundaries.river1_right=boundaries[screenHeader].river2_right-1;
                                        newBoundaries.river2_left=newBoundaries.river2_right=0;
                                        screenHeader=(screenHeader-1)%240;
                                        boundaries[screenHeader]=newBoundaries;
                                        WaterDriver::writeBoundary(newBoundaries);
                                    }else{
                                        break;
                                    }

                                }

                            }
                        }

                    }
                    break;
            }
        }

    }
}

GameScenario::GameScenario(short minimumWidth, short maximumWidth, short frequency): minimumWidth(minimumWidth), maximumWidth(maximumWidth),frequency(frequency){
    memset(boundaries,0,sizeof(BoundaryInRow)*240);
    screenHeader=0;
}
