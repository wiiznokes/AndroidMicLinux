#include "main.h"



using namespace std;

int main() {

	Accessory *accessory = new Accessory();
	vector<uint8_t> sharedBuffer;

	

	if(!accessory->check_for_accessory()) {
		delete accessory;
		exit(-1);
	}
	cout << "accessory configured!" << endl;
	accessory->findEndpoint();

	while(1) {
		try {
			accessory->read_data(sharedBuffer);
			cout << sharedBuffer.data() << endl;
		} catch(AccessoryException &e) {
			cerr << e.what() << endl;
			usleep(2000 * 1000);
		}
  	}
	



  	delete accessory;
}
