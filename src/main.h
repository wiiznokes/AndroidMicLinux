#ifndef MAIN_H_
#define MAIN_H_


// g++ -o prog ./src/*.cpp -lusb-1.0 -lpulse-simple -lpulse && sudo ./prog

#include <iostream>
#include <libusb-1.0/libusb.h>

#include "audioManager.h"
#include "connexion.h"
#include "transfer.h"


#define BUFFER_SIZE 2560


#endif
