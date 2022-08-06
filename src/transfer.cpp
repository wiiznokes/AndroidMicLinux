#include "transfer.h"


namespace transfer 
{
	struct libusb_device_handle *handle;
	unsigned char endpoint;

	bool findEndpoint(libusb_device *device) {
		return false;
	}

	bool init(libusb_device *device){
		int ret;

		ret = libusb_open(device, &handle);
    	if(ret < 0){
			std::cout << "Failed to connect to device: " << ret << std::endl;
			return false;
		}

		if(!findEndpoint(device)) {
			std::cout << "Failed to find endpoint" << std::endl;
			return false;
		}

		return true;
	}

	bool read(unsigned char *data, int length) {

		int ret;

		ret = libusb_bulk_transfer(handle,
								endpoint,
								data,
								length,
								NULL,
								3000);

		if (ret < 0) {
			std::cout << "error in libusb_bulk_transfer(): " << ret << std::endl;
			finish();
			return false;
		}
		return true;						
	}


	void finish() {

	}
}


