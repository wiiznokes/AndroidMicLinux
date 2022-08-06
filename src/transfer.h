#ifndef TRANSFER_H_
#define TRANSFER_H_

#include <iostream>
#include <libusb-1.0/libusb.h>

#define BUFSIZE 2560

namespace transfer 
{
bool init(libusb_device *device);
bool read(unsigned char *data, int length);
void finish();
}

#endif