#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <iostream>
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>


#define BUFSIZE 2560

namespace audioManager 
{
    bool init();
    bool write(const void *data, size_t bytes);
    void finish();
}

#endif