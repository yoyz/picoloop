#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define   BPM_TO_SMOOTH      32

struct timeval a0,a1;
double   accumul=0;
double   bpm=0;

double   smoothed_bpm_v[BPM_TO_SMOOTH];
double   smoothed_bpm;
int      number_of_bpm=0;
int      inc_diff=0;
time_t   difft=0;

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
	  number_of_bpm++;
	  if (number_of_bpm>BPM_TO_SMOOTH) number_of_bpm=0;
	  bpm=((1/(accumul/1000000))*44100)/735;
	  smoothed_bpm_v[number_of_bpm]=bpm;
	  smoothed_bpm=0;
	  for (int i=0;i<BPM_TO_SMOOTH;i++)
	    smoothed_bpm+=smoothed_bpm_v[i];
	 
	  //printf(" <%f> <%f>",accumul,(1.0/accumul)*44100*(2400/2));
	  printf(" <%f> <%f> <%f>",accumul,bpm,smoothed_bpm/BPM_TO_SMOOTH);
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
  unsigned int i = 0, nPorts = midiin->getPortCount();
  std::string portName;

  // Check available ports.
  // precision of cout for "double" 
  std::cout.setf(std::ios::fixed,std::ios::floatfield);
  std::cout.precision(3);

  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }

  if ( nPorts == 1 ) {
    std::cout << "\nOpening " << midiin->getPortName() << std::endl;
  }
  else {
    for ( i=0; i<nPorts; i++ ) {
      portName = midiin->getPortName(i);
      std::cout << "  Input port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
      //i=1;
    } while ( i >= nPorts );
  }


  midiin->openPort( i );
  // Set our callback function. This should be done immediately after
  // opening the port to avoid having incoming messages written to the
  // queue.
  midiin->setCallback( &mycallback );
  // Don't ignore sysex, timing, or active sensing messages.
  midiin->ignoreTypes( false, false, false );
  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);
  std::cin.get(input);
  // Clean up
 cleanup:
  delete midiin;
  return 0;
}
