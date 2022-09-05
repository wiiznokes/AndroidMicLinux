#include "transfer.h"


namespace transfer
{
	struct libusb_device_handle *handle;
	ConfTransfer confTransfer;

	bool findConfTransferList(libusb_device *device, ConfTransferList *confTransferList) {
		int ret;

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
				//creation of the structure that stores the information useful for the transfer
				ConfTransfer *confTransfer = new ConfTransfer;
				

				//nombre d'altsetting
				for (int k = 0; k < interface->num_altsetting; k++) {
					const struct libusb_interface_descriptor *interface_descriptor;
					interface_descriptor = &interface->altsetting[k];

					confTransfer->interface_number = interface_descriptor->bInterfaceNumber;

					//nombre d'endpoint
					for (int l = 0; l < interface_descriptor->bNumEndpoints; l++) {
						const struct libusb_endpoint_descriptor *endpoint_descriptor;
						endpoint_descriptor = &interface_descriptor->endpoint[l];

						confTransfer->endpoints.push_back(endpoint_descriptor->bEndpointAddress);
					}
					confTransferList->list.push_back(*confTransfer);
				}
			}
			libusb_free_config_descriptor(config_descriptor);
		}

		if(confTransferList->list.size() > 0)
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

		ConfTransferList *confTransferList = new ConfTransferList;
		if(!findConfTransferList(device, confTransferList)) {
			std::cout << "Failed to find endpoint" << std::endl;
			libusb_close(handle);
			delete confTransferList;
			return false;
		}

		//choice of confTransfer
		confTransfer = confTransferList->list[0];
		confTransfer.endpoint = confTransfer.endpoints[0];
		delete confTransferList;
		printf("Interface number: %d, endpoint: %02xh\n",confTransfer.interface_number, confTransfer.endpoint);

		ret = libusb_claim_interface(handle, confTransfer.interface_number);
		if(ret < 0){
			std::cout << "error: libusb_claim_interface " << ret << std::endl;
			return false;
		}

		delete confTransferList;
		return true;
	}

	bool read(unsigned char *data, int length) {
		int ret;

		ret = libusb_bulk_transfer(handle,
								confTransfer.endpoints[0],
								data,
								length,
								NULL,
								3000);

		if (ret < 0) {
			std::cout << "error: libusb_bulk_transfer: " << ret << std::endl;
			libusb_close(handle);
			return false;
		}
		return true;						
	}

	void finish() {
		int ret;
		ret = libusb_release_interface(handle, confTransfer.interface_number);
		if (ret < 0) {
			std::cout << "error: libusb_release_interface: " << ret << std::endl;
		}

		libusb_close(handle);
	}
}


