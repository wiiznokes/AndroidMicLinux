#ifndef TRANSFER_H_
#define TRANSFER_H_

#include <iostream>
#include <libusb-1.0/libusb.h>

#define BUFSIZE 2560


void init(struct libusb_device_handle *handle_, unsigned char endpoint_);
bool read(unsigned char *data, int length);
void finish();

#endif