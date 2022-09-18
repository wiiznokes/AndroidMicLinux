#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
 
#include <spa/param/audio/format-utils.h>
#include <pipewire/pipewire.h>
 
#define M_PI_M2 ( M_PI + M_PI )
 
#define DEFAULT_RATE            44100
#define DEFAULT_CHANNELS        2
#define DEFAULT_VOLUME          0.7
#define BUFSIZE 2560
 
struct data {
        struct pw_main_loop *loop;
        struct pw_stream *stream;
 
        double accumulator;
};


namespace audioManager 
{
    
}

#endif