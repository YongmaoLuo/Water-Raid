// The basic data structures for the objects in this project
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