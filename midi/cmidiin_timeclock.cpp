#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

struct timeval a0,a1;
double   accumul=0;
double   bpm=0;
int    inc_diff=0;
time_t difft=0;

void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  int msg;
  unsigned int nBytes = message->size();

  for ( unsigned int i=0; i<nBytes; i++ )
    {
      std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
      msg=(int)message->at(i);
    }
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << " ";

  //printf("        %d         \n\n",msg);
  // if ((i)
  if (msg==248)
    printf("0xF8 Timing Clock (Sys Realtime) ");

  if (msg==248)
    {
      gettimeofday(&a0, NULL);
      difft=((a0.tv_sec * 1000000 + a0.tv_usec) - 
	     (a1.tv_sec * 1000000 + a1.tv_usec));
      printf("[  %ld ] ", difft);
      inc_diff++;
      accumul=accumul+difft;
      if (inc_diff>=24)
	{
	  bpm=((1/(accumul/1000000))*44100)/735;
	  //printf(" <%f> <%f>",accumul,(1.0/accumul)*44100*(2400/2));
	  printf(" <%f> <%f>",accumul,bpm);
	  accumul=0;
	  inc_diff=0;
	}
      a1=a0;

      //a3=a2;
      //a2=a1;
      //a1=a0;
    }

  std::cout << std::endl;
}
int main()
{
  RtMidiIn *midiin = new RtMidiIn();
  // Check available ports.
  // precision of cout for "double" 
  std::cout.setf(std::ios::fixed,std::ios::floatfield);
  std::cout.precision(3);

  unsigned int nPorts = midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }
  midiin->openPort( 2 );
  // Set our callback function. This should be done immediately after
  // opening the port to avoid having incoming messages written to the
  // queue.
  midiin->setCallback( &mycallback );
  // Don't ignore sysex, timing, or active sensing messages.
  midiin->ignoreTypes( false, false, false );
  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);
  // Clean up
 cleanup:
  delete midiin;
  return 0;
}
