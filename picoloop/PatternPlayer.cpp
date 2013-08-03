using namespace std;

#include "PatternPlayer.h"
#include "AudioEngine.h"
#include "Wave.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "Note.h"

AudioEngine ae;
Pattern P;
PatternElement PE;
Wave cowbell;
//Synth beeper;
Instrument inst;

void openaudio()
{
  //beeper.initialize(1000,440);
  //inst.setSynth();
  //inst.assignSynth(beeper);
  ae.set_instrument(inst);
  printf("Before\n");
  ae.openAudio();
  printf("After\n");
}


int seq()
{
  int seqnum=0;
  AudioMixer & am=ae.getAudioMixer();
  Track & t=ae.getAudioMixer().getTrack();
  Machine & m=ae.getAudioMixer().getTrack().getMachine();
  int tempo=120;
  //  am=ae.getAudioMixer();
  //  return 0;
  while (seqnum<16)
    {
      if (ae.getTick()>10)
	{
	  seqnum++;
	  printf("seqnum:%d\n",seqnum);
	  ae.setTick(0);
	}
      printf("loop\n");    
      //am.setSynthFreq(880);
      //am.setSynthFreq(880);
      //ae.getAudioMixer().setSynthFreq(440);
      //t.setSynthFreq(880);
      m.setSynthFreq(880);
      sleep(1);
      m.setSynthFreq(1760);
      //t.setSynthFreq(1660);
      //am.setSynthFreq(1660);
      //ae.getAudioMixer().setSynthFreq(880);
      sleep(2);
      /*
	ae.setSynthFreq(880);      
	sleep(1);
	ae.setSynthFreq(660);      
	sleep(1); 
	ae.setSynthFreq(440);
	sleep(1);
	ae.setSynthFreq(220);      
	sleep(1);
      */
    }
}

void setPattern()
{
  P.setPatternSize(16);
  //PE = P.getPatternElement(0);
  //PE.setNote(23);
  P.getPatternElement(0).setNote(12);
  P.getPatternElement(0).setTrig(true);
  P.getPatternElement(3).setNote(22);
  P.getPatternElement(8).setNote(7);
  P.getPatternElement(8).setTrig(true);

  P.print();
}

int main()
{
  cowbell.loadWave("808-cowbell.wav");

  openaudio();
  seq();
  //sleep(10);
  //PE.print();
}
