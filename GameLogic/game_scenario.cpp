//
// Created by Yongmao Luo on 4/6/22.
//

#include "game_scenario.h"
#include "driver.h"
#include "stdio.h"

void GameScenario::updateBackground(int videoFd) {
    // 20s to change a setting
    double duration=1/double(frequency);
    short singleRiverWidth;
    clock_t change=clock();
    short counter=0;
    BoundaryInRow newBoundaries;
    srand (time(NULL));
    short states=rand()%4;
    clock_t execute=clock();
    bool firstTimeDouble;
    while(1){
        // change game background every duration
        if(double(clock()-execute)/CLOCKS_PER_SEC>=duration){
            execute=clock();


            // a round is over, need to change the background
            if(double(clock()-change)/CLOCKS_PER_SEC>=2){
                firstTimeDouble= true;
                int temp=rand()%4;
                if(states==temp)
                    while(states==temp){
                        temp=rand()%4;
                    }
                change=clock();
                states=temp;
            }
            

            switch (states) {
                case INCREASE_WIDTH:
                    printf("INCREASE WIDTH\n");
                    //while(counter<5){
                        //if(double(clock()-execute)/CLOCKS_PER_SEC>=duration){// run at the setup frequency
                            //execute=clock();
                            //counter++;
                            if(boundaries[screenHeader].river2_left==0){
                                if(boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left<=maximumWidth/2-2){
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left-1;
                                    //printf("river1_left: %d\n",newBoundaries.river1_left);
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                    //printf("river1_right:%d\n",newBoundaries.river1_right);
                                    newBoundaries.river2_left=newBoundaries.river2_right=0;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    //printf("header:%d\n",screenHeader);
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }else{
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right;
                                    //printf("river1_right:%d\n",newBoundaries.river1_right);
                                    newBoundaries.river2_left=newBoundaries.river2_right=0;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    //printf("header:%d\n",screenHeader);
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }
                            }else{// ensure the middle of the two river is at least 4
                                if(boundaries[screenHeader].river1_right+5<boundaries[screenHeader].river2_left && boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left<=maximumWidth/2-2 && boundaries[screenHeader].river2_right-boundaries[screenHeader].river2_left<=maximumWidth/2-2){
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left-1;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                    newBoundaries.river2_left=boundaries[screenHeader].river2_left-1;
                                    newBoundaries.river2_right=boundaries[screenHeader].river2_right+1;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }else{
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right;
                                    newBoundaries.river2_left=boundaries[screenHeader].river2_left;
                                    newBoundaries.river2_right=boundaries[screenHeader].river2_right;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }
                            }
                        //}

                    //}
                    break;
                case DECREASE_WIDTH:
                    printf("DECREASE WIDTH\n");
//                    while(counter<5) {
//                        if(double(clock()-execute)>=duration){
//                            execute=clock();
//                            counter++;
                            if (boundaries[screenHeader].river2_left == 0) {
                                if (boundaries[screenHeader].river1_right - boundaries[screenHeader].river1_left >= minimumWidth + 2) {
                                    newBoundaries.river1_left = boundaries[screenHeader].river1_left + 1;
                                    newBoundaries.river1_right = boundaries[screenHeader].river1_right - 1;
                                    newBoundaries.river2_left = newBoundaries.river2_right = 0;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }else{
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right;
                                    //printf("river1_right:%d\n",newBoundaries.river1_right);
                                    newBoundaries.river2_left=newBoundaries.river2_right=0;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    //printf("header:%d\n",screenHeader);
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
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
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }else{
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right;
                                    newBoundaries.river2_left=boundaries[screenHeader].river2_left;
                                    newBoundaries.river2_right=boundaries[screenHeader].river2_right;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }
                            }
//                        }
//
//                    }
                    counter=0;
                    break;
                case DOUBLE_RIVER:
                    printf("DOUBLE_RIVER\n");
                    //if(boundaries[screenHeader].river2_left==0){ // now we only have one river
                        if(firstTimeDouble){
                            singleRiverWidth=boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left;
                            firstTimeDouble= false;
                            printf("single river width: %d\n",singleRiverWidth);
                        }
//                        while(1){
//                            if(double(clock()-execute)>=duration){
//                                execute=clock();
                                if(boundaries[screenHeader].river2_left==0&&singleRiverWidth*2+10<=maximumWidth){
                                    // when the width is not enough to be divided
                                    if (boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left<(singleRiverWidth)*2+10){
                                        newBoundaries.river1_left=boundaries[screenHeader].river1_left-1;
                                        newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                        newBoundaries.river2_left=newBoundaries.river2_right=0;
                                        // move down the boundary matrix
                                        screenHeader=(screenHeader+1)%480;
                                        boundaries[screenHeader]=newBoundaries;
                                        WaterDriver::writeBoundary(videoFd,newBoundaries);
                                    }else{
                                        printf("Double the river!!!!!!!!\n");
                                        newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                        newBoundaries.river2_right=boundaries[screenHeader].river1_right;
                                        newBoundaries.river1_right=newBoundaries.river1_left+singleRiverWidth;
                                        newBoundaries.river2_left=newBoundaries.river2_right-singleRiverWidth;
                                        printf("river2 left: %d\n",newBoundaries.river2_left);
                                        printf("river2 right: %d\n",newBoundaries.river2_right);
                                        screenHeader=(screenHeader+1)%480;
                                        boundaries[screenHeader]=newBoundaries;
                                        WaterDriver::writeBoundary(videoFd,newBoundaries);
                                        break;
                                    }

                                }else{
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right;
                                    newBoundaries.river2_left=boundaries[screenHeader].river2_left;
                                    newBoundaries.river2_right=boundaries[screenHeader].river2_right;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }
//                            }
//                        }

//                    }else{
//                        states=rand()%4;
//                    }
                    break;
                case SINGLE_RIVER:
                    printf("SINGLE_RIVER\n");
                    if(boundaries[screenHeader].river2_left!=0){ // now we have two rivers
                        short singleRiverWidth=boundaries[screenHeader].river1_right-boundaries[screenHeader].river1_left;
//                        while(1){
//                            if(double(clock()-execute)>=duration){
//                                execute=clock();
                                if (boundaries[screenHeader].river2_left-boundaries[screenHeader].river1_right>0){
                                    // when there are still having 2 rivers
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river1_right+1;
                                    newBoundaries.river2_left=boundaries[screenHeader].river2_left-1;
                                    newBoundaries.river2_right=boundaries[screenHeader].river2_right;
                                    // move down the boundary matrix
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }else if(boundaries[screenHeader].river2_left!=0){
                                    // change it to single river
                                    newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                    newBoundaries.river1_right=boundaries[screenHeader].river2_right;
                                    newBoundaries.river2_left=newBoundaries.river2_right=0;
                                    screenHeader=(screenHeader+1)%480;
                                    boundaries[screenHeader]=newBoundaries;
                                    WaterDriver::writeBoundary(videoFd,newBoundaries);
                                }else{
                                    // single river
                                    if(boundaries[screenHeader].river1_right=boundaries[screenHeader].river1_left>=minimumWidth+2){
                                        newBoundaries.river1_left=boundaries[screenHeader].river1_left+1;
                                        newBoundaries.river1_right=boundaries[screenHeader].river2_right-1;
                                        newBoundaries.river2_left=newBoundaries.river2_right=0;
                                        screenHeader=(screenHeader+1)%480;
                                        boundaries[screenHeader]=newBoundaries;
                                        WaterDriver::writeBoundary(videoFd,newBoundaries);
                                    }else{
                                        newBoundaries.river1_left=boundaries[screenHeader].river1_left;
                                        newBoundaries.river1_right=boundaries[screenHeader].river1_right;
                                        newBoundaries.river2_left=newBoundaries.river2_right=0;
                                        // move down the boundary matrix
                                        screenHeader=(screenHeader+1)%480;
                                        boundaries[screenHeader]=newBoundaries;
                                        WaterDriver::writeBoundary(videoFd,newBoundaries);
                                    }

                               }

//                            }
//                        }

                    }else{
                        int temp=rand()%4;
                        if(states==temp)
                            while(states==temp){
                                temp=rand()%4;
                            }
                        change=clock();
                        states=temp;
                    }
                    break;
            }
        }

    }
}

GameScenario::GameScenario(short minimumWidth, short maximumWidth, short frequency): minimumWidth(minimumWidth), maximumWidth(maximumWidth),frequency(frequency){
    for(int i=0;i<480;i++){
        boundaries->river1_left=300;
        boundaries->river1_right=340;
        boundaries->river2_left=0;
        boundaries->river2_right=0;
    }
    screenHeader=0;
}
