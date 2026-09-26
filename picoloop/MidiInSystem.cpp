#include "MidiInSystem.h"
#ifdef __ANDROID__
#include "AndroidJNIUtils.h"
#endif

MidiInSystem::MidiInSystem()
{
  iamOpen=0;
  lastOpenPortNumber=0;
#ifdef __ANDROID__
  androidDevice=0;
  androidMidiDevice=0;
  androidOutputPort=0;
  androidPollThread=0;
  androidPollRunning=0;
  androidCallbackArmed=false;
#endif
}

MidiInSystem::~MidiInSystem()
{
#ifdef __ANDROID__
  this->closePort();
#endif
}


void midi_botoomhalf(unsigned char msg)
{
  static unsigned char msg0;
  static unsigned char msg1;
  static unsigned char msg2;
  static unsigned char msg3;
  static unsigned char msg4;
  static unsigned char msg5;
  static unsigned char msg6;
  static unsigned char clock;
  //  printf("botoom %d\n",msg);

  // CLOCK 0xF8	Timing Clock (Sys Realtime)
  if (msg==248 && msg0==0)
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
      msg0=0; msg1=0; msg2=0; msg3=0; msg4=0; msg5=0; msg6=0;
      return;
    }
  // STOP 0xFC	Stop (Sys Realtime)
  if (msg==252 && msg0==0)
    {
      DPRINTF("Receiving MIDI sys Stop\n");
      mmc_stop=1;
      msg0=0; msg1=0; msg2=0; msg3=0; msg4=0; msg5=0; msg6=0;
      return;
    }
  // START 0xFA	Start (Sys Realtime)
  if (msg==250 && msg0==0)
    {
      DPRINTF("Receiving MIDI sys Start\n");
      mmc_start=1;
      msg0=0; msg1=0; msg2=0; msg3=0; msg4=0; msg5=0; msg6=0;
      return;
    }
  // PAUSE 0xFA	Start (Sys Realtime)
  if (msg==251 && msg0==0)
    {
      DPRINTF("Receiving MIDI sys continue\n");
      mmc_continue=1;
      msg0=0; msg1=0; msg2=0; msg3=0; msg4=0; msg5=0; msg6=0;
      return;
    }




  if (msg==0 &&                                                          msg==240 )
    { msg0=240; return; }
  else if (msg0==240 &&                                                  msg==127 ) 
    { msg1=127; return; }
  else if (msg0==240 && msg1==127 &&                                     msg==127 ) 
    { msg2=127; return; }
  else if (msg0==240 && msg1==127 && msg2==127 &&                        msg==6 ) 
    { msg3=6; return; }
  else if (msg0==240 && msg1==127 && msg2==127 && msg3==6 &&              msg>0)
    { msg4=msg; return; }
  else if (msg0==240 && msg1==127 && msg2==127 && msg3==6 &&  msg4>0 &&   msg==127)
    { msg5=msg; } // We have found our MMC but we do not have managed it
  else
    {   msg0=0; msg1=0; msg2=0; msg3=0;msg4=0;msg5=0;msg6=0; }

  // MMC STOP
    if (msg0==240  && 
      msg1==127  && 
      msg2==127  &&
      msg3==6    &&
      msg4==1    && // STOP
      msg5==247)
    {
      DPRINTF("Receiving MIDI MMC Stop\n");
      mmc_stop=1;
      counter_recv_midi_clock=0;
      counter_recv_midi_clock_six=0;
      msg0=0; msg1=0; msg2=0; msg3=0;msg4=0;msg5=0;msg6=0;
    }
  
  // MMC PLAY
  if (msg0==240  && 
      msg1==127  && 
      msg2==127  &&
      msg3==6    &&
      msg4==2    && // PLAY
      msg5==247)
    {
      DPRINTF("Receiving MIDI MMC Start\n");
      mmc_start=1;
      counter_recv_midi_clock=0;
      counter_recv_midi_clock_six=0;
      msg0=0; msg1=0; msg2=0; msg3=0;msg4=0;msg5=0;msg6=0;
    }

}

void midiincallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  unsigned char msg;
  unsigned char msg0;
  unsigned char msg1;
  unsigned char msg2;
  unsigned char msg3;
  unsigned char msg4;
  unsigned char msg5;
  unsigned int nBytes = message->size();

  //exit(0);
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

  for ( unsigned int i=0; i<nBytes; i++ )
    {
      //std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
      DPRINTF("Byte:%d=%d %2x\n",i,(unsigned char)message->at(i),(unsigned char)message->at(i));
      msg=(unsigned char)message->at(i);
      midi_botoomhalf(msg);
    }
}

/*
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

  
  //   MMC are sysex commands
  //   ( from http://www.rncbc.org/drupal/node/92 ) 
  //   F0 7F xx 06 01 F7 = MMC STOP
  //   F0 7F xx 06 02 F7 = MMC PLAY
  //   F0 7F xx 06 04 F7 = MMC FFWD
  //   F0 7F xx 06 05 F7 = MMC REW
  //   F0 7F xx 06 06 F7 = MMC REC STROBE
  //   F0 7F xx 06 07 F7 = MMC REC STOP
  //   F0 7F xx 06 08 F7 = MMC REC PAUSE
  //   F0 7F xx 06 09 F7 = MMC PAUSE
 
  
  // MMC STOP
  if (msg0==240  && 
      msg1==127  && 
      msg2==127  &&
      msg3==6    &&
      msg4==1    && // STOP
      msg5==247)
    {
      printf("Receiving MIDI MMC Stop\n");
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
      printf("Receiving MIDI MMC Start\n");
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
*/


