#ifndef _USBXBOXONE_H
#define _USBXBOXONE_H

#include <libusb-1.0/libusb.h>

/* Modifier bits */
#define USB_LCTRL  (1 << 0)
#define USB_LSHIFT (1 << 1)
#define USB_LALT   (1 << 2)
#define USB_LGUI   (1 << 3)
#define USB_RCTRL  (1 << 4)
#define USB_RSHIFT (1 << 5)
#define USB_RALT   (1 << 6) 
#define USB_RGUI   (1 << 7)

struct USBXboxOnePacket {
  uint8_t message;
  uint8_t length;
  uint8_t control[12];
  uint8_t reserved[6];
};

/* Find and open a Xbox One Controller device.  Argument should point to
   space to store an endpoint address.  Returns NULL if no Xbox One Controller
   device was found. */
extern struct libusb_device_handle *openXboxOne(uint8_t *);

#endif
