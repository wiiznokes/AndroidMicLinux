#include "main.h"
#include <thread> 


using namespace std;

static void loopCancelation(bool* loop){
	char c = 'a';
	while(c != 'q') {
		scanf("%c", &c);
	}
	*loop = false;
}

int main() {

	Accessory *accessory = new Accessory();
	vector<uint8_t> sharedBuffer;

	

	if(!accessory->check_for_accessory()) {
		delete accessory;
		exit(-1);
	}
	cout << "accessory configured!" << endl;
	accessory->findEndpoint();

	static bool loop = true;
	cout << "start recording audio from Android, press q to stop" << endl;
	thread cancelationThread(loopCancelation, &loop);
	
	while(loop == true) {
		try {
			accessory->read_data(sharedBuffer);
			cout << sharedBuffer.data() << endl;
		} catch(AccessoryException &e) {
			cerr << e.what() << endl;
			usleep(2000 * 1000);
		}
  	}
	cancelationThread.join();



  	delete accessory;
}
