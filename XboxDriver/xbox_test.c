/*
 *
 * CSEE 4840 Lab 2 for 2019
 *
 * Name/UNI: Yongmao Luo yl4893
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct libusb_device_handle *xbox;

typedef struct {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
}inputEvent;

int main()
{
  int xbox_fd;

  int transferred;

  static const char filename[] = "/dev/input/event0";

  if ( (xbox_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
  //printf("endpoint: %x\n",endpoint_address);
  int i=0;
  inputEvent temp;
  while(1){
    read(xbox_fd,&temp,24);
    printf("code: %d",temp.code);
    printf("value: %d",temp.value);
    
    printf("\n");
    i++;
  }
  
  
    
    
  
  return 0;
}
