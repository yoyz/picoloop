#ifndef __MIDIINSYSTEM__
#define __MIDIINSYSTEM__
#include "RtMidi.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "Master.h"
#include "SysMutex.h" // nostromo lgpt class for mutex

/*
  Purpose : 
  Open the midi interface
   One for read  midiin
   One for write midiout

   Setup callback for read
    handle this kind of message

    128 0x80 Note-off 2 key velocity
    144 0x90 Note-on 2 key veolcity
    160 0xA0 Aftertouch 2 key touch
    176 0xB0 Continuous controller 2 controller # controller value MIDI-CC
    192 0xC0 Patch change 2 instrument # 
    208 0xD0 Channel Pressure 1 pressure
    224 0xE0 Pitch bend 2 lsb (7 bits) msb (7 bits)


    0xF0 (non-musical commands) 
    240 0xF0 start of system exclusive message variable
    241 0xF1 MIDI Time Code Quarter Frame (Sys Common)
    242 0xF2 Song Position Pointer (Sys Common)
    243 0xF3 Song Select (Sys Common) 
    244 0xF4 ??? 
    245 0xF5 ??? 
    246 0xF6 Tune Request (Sys Common) 
    247 0xF7 end of system exclusive message 0
    248 0xF8 Timing Clock (Sys Realtime) 
    249 0xFA Start (Sys Realtime) 
    250 0xFB Continue (Sys Realtime) 
    251 0xFC Stop (Sys Realtime) 
    252 0xFD ??? 
    253 0xFE Active Sensing (Sys Realtime) 
    254 0xFF System Reset (Sys Realtime) 
  
  implement the main callback which produce the stream 
 */
extern int counter_recv_midi_clock;     // send n clock and decrement the counter each time
extern int counter_recv_midi_clock_six; // send n clock and decrement the counter each tim
extern int mmc_stop;                    // reset the step sequencer to step 0
extern int mmc_start;                   // 
extern int sequencer_play;              //

class MidiInSystem
{
 public:
  static MidiInSystem & getInstance();
  bool init();
  int getNumberOfMidiInputDevice();
  char * getMidiInputName(int deviceNumber);
  bool checkChannel(int midiChan);
  bool chooseMidiPort(std::string portName);
  bool chooseMidiPortDeviceNumber(int deviceNumber);
  bool closePort();

  bool setupcallback();

  private:
  MidiInSystem();
  ~MidiInSystem();
  RtMidiIn *rtmidiin;
  std::vector<unsigned char> message;

  SysMutex mtx;

  int lastOpenPortNumber;
  int iamOpen;


};

#endif
