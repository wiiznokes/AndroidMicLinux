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
		exit(1);
	}
	
	printDevice(device);
	
	libusb_exit(context);
    return 0;
}
