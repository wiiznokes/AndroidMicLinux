g++ -Wall -D__UNIX_JACK__ -o maximilian cpp/commandline/main.cpp cpp/commandline/RtAudio.cpp cpp/commandline/player.cpp src/maximilian.cpp `pkg-config --cflags --libs jack` -lpthread


g++ -Wall -D__LINUX_ALSA__ -o maximilian cpp/commandline/main.cpp cpp/commandline/RtAudio.cpp cpp/commandline/player.cpp src/maximilian.cpp -lasound -lpthread