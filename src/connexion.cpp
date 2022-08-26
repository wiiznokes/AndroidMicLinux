#include "connexion.h"

const char *manufacturer = "home";
const char *model = "androidMic";
const char *description = "Android-Auto-Interface";
const char *version = "0.2";
const char *uri = "https://github.com/wiiznokes/androidMicLinux";
const char *serialNumber = "1234567890";

using namespace std;


libusb_device* findAndroidDevice(libusb_context *context){
	libusb_device **devices;
	ssize_t listSize;

	listSize = libusb_get_device_list(context, &devices);

	if(listSize < 0){
		cout << "error: libusb_get_device_list " << listSize << endl;
		libusb_free_device_list(devices, 1);
		return NULL;
	}
	cout << listSize << " devices found" << endl;

    libusb_device *device = NULL;
	struct libusb_device_descriptor device_descriptor;
	int ret;
	int i = 0;
	bool found = false;
	while (i < listSize && !found){
		ret = libusb_get_device_descriptor(devices[i], &device_descriptor);
		if(ret < 0){
			cout << "error: libusb_get_device_descriptor " << ret << endl;
			break;
		}

		if(device_descriptor.idVendor == VID_GOOGLE){
			cout <<"Android device found" << endl;
			found = true;
            device = devices[i];
            break;
		}
        i++;
	}

	libusb_free_device_list(devices, 1);
	return device;
}


int isAndroidAcc(libusb_device *device) {
    struct libusb_device_descriptor device_descriptor;
    int ret;
	ret = libusb_get_device_descriptor(device, &device_descriptor);

	if (ret < 0) {
		cout << "error: libusb_get_device_descriptor " << ret << endl;
		return -1;
	}

    switch(device_descriptor.idProduct) {
		case PID_AOA_ACC:
			return 1;
        case PID_AOA_ACC_ADB:
            return 1;
		default:
			return 0;
		}
}

bool switchAndroidToAcc(libusb_device *device) {

    struct libusb_device_handle *handle;
    unsigned char ioBuffer[2];
	int ret;
    int deviceProtocol;

    ret = libusb_open(device, &handle);
    if(ret < 0){
		cout << "Failed to connect to device: " << ret << endl;
		return false;
	}

    //request 51 (get protocol)
    cout << "request 51" << endl;

    ret = libusb_control_transfer(
        handle,
        192,    //?
        51,
        0,
        0,
        ioBuffer,
        2,
        2000);

    if(ret < 0) {
		cout << "get protocol call failed: " << ret << endl;
		libusb_close(handle);
		return 1;
	}

    //verif protocol
    deviceProtocol = ioBuffer[1] << 8 | ioBuffer[0];
    if (deviceProtocol < AOA_PROTOCOL_MIN || deviceProtocol > AOA_PROTOCOL_MAX) {
		cout << "Unsupported AOA protocol: " << deviceProtocol << endl;
		libusb_close(handle);
		return false;
	}

    //request 52 (determine appropriate Android App)
    cout << "request 52" << endl;

    const char *setupStrings[6];
	setupStrings[0] = manufacturer;
	setupStrings[1] = model;
	setupStrings[2] = description;
	setupStrings[3] = version;
	setupStrings[4] = uri;
	setupStrings[5] = serialNumber;

    int i;
	for(i = 0; i < 6; i++) {
        ret = libusb_control_transfer(
            handle,
            USB_TYPE_VENDOR,
            52,
            0,
            (uint16_t)i,
            (unsigned char*)setupStrings[i],
            strlen(setupStrings[i]),
            2000);


		if(0 > ret) {
			cout << "send string " << i <<  " call failed" << endl;
			libusb_close(handle);
			return false;
		}
	}

    //request 53 (start in accessory mode)
    cout << "request 53" << endl;

    ret = libusb_control_transfer(
        handle,
        USB_TYPE_VENDOR,
        53,
        0,
        0,
        NULL,
        0,
        2000);

	if(ret < 0) {
		cout << "start accessory call failed: " << ret << endl;
		libusb_close(handle);
		return false;
	}
    libusb_close(handle);
    return true;
}



