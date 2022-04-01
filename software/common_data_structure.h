#ifndef WATER_RAID_COMMON_DATA_STRUCTURE_H
#define WATER_RAID_COMMON_DATA_STRUCTURE_H

struct position{
    short x,y; // for y, we should put the coordinate at bit [9:1]
    //y[0] is the shift bit
};

struct shape{
    char width, length;
};

#endif