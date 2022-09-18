// Simple testing software for xpad driver
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
    printf("type: %d\n",temp.type);
    printf("code: %d\n",temp.code);
    printf("value: %d\n",temp.value);
    
    printf("\n");
    i++;
  }
  
  
    
    
  
  return 0;
}
