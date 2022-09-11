#ifndef ACCESSORY_HPP
#define ACCESSORY_HPP

#include <iostream>
#include <cstring>
#include <vector>

//delay
#include <unistd.h>

#include <libusb-1.0/libusb.h>



#define AOA_PROTOCOL_MIN	1
#define AOA_PROTOCOL_MAX	2

#define VID_GOOGLE			    0x18D1

//AOAv1
#define	PID_AOA_ACC			    0x2D00
#define	PID_AOA_ACC_ADB		    0x2D01

//USB constant
#define USB_DIR_IN              0x00000080
#define USB_DIR_OUT             0x00000000
#define USB_TYPE_VENDOR         0x00000040
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
        libusb_device *device = NULL;
        libusb_device_handle *handle = NULL;
        bool isClaim = false;
        bool lib_load = false;

        const uint8_t in_addr = 1;
        const uint8_t out_addr = 0x07;
        

        const char *manufacturer = "home";
        const char *model = "androidMic";
        const char *description = "Android-Auto-Interface";
        const char *version = "0.2";
        const char *uri = "https://github.com/wiiznokes/androidMicLinux";
        const char *serialNumber = "1234567890";
        uint16_t dev_vid;
        uint16_t dev_pid;

        void lib_init();
        void find_dev_vid_pid();
        void load_device();
        void change_device();
        

    public:
        Accessory();

        bool isConfigured();
        bool check_for_accessory();

        void send_data(vector<uint8_t> data);
        void read_data(vector<uint8_t>& data);

        void findEndpoint();

        ~Accessory();
};


#endif