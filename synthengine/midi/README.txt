g++ -D__LINUX_ALSA__ -c RtMidi.cpp -o RtMidi.o
g++ -D__LINUX_ALSA__ RtMidi.cpp midiprobe.cpp -o midiprobe  -lasound -lpthread
g++ -D__LINUX_ALSA__ RtMidi.cpp midinoteonnoteoff.cpp -o a.out  -lasound -lpthread
g++ qmidiin.cpp RtMidi.o -o qmidiin -lasound -lpthread
g++ cmidiin_timeclock.cpp RtMidi.o -o cmidiin_timeclock -lasound -lpthread
g++ cmidiout_timeclock.cpp RtMidi.o -o cmidiin_timeclock -lasound -lpthread
amidi -l
