#include "accessory.hpp"


void Accessory::lib_init(){
  int ret;
  ret = libusb_init(NULL);
  if(ret != 0){
    throw(AccessoryException(libusb_error_name(ret)));
  }
  lib_load = true;
}

Accessory::Accessory() {
    lib_load = false;
}


bool Accessory::check_for_accessory() {

    if(isAccessory()) {
        cerr << "Device already in accessory mode" << endl;
        return true;
    }

    try {
        find_pid_vid();
        load_device();
        change_device();

    } catch(AccessoryException &e) {
        cerr << e.what() << endl;
        return false;
    }

    return isAccessory();

}

void Accessory::find_pid_vid() {

    libusb_device **devices;
	ssize_t listSize;
    listSize = libusb_get_device_list(NULL, &devices);

    if(listSize < 0){
		libusb_free_device_list(devices, 1);
        throw(AccessoryException(libusb_error_name(listSize)));
	}
	cout << listSize << " devices found" << endl;

	struct libusb_device_descriptor device_descriptor;
	int ret;

	for (int i = 0; i < listSize; i++){
		ret = libusb_get_device_descriptor(devices[i], &device_descriptor);
		if(ret < 0){
            libusb_free_device_list(devices, 1);
			throw(AccessoryException(libusb_error_name(ret)));
		}

		if(device_descriptor.idVendor == VID_GOOGLE){
			cout <<"Android device found" << endl;
            
            dev_vid = device_descriptor.idVendor;
            dev_pid = device_descriptor.idProduct;
            libusb_free_device_list(devices, 1);
            break;
		}
	}

}

void Accessory::load_device() {

    handle = libusb_open_device_with_vid_pid(NULL, dev_vid, dev_pid);

    if (handle == NULL)
        throw(AccessoryException("No Device Found"));
    
    int ret = libusb_claim_interface(handle, 0);

    if(ret != 0){
        throw(AccessoryException(libusb_error_name(ret)));
    }
}

void Accessory::change_device() {
    unsigned char ioBuffer[2];
	int ret;
    int deviceProtocol;

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
        DELAY_CONTROL);

    if(ret != 0) {
		libusb_close(handle);
		throw(AccessoryException(libusb_error_name(ret)));
	}

    //verif protocol
    deviceProtocol = ioBuffer[1] << 8 | ioBuffer[0];
    if (deviceProtocol < AOA_PROTOCOL_MIN || deviceProtocol > AOA_PROTOCOL_MAX) {
		libusb_close(handle);
		throw(AccessoryException("Unsupported AOA protocol"));
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
            DELAY_CONTROL);


		if(0 > ret) {
			cerr << "send string " << i <<  " call failed" << endl;
			libusb_close(handle);
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

	if(ret < 0) {
		libusb_close(handle);
		throw(AccessoryException(libusb_error_name(ret)));
	}
    libusb_close(handle);
}


bool Accessory::isAccessory() {
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
		cerr << "error: libusb_get_device_descriptor " << libusb_error_name(ret) << endl;
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

void Accessory::send_data(vector<uint8_t> data){
  int ret = 0;
  int sent = 0;
  ret = libusb_bulk_transfer(handle, out_addr, data.data(), data.size(), &sent, 0);
  if(ret < 0 || (unsigned int)ret != data.size()){
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
  if(ret < 0){
    throw(AccessoryException(libusb_error_name(ret)));
  }
  data.assign(buff, buff + size);
}

Accessory::~Accessory(){
  libusb_release_interface(handle, 0);
  libusb_close(handle);
  libusb_exit(NULL);
}