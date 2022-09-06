#include "main.h"



using namespace std;

int main() {

	Accessory *accessory = new Accessory();
	vector<uint8_t> sharedBuffer;

	try {
		accessory->lib_init();
	} catch(AccessoryException &e){
		cerr << e.what() << endl;
		exit(1);
	}

	if(!accessory->check_for_accessory()) {
		cerr << "No accessory detected" << endl;
		delete accessory;
		exit(-1);
	}
	
  	try {
    	while(1) {
      		accessory->read_data(sharedBuffer);
      		cout << sharedBuffer.data() << endl;
    	}
  	} catch(AccessoryException &e) {
		cerr << e.what() << endl;
  	}
  	delete accessory;
}
