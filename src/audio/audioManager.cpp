#include "audioManager.h"


pa_simple *s;

bool init() {
    pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16NE;
	ss.channels = 1;
	ss.rate = 16000;
	
	s = pa_simple_new(NULL,               	// Use the default server.
					"AndroidMicLinux",    	// Our application's name.
					PA_STREAM_PLAYBACK,
					NULL,               	// Use the default device.
					"Android mic",          // Description of our stream.
					&ss,                	// Our sample format.
					NULL,               	// Use default channel map
					NULL,               	// Use default buffering attributes.
					NULL);               	// Ignore error code.
					
	if(s == NULL) {
		std::cout << "pa_simple = NULL" << std::endl;
		return false;
	}
    return true;
}

bool write(const void *data, size_t bytes) {

    int ret;
    int error;

    ret = pa_simple_write(s, data, bytes, &error);

    if(ret < 0) {
		std::cout << "error in pa_simple_write: ";
        pa_strerror(error);
		pa_simple_free(s);
        return false;
	}
    return true;
}





void finish() {
    pa_simple_free(s);
}