/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int vga_ball_fd;

/* Set the background */
void set_background(short bound1, short bound2, short bound3, short bound4, short shift,short sprite1x, short sprite1y, short sprite1img, short sprite2x, short sprite2y, short sprite2img, short sprite3x, short sprite3y, short sprite3img, short fuelx, short fuely, short sprite4img, short scoreboardx, short scoreboardy, short digit1x, short digit1y, short digit1img, short digit2x, short digit2y, short digit2img, short digit3x, short digit3y, short digit3img, short fuelgaugex, short fuelgaugey, short indicatorx, short indicatory, short sprite5x, short sprite5y, short sprite5img , short sprite6x, short sprite6y, short sprite6img , short sprite7x, short sprite7y, short sprite7img, short sprite8x, short sprite8y, short sprite8img, short sprite9x, short sprite9y, short sprite9img)
{
  vga_ball_arg_t vla;
  vla.bound1 = bound1;
  vla.bound2 = bound2;
  vla.bound3 = bound3;
  vla.bound4 = bound4;
  vla.shift = shift;
  vla.sprite1x = sprite1x;
  vla.sprite1y = sprite1y;
  vla.sprite1img = sprite1img;
  vla.sprite2x = sprite2x;
  vla.sprite2y = sprite2y;
  vla.sprite2img = sprite2img;
  vla.sprite3x = sprite3x;
  vla.sprite3y = sprite3y;
  vla.sprite3img = sprite3img;
  vla.fuelx = fuelx;
  vla.fuely = fuely;
  vla.sprite4img = sprite4img;
  vla.scoreboardx = scoreboardx;
  vla.scoreboardy = scoreboardy;
  vla.digit1x = digit1x;
  vla.digit1y = digit1y;
  vla.digit1img = digit1img;
  vla.digit2x = digit2x;
  vla.digit2y = digit2y;
  vla.digit2img = digit2img;
  vla.digit3x = digit3x;
  vla.digit3y = digit3y;
  vla.digit3img = digit3img;
  vla.fuelgaugex = fuelgaugex;
  vla.fuelgaugey = fuelgaugey;
  vla.indicatorx = indicatorx;
  vla.indicatory = indicatory;
  vla.sprite5x = sprite5x;
  vla.sprite5y = sprite5y;
  vla.sprite5img = sprite5img;
  vla.sprite6x = sprite6x;
  vla.sprite6y = sprite6y;
  vla.sprite6img = sprite6img;
  vla.sprite7x = sprite7x;
  vla.sprite7y = sprite7y;
  vla.sprite7img = sprite7img;
  vla.sprite8x = sprite8x;
  vla.sprite8y = sprite8y;
  vla.sprite8img = sprite8img;
  vla.sprite9x = sprite9x;
  vla.sprite9y = sprite9y;
  vla.sprite9img = sprite9img;


  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";


  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  short boundary1 = 200;
  short boundary2 = 440;
  short boundary3 = 0;
  short boundary4 = 0;

  short sprite1x = 320;
  short sprite1y = 751;
  short sprite1img = 0;
  short sprite2x = 140;
  short sprite2y = 200;
  short sprite2img = 1;
  short shift = 0;
  short scoreboardx = 480;
  short scoreboardy = (440 << 1) + 1;
  short digit1x = 525;
  short digit1y = (440 << 1) + 1;
  short digit1img = 2;

  short digit2x = 550;
  short digit2y = (440 << 1) + 1;
  short digit2img = 9;

  short digit3x = 575;
  short digit3y = (440 << 1) + 1;
  short digit3img = 9;

  short fuelgaugex = 320;
  short fuelgaugey = (440 <<1) + 1;

  short indicatorx = 320;
  short indicatory = (438 << 1) + 1;

  short sprite5x = 200;
  short sprite5y = 501;
  short sprite5img = 4;

  short sprite6x = 300;
  short sprite6y = 601;
  short sprite6img = 5;

  short sprite7x = 400;
  short sprite7y = 701;
  short sprite7img = 6;

  short sprite8x = 450;
  short sprite8y = 201;
  short sprite8img = 3;

  short sprite9x = 300;
  short sprite9y = 201;
  short sprite9img = 3;

  int b1incr = 2;
  int b2incr = -2;
  int j = 0;
  int counter = 0;
  while(1){
	shift = 1-shift;
	printf("%d\n", shift);
	if(boundary1 < 150 || boundary1 > 250) b1incr = -1*b1incr;
	if(boundary2 < 390 || boundary2 > 490) b2incr = -1*b2incr;
	
	printf("ITER: %d\n", j);
	j++;
	if(j%4 == 0) counter++;
	digit1img = (short) counter/100;
	digit2img = (short) (counter / 10)%10;
	digit3img = (short) counter %10;
	boundary1 = boundary1 + b1incr;
	boundary2 = boundary2 + b2incr;

	
  	set_background(boundary1, boundary2, boundary3, boundary4, shift, sprite1x, sprite1y, 0, sprite2x, (sprite2y<<1)+1, 1, 100, (100<<1)+1, 2, 400, (200<<1)+1, 3, scoreboardx, scoreboardy, digit1x, digit1y, digit1img, digit2x, digit2y, digit2img, digit3x, digit3y, digit3img, fuelgaugex, fuelgaugey, indicatorx, indicatory, sprite5x, sprite5y, sprite5img, sprite6x, sprite6y, sprite6img, sprite7x, sprite7y, sprite7img, sprite8x, sprite8y, sprite8img, sprite9x, sprite9y, sprite9img);
  	usleep(40000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
