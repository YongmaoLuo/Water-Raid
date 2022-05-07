#ifndef WATER_RAID_COMMON_DATA_STRUCTURE_H
#define WATER_RAID_COMMON_DATA_STRUCTURE_H

typedef struct{
    short x,y; // for y, we should put the coordinate at bit [9:1]
    //y[0] is the shift bit, y[0]=1 means disappear
}Position;

typedef struct{
    char width, length; // width is related to x coordinates
    // length is related to y coordinates
}Shape;

typedef struct{
    short river1_left,river1_right,river2_left, river2_right;
}BoundaryInRow;

#endif