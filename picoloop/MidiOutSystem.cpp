#include "Master.h"
#include "MidiOutSystem.h"
#ifdef __ANDROID__
#include "AndroidJNIUtils.h"
#endif



MidiOutSystem::MidiOutSystem()
{
  iamOpen=0;
  lastOpenPortNumber=0;
#ifdef __ANDROID__
  androidDevice=0;
  androidMidiDevice=0;
  androidInputPort=0;
#endif
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

#ifndef __ANDROID__
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
#endif

#ifdef __ANDROID__
bool MidiOutSystem::init()
{
  iamOpen=0;
  return false;
}
#endif

bool MidiOutSystem::checkChannel(int channel)
{
  if (channel<0 |
      channel>15)
    {
      DPRINTF("MidiOutSystem::noteOn wrong channel=%d",channel);
      return false;
    }
  return true;
}

#ifndef __ANDROID__
void MidiOutSystem::clock()
{
  mtx.Lock();

  message.push_back(0xF8);
  rtmidiout->sendMessage(&message);
  message.clear();
  mtx.Unlock();
}
#endif

#ifdef __ANDROID__
void MidiOutSystem::clock()
{
  mtx.Lock();
  message.push_back(0xF8);
  if (iamOpen && androidInputPort)
    AMidiInputPort_send(androidInputPort, message.data(), message.size());
  message.clear();
  mtx.Unlock();
}
#endif


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

#ifndef __ANDROID__
void MidiOutSystem::flushMsg()
{
  DPRINTF("FLUSH:%lu",message.size());
  mtx.Lock();
  if (message.size())
    {
      rtmidiout->sendMessage(&message);
      message.clear();
    }
  mtx.Unlock();
}
#endif

#ifdef __ANDROID__
void MidiOutSystem::flushMsg()
{
  DPRINTF("FLUSH:%lu",message.size());
  mtx.Lock();
  if (message.size() && iamOpen && androidInputPort)
    {
      AMidiInputPort_send(androidInputPort, message.data(), message.size());
    }
  message.clear();
  mtx.Unlock();
}
#endif

#ifndef __ANDROID__
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


  //std::cout << "  Will try to open midi out port #" << deviceNumber << " [" << tmpPortName << "]\n";
  DPRINTF("Will try to open midi out port %d %s",deviceNumber,tmpPortName.c_str());
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

  DPRINTF("Choose midi out port %s",portName.c_str());
  //std::cout << "MidiOutSystem::chooseMidiPort(\""<< portName <<"\")\n";

  unsigned int i = 0, nPorts = rtmidiout->getPortCount();
  if ( nPorts == 0 )
    {
      //std::cout << "No output ports available!" << std::endl;
      DPRINTF("No output ports available!");
      return false;
  }

  //std::cout << "Displaying All Midi Port\n";
  DPRINTF("Displaying All Midi Port");
  for ( i=0; i<nPorts; i++ )
    {
      tmpPortName = rtmidiout->getPortName(i);
      DPRINTF("Output port %d %s",i,tmpPortName.c_str());
      //std::cout << "  Output port #" << i << ": [" << tmpPortName << "]\n";
    }
  //std::cout << "\n";

    for ( i=0; i<nPorts; i++ ) {
      tmpPortName = rtmidiout->getPortName(i);
      if (tmpPortName==portName)
	{
	  DPRINTF("The midi port was not found : %s",portName.c_str());
	  //std::cout << "The midi port was found : [" << portName << "]\n";
	  portNumber=i;
	}
    }

    if (portNumber!=-1)
      {
	DPRINTF("Opening midi port %d %s",portNumber,portName.c_str());
	//std::cout << "Opening port : " << portNumber << " [" << portName << "]\n";
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
#endif // !__ANDROID__


#ifdef __ANDROID__

int MidiOutSystem::getNumberOfMidiOutputDevice()
{
  androidOutputPorts.clear();
  std::vector<AndroidMidiPortInfo> ports = AndroidMidi_ListPorts();
  for (size_t i=0;i<ports.size();i++)
    if (ports[i].isInput) // a device's IN port is where WE send to it -> output
      androidOutputPorts.push_back(ports[i]);
  return (int)androidOutputPorts.size();
}

char * MidiOutSystem::getMidiOutputName(int deviceNumber)
{
  static char midiOutputName[128];
  midiOutputName[0]=0;
  if (deviceNumber>=0 && deviceNumber<(int)androidOutputPorts.size())
    strncpy(midiOutputName, androidOutputPorts[deviceNumber].name.c_str(), 127);
  return midiOutputName;
}

bool MidiOutSystem::chooseMidiPortDeviceNumber(int deviceNumber)
{
  if (deviceNumber<0 || deviceNumber>=(int)androidOutputPorts.size())
    return false;

  if (iamOpen)
    this->closePort();

  const AndroidMidiPortInfo & port = androidOutputPorts[deviceNumber];

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

  status = AMidiInputPort_open(androidMidiDevice, port.portIndex, &androidInputPort);
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
  return true;
}

bool MidiOutSystem::chooseMidiPort(std::string portName)
{
  this->getNumberOfMidiOutputDevice();
  for (size_t i=0;i<androidOutputPorts.size();i++)
    if (androidOutputPorts[i].name==portName)
      return this->chooseMidiPortDeviceNumber((int)i);
  return false;
}

bool MidiOutSystem::closePort()
{
  if (!iamOpen)
    return false;

  if (androidInputPort)
    {
      AMidiInputPort_close(androidInputPort);
      androidInputPort=0;
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
