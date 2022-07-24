#include "connexion.h"

using namespace std;


libusb_device* findAndroidDevice(libusb_context *context){
	libusb_device **devices;
	ssize_t listSize;

	listSize = libusb_get_device_list(context, &devices);

	if(listSize < 0){
		cout << "Error in getting device list" << endl;
		libusb_free_device_list(devices, 1);
		return NULL;
	}
	cout << listSize << " devices found" << endl;

    libusb_device *device = NULL;
	struct libusb_device_descriptor *device_descriptor;
	int ret;
	int i = 0;
	bool found = false;
	while (i < listSize && !found){
		ret = libusb_get_device_descriptor(devices[i], device_descriptor);
		if(ret < 0){
			cout << "Error in getting device descriptor" << endl;
			break;
		}

		if(device_descriptor->idVendor == VID_GOOGLE){
			cout <<"Android device found" << "\n" << endl;
			found = true;
            device = devices[i];
            break;
		}
        i++;
	}

	libusb_free_device_list(devices, 1);
	return device;
}

void printDevice(libusb_device *device){

    struct libusb_device_descriptor *device_descriptor;
    struct libusb_config_descriptor *config_descriptor;
    const struct libusb_interface *interface;
    const struct libusb_interface_descriptor *interface_descriptor;
    const struct libusb_endpoint_descriptor *endpoint_descriptor;

    int ret;
    int i, j, k;

    ret = libusb_get_device_descriptor(device, device_descriptor);

    if(ret < 0){
        cout << "Error in getting device descriptor" << endl;
        return;
    }

    cout << "Number of configs: " << device_descriptor->bNumConfigurations << endl;
    cout << "IdVendor: " << device_descriptor->idVendor << endl;
    cout << "IdProduct: " << device_descriptor->idProduct << endl;

    libusb_get_config_descriptor(device, 0, &config_descriptor);
    cout << "Number of interface: " << config_descriptor->bNumInterfaces << endl;

    for(i = 0; i < config_descriptor->bNumInterfaces; i++){
        interface = &config_descriptor->interface[i];
        cout << " Number of alt settings: " << interface->num_altsetting << endl;

        for(j = 0; j < interface->num_altsetting; j++){
            interface_descriptor = &interface->altsetting[j];
            cout << "  Interface number: " << interface_descriptor->bInterfaceNumber << endl;
            cout << "  Number of endpoints: " << interface_descriptor->bNumEndpoints << endl;

            for(k = 0; k < interface_descriptor->bNumEndpoints; k++){
                endpoint_descriptor = &interface_descriptor->endpoint[k];
                cout << "   Desc type: " << endpoint_descriptor->bDescriptorType << endl;
                cout << "   EndPoint adress: " << endpoint_descriptor->bEndpointAddress << endl;
            }
        }
    }

    cout << "\n\n" << endl;
    libusb_free_config_descriptor(config_descriptor);
}



