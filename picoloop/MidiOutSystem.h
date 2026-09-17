#ifndef __MIDIOUTSYSTEM__
#define __MIDIOUTSYSTEM__
#ifndef __ANDROID__
#include "RtMidi.h"
#endif
#include <iostream>
#include <cstdlib>
#include <stdio.h>
//#include <mutex>
#include "SysMutex.h" // nostromo lgpt class for mutex

#ifdef __ANDROID__
#include <amidi/AMidi.h>
#include <jni.h>
#include "AndroidMidiBridge.h"
#endif


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
#ifndef __ANDROID__
  RtMidiOut *rtmidiout;
#endif
  std::vector<unsigned char> message;
  int midiChannel;
  //std::mutex lock_a;

  SysMutex mtx;

  int lastOpenPortNumber;
  int iamOpen;

#ifdef __ANDROID__
  // android.media.midi, via AndroidMidiBridge.cpp/MidiBridge.kt - see
  // getNumberOfMidiOutputDevice()/chooseMidiPortDeviceNumber() in
  // MidiOutSystem.cpp for how these get populated/used.
  std::vector<AndroidMidiPortInfo> androidOutputPorts;
  jobject       androidDevice;
  AMidiDevice * androidMidiDevice;
  AMidiInputPort * androidInputPort; // "input" from the device's point of view = where we send TO
#endif
};


#endif
