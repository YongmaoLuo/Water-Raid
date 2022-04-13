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
#include "usbxbox.h"

struct libusb_device_handle *xbox;
uint8_t endpoint_address;

int main()
{
  int err, col;

  struct USBXboxOnePacket packet;
  int transferred;
  char keystate[12];

  if ((err = fbopen()) != 0) {
    fprintf(stderr, "Error: Could not open framebuffer: %d\n", err);
    exit(1);
  }

  /* Open the keyboard */
  if ( (xbox = openXboxOne(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a Xbox One Controller\n");
    exit(1);
  }
  while(1){
    libusb_interrupt_transfer(xbox, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    printf("%d\n",packet[1]);
  }
  
  
    
    
  
  return 0;
}
