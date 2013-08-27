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
  AudioMixer & am=ae.getAudioMixer();
  Track & t=ae.getAudioMixer().getTrack();
  Machine & m=ae.getAudioMixer().getTrack().getMachine();
  int tempo=60;
  int step=0;      // current step
  int nbcb=0;      // current nb audio callback 
  int last_nbcb=0; // nb audio callback from last step
  int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*tempo);
  
  m.setSawOsc();

  //  am=ae.getAudioMixer();
  //  return 0;
  while (true)
    {
      nbcb=ae.getNbCallback();
      if (nbcb-last_nbcb>nb_cb_ch_step)
	{
	  
	  if (step%3==0) { m.setSineOsc(); }
	  if (step%3==1) { m.setSawOsc(); }
	  if (step%3==2) { m.setFuzzyPulseOsc(); }
	  
	  //m.setSawOsc(); 
	  //m.setFuzzyPulseOsc();
	  //m.setSineOsc();
	  
	  if (step==16) { step=0; }
	  printf("STEP:%d\n",step);	  

	  //printf("loop\n");    
	  last_nbcb=nbcb;
	  if (P.getPatternElement(step).getTrig()==true)
	    {
	      float f=P.getPatternElement(step).getNoteFreq();
	      int   i=f;
		
	      printf("%f %d \n",P.getPatternElement(step).getNoteFreq(),i);
	      m.setSynthFreq(i);
	      //m.setSynthFreq(1200);
	    }
	  else
	    {
	      //m.setSynthFreq(800);
	      printf("m.setSynthFreq(0);\n");
	      m.setSynthFreq(0);
	    }
	  step++;  
	}
      //am.setSynthFreq(880);
      //am.setSynthFreq(880);
      //ae.getAudioMixer().setSynthFreq(440);
      //t.setSynthFreq(880);
      //m.setSynthFreq(880);
      //printf("loop\n");
      usleep(1000);
      //t.setSynthFreq(1660);
      //am.setSynthFreq(1660);
      //ae.getAudioMixer().setSynthFreq(880);
      //sleep(2);
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
  P.getPatternElement(0).setNote(41);
  P.getPatternElement(0).setTrig(true);

  P.getPatternElement(1).setNote(72);
  P.getPatternElement(1).setTrig(true);


  P.getPatternElement(3).setNote(51);
  P.getPatternElement(3).setTrig(true);

  P.getPatternElement(5).setNote(62);
  P.getPatternElement(5).setTrig(true);


  P.getPatternElement(8).setTrig(true);
  P.getPatternElement(8).setNote(69);

  P.getPatternElement(9).setTrig(true);
  P.getPatternElement(9).setNote(48);

  P.getPatternElement(13).setTrig(true);
  P.getPatternElement(13).setNote(38);



  P.print();
}

int main()
{
  cowbell.loadWave("808-cowbell.wav");
  setPattern();
  openaudio();
  seq();
  //sleep(10);
  //PE.print();
}
