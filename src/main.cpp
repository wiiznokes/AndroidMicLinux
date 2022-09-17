#include "main.h"
#include <thread> 


using namespace std;

static void loopCancelation(bool* loop){
	char c = 'a';
	while(c != 'q') {
		cout << "Je suis dans loopCancelation" << endl;
		scanf("%c", &c);
		cout << "scanf = " << c << endl;
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
	cout << "J'ai quitté la boucle" << endl;


  	delete accessory;
}
