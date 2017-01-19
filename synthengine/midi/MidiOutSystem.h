#ifndef __MIDIOUTSYSTEM__
#define __MIDIOUTSYSTEM__
#include "RtMidi.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>


class MidiOutSystem
{
  public:
  static MidiOutSystem & getInstance();
  //MidiOutSystem();
  bool init();
  bool checkChannel(int midiChan);
  void noteOn(int midiChan,int note,int velocity);
  void noteOff(int midiChan,int note);
  void flushMsg();
  //bool chooseMidiPort( RtMidiOut *rtmidi );
  bool chooseMidiPort(std::string portName);

  private:
  MidiOutSystem();
  //~MidiOutSystem();
  RtMidiOut *rtmidiout;
  std::vector<unsigned char> message;
  int midiChannel;
};


#endif
