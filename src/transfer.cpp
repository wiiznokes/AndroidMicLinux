#include "transfer.h"


namespace transfer
{
	struct libusb_device_handle *handle;
	unsigned char endpoints[10];

	bool findEndpoint(libusb_device *device) {
		int ret;
		int endpointCount = 0;
		endpoints[0] = 0;
		struct libusb_device_descriptor device_descriptor;

		ret = libusb_get_device_descriptor(device, &device_descriptor);

		if(ret < 0) {
			std::cout << "error: libusb_get_device_descriptor " << ret << std::endl;
			return false;
		}

		//nombre de configurations
		for (int i = 0; i < device_descriptor.bNumConfigurations; i++) {
			struct libusb_config_descriptor *config_descriptor;
			ret = libusb_get_config_descriptor(device, i, &config_descriptor);
			if(ret < 0) {
				std::cout << "error: libusb_get_config_descriptor " << ret << std::endl;
				libusb_free_config_descriptor(config_descriptor);
				return false;
			}

			//nombre d'interfaces
			for (int j = 0; j < config_descriptor->bNumInterfaces; j++) {
				const struct libusb_interface *interface;
				interface = &config_descriptor->interface[j];

				//nombre d'altsetting
				for (int k = 0; k < interface->num_altsetting; k++) {
					const struct libusb_interface_descriptor *interface_descriptor;
					interface_descriptor = &interface->altsetting[k];

					//nombre d'endpoint
					for (int l = 0; l < interface_descriptor->bNumEndpoints; l++) {
						const struct libusb_endpoint_descriptor *endpoint_descriptor;
						endpoint_descriptor = &interface_descriptor->endpoint[l];

						printf("bEndpointAddress: %02xh\n", endpoint_descriptor->bEndpointAddress);
						endpoints[endpointCount] = endpoint_descriptor->bEndpointAddress;
						endpointCount++;
					}
				}
			}
			libusb_free_config_descriptor(config_descriptor);
		}
		printf("Endpoint count = %d, [0] = %02xh\n", endpointCount, endpoints[0]);
		if(endpoints[0] != 0)
			return true;
		else
			return false;
	}

	bool init(libusb_device *device){
		int ret;

		ret = libusb_open(device, &handle);
    	if(ret < 0){
			std::cout << "error: libusb_open " << ret << std::endl;
			return false;
		}

		if(!findEndpoint(device)) {
			std::cout << "Failed to find endpoint" << std::endl;
			libusb_close(handle);
			return false;
		}

		return true;
	}

	bool read(unsigned char *data, int length) {
		int ret;

		ret = libusb_bulk_transfer(handle,
								endpoints[0],
								data,
								length,
								NULL,
								3000);

		if (ret < 0) {
			std::cout << "error: libusb_bulk_transfer(): " << ret << std::endl;
			libusb_close(handle);
			return false;
		}
		return true;						
	}

	void finish() {
		libusb_close(handle);
	}
}