MidiInSystem & MidiInSystem::getInstance()
{
  static MidiInSystem instance;
  return instance;
}



#ifndef __ANDROID__
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
  return false;
}

bool MidiInSystem::setupcallback()
{
  rtmidiin->setCallback( &midiincallback );
  return false;
}
#endif // !__ANDROID__


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


#ifndef __ANDROID__
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
#endif // !__ANDROID__


#ifdef __ANDROID__

// AMidiOutputPort_receive() is a poll-only API - there is no OS-level
// callback - so a dedicated thread polls it at a short, fixed interval and
// feeds each byte straight into midi_botoomhalf(), same as what RtMidi's
// callback (midiincallback(), above) does on desktop builds.
int androidMidiInPollThreadFunc(void * data)
{
  MidiInSystem * self = (MidiInSystem*)data;
  uint8_t buffer[3];
  int32_t opcode;
  size_t numBytes;
  int64_t timestamp;

  while (self->androidPollRunning)
    {
      if (self->androidOutputPort)
	{
	  ssize_t numMessages = AMidiOutputPort_receive(self->androidOutputPort, &opcode,
							 buffer, sizeof(buffer), &numBytes, &timestamp);
	  if (numMessages>0 && opcode==AMIDI_OPCODE_DATA)
	    {
	      for (size_t i=0;i<numBytes;i++)
		midi_botoomhalf(buffer[i]);
	    }
	}
      SDL_Delay(2);
    }
  return 0;
}

int MidiInSystem::getNumberOfMidiInputDevice()
{
  androidInputPorts.clear();
  std::vector<AndroidMidiPortInfo> ports = AndroidMidi_ListPorts();
  for (size_t i=0;i<ports.size();i++)
    if (!ports[i].isInput) // a device's OUT port is where WE receive from it -> input
      androidInputPorts.push_back(ports[i]);
  return (int)androidInputPorts.size();
}

char * MidiInSystem::getMidiInputName(int deviceNumber)
{
  static char midiInputName[128];
  midiInputName[0]=0;
  if (deviceNumber>=0 && deviceNumber<(int)androidInputPorts.size())
    strncpy(midiInputName, androidInputPorts[deviceNumber].name.c_str(), 127);
  return midiInputName;
}

bool MidiInSystem::chooseMidiPortDeviceNumber(int deviceNumber)
{
  if (deviceNumber<0 || deviceNumber>=(int)androidInputPorts.size())
    return false;

  if (iamOpen)
    this->closePort();

  const AndroidMidiPortInfo & port = androidInputPorts[deviceNumber];

  jobject device = AndroidMidi_OpenDevice(port.deviceId);
  if (!device)
    return false;

  bool didAttach=false;
  JNIEnv * env = AcquireJNIEnv(&didAttach);
  if (!env)
    {
      AndroidMidi_CloseDevice(device);
      return false;
    }
  media_status_t status = AMidiDevice_fromJava(env, device, &androidMidiDevice);
  ReleaseJNIEnv(didAttach);

  if (status!=AMEDIA_OK)
    {
      AndroidMidi_CloseDevice(device);
      androidMidiDevice=0;
      return false;
    }

  status = AMidiOutputPort_open(androidMidiDevice, port.portIndex, &androidOutputPort);
  if (status!=AMEDIA_OK)
    {
      AMidiDevice_release(androidMidiDevice);
      androidMidiDevice=0;
      AndroidMidi_CloseDevice(device);
      return false;
    }

  androidDevice=device;
  lastOpenPortNumber=deviceNumber;
  iamOpen=1;

  if (androidCallbackArmed)
    this->setupcallback();

  return true;
}

bool MidiInSystem::chooseMidiPort(std::string portName)
{
  this->getNumberOfMidiInputDevice();
  for (size_t i=0;i<androidInputPorts.size();i++)
    if (androidInputPorts[i].name==portName)
      return this->chooseMidiPortDeviceNumber((int)i);
  return false;
}

bool MidiInSystem::init()
{
  iamOpen=0;
  return false;
}

// Starts the polling thread (see androidMidiInPollThreadFunc above). Unlike
// RtMidi's setCallback(), this needs a port already open to have anything
// to poll - if chooseMidiPortDeviceNumber() hasn't run yet, just remember
// the request and start polling once it does.
bool MidiInSystem::setupcallback()
{
  androidCallbackArmed=true;
  if (!iamOpen || androidPollThread)
    return false;

  androidPollRunning=1;
  androidPollThread = SDL_CreateThread(androidMidiInPollThreadFunc, "picoloopmidiin", this);
  return false;
}

bool MidiInSystem::closePort()
{
  if (!iamOpen)
    return false;

  if (androidPollThread)
    {
      androidPollRunning=0;
      SDL_WaitThread((SDL_Thread*)androidPollThread, NULL);
      androidPollThread=0;
    }
  if (androidOutputPort)
    {
      AMidiOutputPort_close(androidOutputPort);
      androidOutputPort=0;
    }
  if (androidMidiDevice)
    {
      AMidiDevice_release(androidMidiDevice);
      androidMidiDevice=0;
    }
  if (androidDevice)
    {
      AndroidMidi_CloseDevice(androidDevice);
      androidDevice=0;
    }
  iamOpen=0;
  return true;
}

#endif // __ANDROID__
