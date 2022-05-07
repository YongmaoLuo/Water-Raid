#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>

  

typedef struct {
  short bound1;
  short bound2;
  short bound3;
  short bound4;
  short shift;
  short sprite1x;
  short sprite1y;
  short sprite1img;
  short sprite2x;
  short sprite2y;
  short sprite2img;
  short sprite3x;
  short sprite3y;
  short sprite3img;
  short fuelx;
  short fuely;
  short sprite4img;
  short scoreboardx;
  short scoreboardy;
  short digit1x;
  short digit1y;
  short digit1img;
  short digit2x;
  short digit2y;
  short digit2img;
  short digit3x;
  short digit3y;
  short digit3img;
  short fuelgaugex;
  short fuelgaugey;
  short indicatorx;
  short indicatory;
  short sprite5x;
  short sprite5y;
  short sprite5img;
  short sprite6x;
  short sprite6y;
  short sprite6img;
  short sprite7x;
  short sprite7y;
  short sprite7img;
  short sprite8x;
  short sprite8y;
  short sprite8img;
  short sprite9x;
  short sprite9y;
  short sprite9img;

} vga_ball_arg_t;

#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_BACKGROUND _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t *)
#define VGA_BALL_READ_BACKGROUND  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t *)

#endif
