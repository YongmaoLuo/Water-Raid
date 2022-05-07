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


#define WATER_VIDEO_MAGIC 'q'

/* ioctls and their arguments */
#define WATER_VIDEO_WRITE_BOUNDARY _IOW(WATER_VIDEO_MAGIC, 1, water_video_arg_boundary *)
#define WATER_VIDEO_WRITE_POSITION _IOR(WATER_VIDEO_MAGIC, 2, water_video_arg_position *)

#endif
