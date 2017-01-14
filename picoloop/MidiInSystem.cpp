#include "MidiInSystem.h"

MidiInSystem::MidiInSystem()
{
  iamOpen=0;
  lastOpenPortNumber=0;

}

MidiInSystem::~MidiInSystem()
{
}


void midiincallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  int msg;
  int msg0;
  int msg1;
  int msg2;
  int msg3;
  int msg4;
  int msg5;
  unsigned int nBytes = message->size();

  //exit(0);
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

  for ( unsigned int i=0; i<nBytes; i++ )
    {
      //std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
      printf("Byte:%d=%d\n",i,(int)message->at(i));
      msg=(int)message->at(i);
      if (i==0) msg0=(int)message->at(i);
      if (i==1) msg1=(int)message->at(i);
      if (i==2) msg2=(int)message->at(i);
      if (i==3) msg3=(int)message->at(i);
      if (i==4) msg4=(int)message->at(i);
      if (i==5) msg5=(int)message->at(i);
    }

  /*
     MMC are sysex commands
     ( from http://www.rncbc.org/drupal/node/92 ) 
     F0 7F xx 06 01 F7 = MMC STOP
     F0 7F xx 06 02 F7 = MMC PLAY
     F0 7F xx 06 04 F7 = MMC FFWD
     F0 7F xx 06 05 F7 = MMC REW
     F0 7F xx 06 06 F7 = MMC REC STROBE
     F0 7F xx 06 07 F7 = MMC REC STOP
     F0 7F xx 06 08 F7 = MMC REC PAUSE
     F0 7F xx 06 09 F7 = MMC PAUSE
   */
  
  // MMC STOP
  if (msg0==240  && 
      msg1==127  && 
      msg2==127  &&
      msg3==6    &&
      msg4==1    && // STOP
      msg5==247)
    {
      mmc_stop=1;
      counter_recv_midi_clock=0;
      counter_recv_midi_clock_six=0;
    }

  // MMC PLAY
  if (msg0==240  && 
      msg1==127  && 
      msg2==127  &&
      msg3==6    &&
      msg4==2    && // PLAY
      msg5==247)
    {
      mmc_start=1;
      counter_recv_midi_clock=0;
      counter_recv_midi_clock_six=0;
    }


  
  if (msg==248)
    {
      counter_recv_midi_clock++;
      if (counter_recv_midi_clock>5)
	{
	  counter_recv_midi_clock_six++;
	  counter_recv_midi_clock=0;
	  DPRINTF("*****MIDICLOCK 6 *****");
	}
      else
	{
	  DPRINTF("*****MIDICLOCK   *****");
	}
    }

//if ( nBytes > 0 )
  //std::cout << "stamp = " << deltatime << " ";
  
  //printf("        %d         \n\n",msg);
  // if ((i)
}



MidiInSystem & MidiInSystem::getInstance()
{
  static MidiInSystem instance;
  return instance;
}



bool MidiInSystem::init()
{
  try {
    rtmidiin = new RtMidiIn();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }
  iamOpen=0;
}

bool MidiInSystem::setupcallback()
{
  rtmidiin->setCallback( &midiincallback );
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


int MidiInSystem::getNumberOfMidiInputDevice()
{
 int nPorts = rtmidiin->getPortCount();
 return nPorts;
}

char * MidiInSystem::getMidiInputName(int deviceNumber)
{
  static char midiInputName[128];
  std::string tmpPortName;

  tmpPortName = rtmidiin->getPortName(deviceNumber);
  strcpy(midiInputName,tmpPortName.c_str());
  return midiInputName;

}


bool MidiInSystem::chooseMidiPortDeviceNumber(int deviceNumber)
{
  std::string tmpPortName;
  int portNumber=-1;
  unsigned int i = 0, nPorts = rtmidiin->getPortCount();
  tmpPortName = rtmidiin->getPortName(deviceNumber);


  std::cout << "  Will try to open midi in port #" << deviceNumber << " [" << tmpPortName << "]\n";
  if (iamOpen)
    rtmidiin->closePort();

  iamOpen=1;
  try {
    rtmidiin->openPort( deviceNumber );
    rtmidiin->ignoreTypes( false, false, false );
  }
  catch (RtMidiError &error) {
    // Handle the exception here
    error.printMessage();
    iamOpen=0;
  }
  
  if (iamOpen)
    return true;
  else
    return false;
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
	if (iamOpen)
	  rtmidiin->closePort();

	std::cout << "Opening port : " << portNumber << " [" << portName << "]\n";
	rtmidiin->openPort( portNumber );
	rtmidiin->ignoreTypes( false, false, false );
      }


  // Don't ignore sysex, timing, or active sensing messages.
  rtmidiin->ignoreTypes( false, false, false );


  return true;
}



bool MidiInSystem::closePort()
{
  if (iamOpen)
    {
      rtmidiin->closePort();
      return true;
    }
  return false;
}
