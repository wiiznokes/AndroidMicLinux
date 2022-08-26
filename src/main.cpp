#include "main.h"



using namespace std;

int main() {

	libusb_context *context = NULL;
	libusb_device *device;
	int ret;

    ret = libusb_init(&context);
    if(ret < 0){
        cout << "error: libusb_init " << ret << endl;
        return 1;
    }
	
	device = findAndroidDevice(context);

	if(device == NULL) {
		cout << "no android device found" << endl;
		libusb_exit(context);
		return 1;
	}

	ret = isAndroidAcc(device);

	//error in isAndroidAcc
	if(ret == -1) {
		libusb_exit(context);
    	return 1;
	}

	if(ret == 0) {
		if(!switchAndroidToAcc(device)) {
			cout << "switch android to acc fail" << endl;
			libusb_exit(context);
			return 1;
		}
		cout << "switch android to acc success" << endl;
	}
	else
		cout << "device already in accessory mode" << endl;
	

	if(!transfer::init(device)) {
		cout << "error in init transfer" << endl;
		libusb_exit(context);
		return 1;
	}

	unsigned char *buffer;

	if(transfer::read(buffer, BUFFER_SIZE)) {
		cout << sizeof(buffer) << endl;
	}
	else {
		cout << "error while reading" << endl;
		libusb_exit(context);
	}



	transfer::finish();
	libusb_exit(context);
    return 0;
}
