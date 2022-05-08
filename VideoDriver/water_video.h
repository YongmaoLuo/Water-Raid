#ifndef _WATER_VIDEO_H
#define _WATER_VIDEO_H

#include <linux/ioctl.h>
#include "../GameLogic/common_data_structure.h"

// typedef struct {
// 	unsigned char red, green, blue;
// } vga_ball_color_t;

typedef struct{
    BoundaryInRow boundary;
    short shift;
}water_video_arg_boundary;

typedef struct{
    Position pos;
    short type;
    short index;
}water_video_arg_position;

typedef struct {
    short fuel;
}water_video_arg_fuel;

typedef struct {
    short score;
}water_video_arg_score;

typedef struct {
    Position fuelPos,scorePos,digit1Pos,digit2Pos,digit3Pos,indicatorPos;
}water_video_arg_init;

#define WATER_VIDEO_MAGIC 'q'

/* ioctls and their arguments */
#define WATER_VIDEO_WRITE_BOUNDARY _IOW(WATER_VIDEO_MAGIC, 1, water_video_arg_boundary *)
#define WATER_VIDEO_WRITE_POSITION _IOR(WATER_VIDEO_MAGIC, 2, water_video_arg_position *)
#define WATER_VIDEO_WRITE_FUEL _IOR(WATER_VIDEO_MAGIC, 3, water_video_arg_fuel *)
#define WATER_VIDEO_WRITE_SCORE _IOR(WATER_VIDEO_MAGIC, 4, water_video_arg_score *)
#define WATER_VIDEO_INIT _IOR(WATER_VIDEO_MAGIC, 4, water_video_arg_init *)

#endif
