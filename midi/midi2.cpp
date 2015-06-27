//*****************************************//
//  midiout.cpp
//  by Gary Scavone, 2003-2004.
//
//  Simple program to test MIDI output.
//
//*****************************************//

#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <stdio.h>

// Platform-dependent sleep routines.
#if defined(__WINDOWS_MM__)
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.


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





int main( void )
{
  //MidiOutSystem MOS;
  MidiOutSystem & MOS=MidiOutSystem::getInstance();

  int i;
  MOS.init();
  MOS.chooseMidiPort("TiMidity 128:0");
  sleep(1);
  MOS.noteOn(0,40,50);
  MOS.flushMsg();
  sleep(1);
  MOS.noteOff(0,40);
  MOS.flushMsg();

}

MidiOutSystem::MidiOutSystem()
{

}

MidiOutSystem & MidiOutSystem::getInstance()
{
  static MidiOutSystem instance;
  return instance;
}

bool MidiOutSystem::init()
{
  //RtMidiOut *midiout = 0;
  try {
    rtmidiout = new RtMidiOut();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }
  //midiChannel=0;
}

bool MidiOutSystem::checkChannel(int channel)
{
  if (channel<0 |
      channel>15)
    {
      printf("MidiOutSystem::noteOn wrong channel=%d\n",channel);
      return false;
    }
  return true;
}

void MidiOutSystem::noteOn( int midiChan,int note,int velocity )
{
  if (this->checkChannel(midiChan))
    {
      message.push_back(0x90+midiChan);
      message.push_back(note);
      message.push_back(0x7c);
    }
}

void MidiOutSystem::noteOff( int midiChan,int note)
{
  if (this->checkChannel(midiChan))
    {
      message.push_back(0x80+midiChan);
      message.push_back(note);
      message.push_back(0x0);
    }
}

void MidiOutSystem::flushMsg()
{
  rtmidiout->sendMessage(&message);
  message.clear();
}


bool MidiOutSystem::chooseMidiPort( std::string portName )
{
  std::string tmpPortName;
  int portNumber=-1;

  std::cout << "MidiOutSystem::chooseMidiPort(\""<< portName <<"\")\n";

  unsigned int i = 0, nPorts = rtmidiout->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No output ports available!" << std::endl;
    return false;
  }

  std::cout << "Displaying All Midi Port\n";
  for ( i=0; i<nPorts; i++ ) {
    tmpPortName = rtmidiout->getPortName(i);
    std::cout << "  Output port #" << i << ": [" << tmpPortName << "]\n";
  }
  std::cout << "\n";

    for ( i=0; i<nPorts; i++ ) {
      tmpPortName = rtmidiout->getPortName(i);
      if (tmpPortName==portName)
	{
	  std::cout << "The midi port was found : [" << portName << "]\n";
	  portNumber=i;
	}
    }

    if (portNumber!=-1)
      {
	std::cout << "Opening port : " << portNumber << " [" << portName << "]\n";
	rtmidiout->openPort( portNumber );
      }

  return true;
}

// bool chooseMidiPort( RtMidiOut *rtmidi )
// {
//   std::string portName;
//   unsigned int i = 0, nPorts = rtmidi->getPortCount();
//   if ( nPorts == 0 ) {
//     std::cout << "No output ports available!" << std::endl;
//     return false;
//   }

//   if ( nPorts == 1 ) {
//     std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
//   }
//   else {
//     for ( i=0; i<nPorts; i++ ) {
//       portName = rtmidi->getPortName(i);
//       std::cout << "  Output port #" << i << ": " << portName << '\n';
//     }

//     do {
//       //std::cout << "\nChoose a port number: ";
//       //std::cin >> i;
//       i=1;
//     } while ( i >= nPorts );
//   }

//   std::cout << "\n";
//   rtmidi->openPort( i );

//   return true;
// }
