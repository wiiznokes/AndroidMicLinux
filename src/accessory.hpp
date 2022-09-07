#ifndef ACCESSORY_HPP
#define ACCESSORY_HPP

#include <iostream>
#include <cstring>
#include <vector>

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
#define DELAY_CONTROL           2000
#define BUFFER_SIZE 2560

using namespace std;

class AccessoryException : public runtime_error {
    public:
        AccessoryException(const string &message) : runtime_error(message) {};
};

class Accessory{

    private:
        libusb_context *context = NULL;
        libusb_device_handle* handle = NULL;
        bool isClaim = false;

        const uint8_t in_addr = 0x85;
        const uint8_t out_addr = 0x07;
        

        const char *manufacturer = "home";
        const char *model = "androidMic";
        const char *description = "Android-Auto-Interface";
        const char *version = "0.2";
        const char *uri = "https://github.com/wiiznokes/androidMicLinux";
        const char *serialNumber = "1234567890";
        uint16_t dev_vid;
        uint16_t dev_pid;

        void find_pid_vid();
        void load_device();
        void change_device();
        

    public:
        Accessory();
        void lib_init();

        bool isAccessory();
        bool check_for_accessory();

        void send_data(vector<uint8_t> data);
        void read_data(vector<uint8_t>& data);

        ~Accessory();
};


#endif