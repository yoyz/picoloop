#include "MidiInSystem.h"

MidiInSystem::MidiInSystem()
{
}

MidiInSystem::~MidiInSystem()
{
}


MidiInSystem & MidiInSystem::getInstance()
{
  static MidiInSystem instance;
  return instance;
}



bool MidiInSystem::init()
{
  //RtMidiOut *midiout = 0;
  try {
    rtmidiin = new RtMidiIn();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }
  
}

bool MidiInSystem::checkChannel(int channel)
{
  if (channel<0 |
      channel>15)
    {
      printf("MidiInSystem::checkChannel wrong channel=%d\n",channel);
      return false;
    }
  return true;
}


bool MidiInSystem::chooseMidiPort( std::string portName )
{
  std::string tmpPortName;
  int portNumber=-1;
  
  std::cout << "MidiInSystem::chooseMidiPort(\""<< portName <<"\")\n";

  unsigned int i = 0, nPorts = rtmidiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No input ports available!" << std::endl;
    return false;
  }

  std::cout << "Displaying All Midi In Port\n";
  for ( i=0; i<nPorts; i++ ) {
    tmpPortName = rtmidiin->getPortName(i);
    std::cout << "  Input port #" << i << ": [" << tmpPortName << "]\n";
  }
  std::cout << "\n";

    for ( i=0; i<nPorts; i++ ) {
      tmpPortName = rtmidiin->getPortName(i);
      if (tmpPortName==portName)
	{
	  std::cout << "The midi in port was found : [" << portName << "]\n";
	  portNumber=i;
	}
    }

    if (portNumber!=-1)
      {
	std::cout << "Opening port : " << portNumber << " [" << portName << "]\n";
	rtmidiin->openPort( portNumber );
      }


  // Don't ignore sysex, timing, or active sensing messages.
  rtmidiin->ignoreTypes( false, false, false );


  return true;
}



