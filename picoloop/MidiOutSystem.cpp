#include "MidiOutSystem.h"




MidiOutSystem::MidiOutSystem()
{
  iamOpen=0;
  lastOpenPortNumber=0;
}


MidiOutSystem::~MidiOutSystem()
{
  int note;
  int i;
  for (i=0;i<88;i++)
    this->noteOff(0,i);

}


MidiOutSystem & MidiOutSystem::getInstance()
{
  static MidiOutSystem instance;
  return instance;
}

bool MidiOutSystem::init()
{
  try {
    rtmidiout = new RtMidiOut();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }
  iamOpen=0;
  return false; // I will need to "double check" this return type...
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

void MidiOutSystem::clock()
{
  mtx.Lock();

  message.push_back(0xF8);
  rtmidiout->sendMessage(&message);
  message.clear();  
  mtx.Unlock();
}


void MidiOutSystem::noteOn( int midiChan,int note,int velocity )
{
  if (this->checkChannel(midiChan))
    {
      mtx.Lock();

      message.push_back(0x90+midiChan);
      message.push_back(note);
      message.push_back(0x7c);

      mtx.Unlock();
      
    }
}


void MidiOutSystem::noteOff( int midiChan,int note)
{
  if (this->checkChannel(midiChan))
    {
      mtx.Lock();      
      message.push_back(0x80+midiChan);
      message.push_back(note);
      message.push_back(0x0);
      mtx.Unlock();
    }
}


void MidiOutSystem::cc( int midiChan,int cc,int value )
{
  if (this->checkChannel(midiChan))
    {
      mtx.Lock();
      message.push_back(0xB0+midiChan);
      message.push_back(cc);
      message.push_back(value);
      mtx.Unlock();
    }
}


int MidiOutSystem::msgSize()
{
  return message.size(); 
}

void MidiOutSystem::flushMsg()
{
  printf("*****************FLUSH:%lu\n",message.size());
  mtx.Lock();
  if (message.size())
    {
      rtmidiout->sendMessage(&message);
      message.clear();
    }
  mtx.Unlock();
}

int MidiOutSystem::getNumberOfMidiOutputDevice()
{
 int nPorts = rtmidiout->getPortCount()-1;
 return nPorts;
}

char * MidiOutSystem::getMidiOutputName(int deviceNumber)
{
  static char midiOutputName[128];
  std::string tmpPortName;

  tmpPortName = rtmidiout->getPortName(deviceNumber);
  strcpy(midiOutputName,tmpPortName.c_str());
  return midiOutputName;

}

bool MidiOutSystem::chooseMidiPortDeviceNumber(int deviceNumber)
{
  std::string tmpPortName;
  int portNumber=-1;
  unsigned int i = 0, nPorts = rtmidiout->getPortCount();
  tmpPortName = rtmidiout->getPortName(deviceNumber);


  std::cout << "  Will try to open midi out port #" << deviceNumber << " [" << tmpPortName << "]\n";
  if (iamOpen)
    rtmidiout->closePort();

  rtmidiout->openPort( deviceNumber );
  iamOpen=1;
  return true;
}

bool MidiOutSystem::chooseMidiPort( std::string portName )
{
  int portNumber=-1;
  std::string tmpPortName;

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
	if (iamOpen)
	  rtmidiout->closePort();
	rtmidiout->openPort( portNumber );
	lastOpenPortNumber=portNumber;
	iamOpen=1;
      }

  return true;
}


bool MidiOutSystem::closePort()
{
  if (iamOpen)
    {
      rtmidiout->closePort();
      return true;
    }
  return false;
}
