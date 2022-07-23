#ifndef CONNEXION_H_
#define CONNEXION_H_

#include <iostream>
#include <libusb-1.0/libusb.h>


#define AOA_PROTOCOL_MIN	1
#define AOA_PROTOCOL_MAX	2

#define VID_GOOGLE			    0x18D1

//AOAv1
#define	PID_AOA_ACC			    0x2D00
#define	PID_AOA_ACC_ADB		    0x2D01

//USB constant
#define USB_DIR_IN              0x80
#define USB_DIR_OUT             0x0
#define USB_TYPE_VENDOR         0x40




libusb_device *findAndroidDevice(libusb_context *context);
void printDevice(libusb_device *device);

#endif
