#include "main.h"

using namespace std;

void initUsb(libusb_context *context) {
    int ret;
    ret = libusb_init(&context);
    if(ret < 0){
        perror("Error libusb_init\n\n");
        exit(1);
    }
    return;
}

// g++ -o prog ./src/*cpp -lusb-1.0 && ./prog
int main() {

	libusb_context *context = NULL;
	libusb_device *device;
	initUsb(context);
	
	device = findAndroidDevice(context);

	if(device == NULL) {
		cout << "no android device found" << endl;
		libusb_exit(context);
		exit(1);
	}
	
	printDevice(device);

	if(!isAndroidAcc(device)) {
		if(!switchAndroidToAcc(device)) {
			cout << "switch android to acc fail" << endl;
			libusb_exit(context);
			exit(1);
		}
		cout << "switch android to acc success" << endl;
	}
	else
		cout << "device already in accessory mode" << endl;

	

	
	libusb_exit(context);
    return 0;
}
