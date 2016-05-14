g++ -D__LINUX_ALSA__ RtMidi.cpp midinoteonnoteoff.cpp -o a.out  -lasound -lpthread
g++ qmidiin.cpp RtMidi.o -o qmidiin -lasound -lpthread
