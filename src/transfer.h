#ifndef TRANSFER_H_
#define TRANSFER_H_

#include <iostream>
#include <vector>
#include <libusb-1.0/libusb.h>

#define BUFSIZE 2560

struct ConfTransfer
{
    int interface_number;
    std::vector<unsigned char> endpoints;
    unsigned char endpoint;
};

struct ConfTransferList
{
  std::vector<ConfTransfer> list;  
};


namespace transfer 
{
bool init(libusb_device *device);
bool read(unsigned char *data, int length);
void finish();
}

#endif