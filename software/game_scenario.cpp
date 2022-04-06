//
// Created by Yongmao Luo on 4/6/22.
//

#include "game_scenario.h"

void GameScenario::move_forward(){
    // 20s to change a setting
    double duration=2/double(frequency);
    clock_t change=clock();
    short increaseWidthCounter=0;
    short decreaseWidthCounter=0;
    srand (time(NULL));
    while(1){
        BoundaryInRow newBoundaries;
        // change game background every 20s
        if((clock()-change)/CLOCKS_PER_SEC>=duration){
            change=clock();
            increaseWidthCounter++;
            clock_t execute=clock();
            // a round is over, need to change the background
            short temp=rand()%4;

            switch (temp) {
                case INCREASE_WIDTH:
                    while(increaseWidthCounter<5){
                        increaseWidthCounter++;
                        if(boundaries[0].river2_left==0){
                            if(boundaries[0].river1_right-boundaries[0].river1_left<=maximumWidth-2){
                                newBoundaries.river1_left=boundaries[0].river1_left-1;
                                newBoundaries.river1_right=boundaries[0].river1_right+1;
                                newBoundaries.river2_left=newBoundaries.river2_right=0;
                                // move down the boundary matrix
                                int pointer=238;
                                while(pointer>=0){
                                    boundaries[pointer+1]=boundaries[pointer];
                                    pointer--;
                                }
                                boundaries[0]=newBoundaries;
                            }
                        }else{
                            if(boundaries[0].river1_right+5<boundaries[0].river2_left && boundaries[0].river1_right-boundaries[0].river1_left<=maximumWidth-2 && boundaries[0].river2_right-boundaries[0].river2_left<=maximumWidth-2){
                                // ensure the middle of the two river is at least 4

                                newBoundaries.river1_left=boundaries[0].river1_left-1;
                                newBoundaries.river1_right=boundaries[0].river1_right+1;
                                newBoundaries.river2_left=boundaries[0].river2_left-1;
                                newBoundaries.river2_right=boundaries[0].river2_right+1;
                                // move down the boundary matrix
                                int pointer=238;
                                while(pointer>=0){
                                    boundaries[pointer+1]=boundaries[pointer];
                                    pointer--;
                                }
                                boundaries[0]=newBoundaries;
                            }
                        }
                    }
                    increaseWidthCounter=0;
                    break;
                case DECREASE_WIDTH:
                    while(decreaseWidthCounter<5) {
                        decreaseWidthCounter++;
                        if (boundaries[0].river2_left == 0) {
                            if (boundaries[0].river1_right - boundaries[0].river1_left >= minimumWidth + 2) {
                                newBoundaries.river1_left = boundaries[0].river1_left + 1;
                                newBoundaries.river1_right = boundaries[0].river1_right - 1;
                                newBoundaries.river2_left = newBoundaries.river2_right = 0;
                                // move down the boundary matrix
                                int pointer=238;
                                while(pointer>=0){
                                    boundaries[pointer+1]=boundaries[pointer];
                                    pointer--;
                                }
                                boundaries[0]=newBoundaries;
                            }
                        } else {
                            if (boundaries[0].river1_right - boundaries[0].river1_left >= minimumWidth + 2 &&
                                boundaries[0].river2_right - boundaries[0].river2_left >= minimumWidth - 2) {
                                // ensure the middle of the two river is at least 4
                                newBoundaries.river1_left = boundaries[0].river1_left + 1;
                                newBoundaries.river1_right = boundaries[0].river1_right - 1;
                                newBoundaries.river2_left = boundaries[0].river2_left + 1;
                                newBoundaries.river2_right = boundaries[0].river2_right - 1;
                                // move down the boundary matrix
                                int pointer=238;
                                while(pointer>=0){
                                    boundaries[pointer+1]=boundaries[pointer];
                                    pointer--;
                                }
                                boundaries[0]=newBoundaries;
                            }
                        }
                    }
                    decreaseWidthCounter=0;
                    break;
                case DOUBLE_RIVER:
                    if(boundaries[0].river2_left==0){ // now we only have one river
                        short initialWidth=boundaries[0].river1_right-boundaries[0].river1_left;
                        if(initialWidth*2+4<=maximumWidth){
                            while (newBoundaries.river1_right-newBoundaries.river1_left<(initialWidth)*2+4){
                                newBoundaries.river1_left=boundaries[0].river1_left-1;
                                newBoundaries.river1_right=boundaries[0].river1_right+1;
                                newBoundaries.river2_left=newBoundaries.river2_right=0;
                                // move down the boundary matrix
                                int pointer=238;
                                while(pointer>=0){
                                    boundaries[pointer+1]=boundaries[pointer];
                                    pointer--;
                                }
                                boundaries[0]=newBoundaries;
                            }
                        }

                    }
                    break;
            }
        }

    }
}

GameScenario::GameScenario(short minimumWidth, short maximumWidth): minimumWidth(minimumWidth), maximumWidth(maximumWidth),frequency(frequency){
    memset(boundaries,0,sizeof(BoundaryInRow)*240);
}
