#include "transfer.h"

struct libusb_device_handle *handle;
unsigned char endpoint;

void init(struct libusb_device_handle *handle_, unsigned char endpoint_){
	handle = handle_;
	endpoint = endpoint_;
};

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
