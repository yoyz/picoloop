
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "MidiOutSystem.h"
// Platform-dependent sleep routines.
#if defined(__WINDOWS_MM__)
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif






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

