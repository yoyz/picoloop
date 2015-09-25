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
bool chooseMidiPort( RtMidiOut *rtmidi );

class MidiMsg
{
  MidiMsg();
  
};


int main( void )
{
  int i;
  int midiChannel=1-1; // channel 1 to 16
  RtMidiOut *midiout = 0;
  std::vector<unsigned char> message;

  // RtMidiOut constructor
  try {
    midiout = new RtMidiOut();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  // Call function to select port.
  try {
    if ( chooseMidiPort( midiout ) == false ) goto cleanup;
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    goto cleanup;
  }

  // Send out a series of MIDI messages.
  
  // Program change: 192, 5

  //message.push_back( 192+9 );
  /*
  message.push_back( 0xC0+0 );
  message.push_back( 8 );
  midiout->sendMessage( &message );

  SLEEP( 500 );



  message[0] = 0xF1;
  message[1] = 60;
  midiout->sendMessage( &message );
  */
  /*  
  // Control Change: 176, 7, 100 (volume)
  message[0] = 176;
  message[1] = 7;
  message.push_back( 100 );
  midiout->sendMessage( &message );
  */
  // Note On: 144, 64, 90
  for (i=0;i<88;i++)
    {
      //SLEEP( 10 );


      
      printf("note off : %d\n",i);
      message.push_back(0x80+midiChannel);
      message.push_back(i);
      message.push_back(0x0);

      //message[0] = 0x80+0;
      //message[1] = i;
      //message[2] = 0x00;
      //message.push_back( 3 );
      midiout->sendMessage( &message );
      message.clear();
      
      SLEEP( 1 );


    }

  // Note Off: 128, 64, 40
  /*
  message[0] = 128;
  message[1] = 64;
  message[2] = 40;
  midiout->sendMessage( &message );

  SLEEP( 500 );
  */
  // Control Change: 176, 7, 40

  /*
  message[0] = 176;
  message[1] = 7;
  message[2] = 40;
  midiout->sendMessage( &message );

  SLEEP( 500 );

  // Sysex: 240, 67, 4, 3, 2, 247
  message[0] = 240;
  message[1] = 67;
  message[2] = 4;
  message.push_back( 3 );
  message.push_back( 2 );
  message.push_back( 247 );
  midiout->sendMessage( &message );
  */
  // Clean up
 cleanup:
  delete midiout;

  return 0;
}

bool chooseMidiPort( RtMidiOut *rtmidi )
{
  /*
  std::cout << "\nWould you like to open a virtual output port? [y/N] ";

  std::string keyHit;
  std::getline( std::cin, keyHit );
  if ( keyHit == "y" ) {
    rtmidi->openVirtualPort();
    return true;
  }
  */

  std::string portName;
  unsigned int i = 0, nPorts = rtmidi->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No output ports available!" << std::endl;
    return false;
  }

  if ( nPorts == 1 ) {
    std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
  }
  else {
    for ( i=0; i<nPorts; i++ ) {
      portName = rtmidi->getPortName(i);
      std::cout << "  Output port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
      //i=1;
    } while ( i >= nPorts );
  }

  std::cout << "\n";
  rtmidi->openPort( i );

  return true;
}
