#ifndef __MIDIOUTSYSTEM__
#define __MIDIOUTSYSTEM__
#include "RtMidi.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
//#include <mutex>
#include "SysMutex.h" // nostromo lgpt class for mutex



class MidiOutSystem
{
  public:
  static MidiOutSystem & getInstance();
  //MidiOutSystem();
  bool init();
  int getNumberOfMidiOutputDevice();
  char * getMidiOutputName(int deviceNumber);
  bool checkChannel(int midiChan);
  void clock();
  void noteOn(int midiChan,int note,int velocity);
  void noteOff(int midiChan,int note);
  void cc(int midiChan,int cc,int value);
  int  msgSize();
  void flushMsg();
  //bool chooseMidiPort( RtMidiOut *rtmidi );
  bool chooseMidiPort(std::string portName);
  bool chooseMidiPortDeviceNumber(int deviceNumber);
  bool closePort();

  private:
  MidiOutSystem();
  ~MidiOutSystem();
  RtMidiOut *rtmidiout;
  std::vector<unsigned char> message;
  int midiChannel;
  //std::mutex lock_a;

  SysMutex mtx;

  int lastOpenPortNumber;
  int iamOpen;
  
};


#endif
