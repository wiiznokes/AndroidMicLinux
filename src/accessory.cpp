#include "accessory.hpp"


void Accessory::lib_init() {
  int ret;
  ret = libusb_init(&context);
  if(ret < 0){
    throw(AccessoryException(libusb_error_name(ret)));
  }
  lib_load = true;
}

Accessory::Accessory() {
    try {
		lib_init();
	} catch(AccessoryException &e){
		cerr << e.what() << endl;
	}
}


bool Accessory::check_for_accessory() {
    cout << "check_for_accessory" << endl;

    if(!lib_load) {
        try {
		    lib_init();
	    } catch(AccessoryException &e) {
		    cerr << e.what() << endl;
            return false;
	    }
    }

    if(isConfigured()) {
        cerr << "Device already configured" << endl;
        return true;
    }

    try {
        find_dev_vid_pid();
        cout << "find_dev_vid_pid: success" << endl;
        load_device();
        cout << "load_device: success" << endl;
        change_device();
        cout << "change_device: success" << endl;
        usleep(2000 * 1000);
        libusb_exit(context);
        lib_init();
        find_dev_vid_pid();
        cout << "find_dev_vid_pid: success" << endl;
        load_device();
        cout << "load_device: success" << endl;

    } catch(AccessoryException &e) {
        cerr << e.what() << endl;
        return false;
    }

    return isConfigured();

}

void Accessory::find_dev_vid_pid() {
    cout << "find_dev_vid_pid" << endl;

    libusb_device **devices;
	ssize_t listSize;
    listSize = libusb_get_device_list(context, &devices);

    if(listSize < 0){
		libusb_free_device_list(devices, 1);
        throw(AccessoryException(libusb_error_name(listSize)));
	}
	cout << listSize << " devices found" << endl;

	struct libusb_device_descriptor device_descriptor;
	int ret;
    bool found = false;

	for (int i = 0; i < listSize; i++){
		ret = libusb_get_device_descriptor(devices[i], &device_descriptor);
		if(ret < 0){
            libusb_free_device_list(devices, 1);
			throw(AccessoryException(libusb_error_name(ret)));
		}

		if(device_descriptor.idVendor == VID_GOOGLE){
            found = true;
			cout <<"Android device found" << endl;
            
            dev_vid = device_descriptor.idVendor;
            dev_pid = device_descriptor.idProduct;
            device = devices[i];
            libusb_free_device_list(devices, 1);
            break;
		}
	}

    if(!found)
        throw(AccessoryException("No android device detected"));

}

void Accessory::load_device() {
    cout << "load_device" << endl;
    int ret;

    ret = libusb_open(device, &handle);
    if (ret < 0) {
        cout << "Can't open device with libusb_open: " << libusb_error_name(ret) << endl;
        handle = libusb_open_device_with_vid_pid(context, dev_vid, dev_pid);
        if(handle == NULL) {
            cout << "Can't open device with vid/pid: " << libusb_error_name(ret) << endl;
            throw(AccessoryException("Can't open device"));
        }
    }

    // ?
    if(libusb_kernel_driver_active(handle, 0) != 0) {
        libusb_detach_kernel_driver(handle, 0);
    }

    ret = libusb_claim_interface(handle, 0);
    if(ret < 0){
        throw(AccessoryException(libusb_error_name(ret)));
    }
    else
        isClaim = true;

    usleep(20 * 1000);
}

void Accessory::change_device() {
    cout << "change_device" << endl;
    uint8_t buffer[2];
	int ret;
    int deviceProtocol;

    //request 51 (get protocol)
    cout << "request 51" << endl;

    ret = libusb_control_transfer(
        handle,
        192,             //request type
        51,                     //request
        0,                      //value
        0,                      //index      
        buffer,                 //data
        2,                      //lenght       
        DELAY_CONTROL);

    if(ret < 0)
		throw(AccessoryException(libusb_error_name(ret)));

    //verif protocol
    deviceProtocol = buffer[1] << 8 | buffer[0];
    if (deviceProtocol < AOA_PROTOCOL_MIN || deviceProtocol > AOA_PROTOCOL_MAX)
		throw(AccessoryException("Unsupported AOA protocol"));

    usleep(20 * 1000);

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
            DELAY_CONTROL);


		if(ret < 0) {
			cerr << "send string " << i <<  " call failed" << endl;
			throw(AccessoryException(libusb_error_name(ret)));
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
        DELAY_CONTROL);

	if(ret < 0)
		throw(AccessoryException(libusb_error_name(ret)));

    libusb_release_interface(handle, 0);
    isClaim = false;

    usleep(20 * 1000);
}


bool Accessory::isConfigured() {
    cout << "isConfigured" << endl;

    if(!lib_load) {
        cerr << "lib not loaded" << endl;
        return false;
    }

    if(handle == NULL) {
        cerr << "no handle" << endl;
        return false;
    }
    libusb_device *device = NULL;
    device = libusb_get_device(handle);

    if(device == NULL) {
        cerr << "can't get device" << endl;
        return false;
    }

    struct libusb_device_descriptor device_descriptor;
    int ret;
	ret = libusb_get_device_descriptor(device, &device_descriptor);

	if (ret < 0) {
		cerr << "error: libusb_get_device_descriptor: " << libusb_error_name(ret) << endl;
		return false;
	}

    switch(device_descriptor.idProduct) {
		case PID_AOA_ACC:
			return true;
        case PID_AOA_ACC_ADB:
            return true;
		default:
			return false;
		}
}

void Accessory::send_data(vector<uint8_t> data) {
    int ret = 0;
    int sent = 0;
    ret = libusb_bulk_transfer(handle, out_addr, data.data(), data.size(), &sent, 0);
    if(ret < 0 || (unsigned int)ret != data.size()) {
        throw(AccessoryException(libusb_error_name(ret)));
    }
}

void Accessory::read_data(vector<uint8_t>& data){
    int ret = 0;
    int size = 0;
    uint8_t buff[BUFFER_SIZE];
    data.clear();
    ret = libusb_bulk_transfer(handle, in_addr, buff, BUFFER_SIZE, &size, 0);
    cout << size << endl;
    if(ret < 0) {
        throw(AccessoryException(libusb_error_name(ret)));
    }
    data.assign(buff, buff + size);
}

Accessory::~Accessory(){
    

    if(handle != NULL) {
        if(isClaim)
            libusb_release_interface(handle, 0);
        libusb_close(handle);
    }
    if(context != NULL)
        libusb_exit(context);
}

