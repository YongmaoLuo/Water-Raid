/*
 * Userspace program that communicates with the water_video device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "water_video.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int water_video_fd;

#define SCREEN_X 640
#define SCREEN_Y 480

/* Read and print the coordinates */
void print_coordinate(water_video_coordinate_t *c) {
  water_video_arg_t vla;
  
  if (ioctl(water_video_fd, water_video_READ_COORDINATE, &vla)) {
      perror("ioctl(water_video_READ_COORDINATE) failed");
      return;
  }
  printf("%03d %03d\n",
	 vla.coordinate.vcor,vla.coordinate.hcor);
   c->vcor=vla.coordinate.vcor;
   c->hcor=vla.coordinate.hcor;
}

/* Set the background color */
void set_coordinate(const water_video_coordinate_t *c)
{
  water_video_arg_t vla;
  vla.coordinate = *c;
  if (ioctl(water_video_fd, water_video_WRITE_COORDINATE, &vla)) {
      perror("ioctl(water_video_SET_COORDINATE) failed");
      return;
  }
}

int main()
{
  water_video_arg_boundary boundary;
  int i;
  static const char filename[] = "/dev/vga_ball";

  // static const water_video_color_t colors[] = {
  //   { 0xff, 0x00, 0x00 }, /* Red */
  //   { 0x00, 0xff, 0x00 }, /* Green */
  //   { 0x00, 0x00, 0xff }, /* Blue */
  //   { 0xff, 0xff, 0x00 }, /* Yellow */
  //   { 0x00, 0xff, 0xff }, /* Cyan */
  //   { 0xff, 0x00, 0xff }, /* Magenta */
  //   { 0x80, 0x80, 0x80 }, /* Gray */
  //   { 0x00, 0x00, 0x00 }, /* Black */
  //   { 0xff, 0xff, 0xff }  /* White */
  // };

//# define COLORS 9

  printf("VGA ball Userspace program started\n");
  printf("HELLO WORLD\n");

  if ( (water_video_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  water_video_coordinate_t temp;

  printf("initial state: ");
  //print_coordinate(&temp);

  //set_coordinate(&temp);
  short slop=1;
  short up=1;

  while(1){
    print_coordinate(&temp);
    // when hitting different edge, it will have different behaviors
    if(temp.vcor-16<=0){
      // reach the upper or rightmost edge
      slop=slop*(-1);
      up=up*(-1); // the ball will move downwards
    }else if(temp.hcor+16>=SCREEN_X){
      slop=slop*(-1);
      up=up; // the ball will move downwards
    }else if(temp.vcor+16>=SCREEN_Y){
      // reach the lower or leftmost edge
      slop=slop*(-1);
      up=up*(-1);
    }else if(temp.hcor-16<=0){
      slop=slop*(-1);
      up=up; // the ball will move downwards
    }
    // according to the moving direction, change the coordinates
    if(slop==1&&up==1){
      temp.vcor-=1;
      temp.hcor+=1;
    }else if(slop==1&&up==-1){
      temp.vcor+=1;
      temp.hcor-=1;
    }else if(slop==-1&&up==1){
      temp.vcor-=1;
      temp.hcor-=1;
    }else{
      temp.vcor+=1;
      temp.hcor+=1;
    }

    set_coordinate(&temp);
    usleep(20000);

  }

  // for (i = 0 ; i < 24 ; i++) {
  //   temp.vcor+=1;
  //   temp.hcor+=1;
  //   set_coordinate(&temp);
  //   print_coordinate(&temp);
  //   usleep(1000000);
  // }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
