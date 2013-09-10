using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include "Note.h"
#include "PatternReader.h"
#include "PatternPlayer.h"
#include "AudioEngine.h"
#include "Wave.h"
#include "MonoMixer.h"

AudioEngine ae;

Pattern P0;
Pattern P1;

PatternElement PE0;
PatternElement PE1;

//MonoMixer MM0;
//MonoMixer MM1;

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
  Machine m0;
  Machine m1;

  int tempo=60;
  int step=0;      // current step
  int nbcb=0;      // current nb audio callback 
  int last_nbcb=0; // nb audio callback from last step
  int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*tempo); // Weird ?




  AudioMixer & am=ae.getAudioMixer();
  //Track & t0=ae.getAudioMixer().getTrack(0);

  //  m0.setSawOsc();
  //m0.setSawOsc();
  m0.setFuzzyPulseOsc();
  //  m1.setSawOsc();

  //  m0.setSineOsc();
  //m1.setSawOsc();
    m1.setSineOsc();

  m0.setSynthFreq(0);
  m1.setSynthFreq(0);

  //Track     & t0=ae.getAudioMixer().getTrack(0);
  MonoMixer & mm0=ae.getAudioMixer().getTrack(0).getMonoMixer();
  MonoMixer & mm1=ae.getAudioMixer().getTrack(1).getMonoMixer();

  mm0.setInput(&m0);
  mm1.setInput(&m1);

  ae.startAudio();
  //Machine & m0=ae.getAudioMixer().getTrack(0).getMachine();

  //  Track & t1=ae.getAudioMixer().getTrack(1);
  //  Machine & m1=ae.getAudioMixer().getTrack(1).getMachine();


  
  //m0.setSawOsc();
  //  m1.setSineOsc();
  //m0.setSawOsc();
  //  m0.setFuzzyPulseOsc();
  //  m1.setSawOsc();
  //m1.setSineOsc();
  //m0.setSawOsc();
  //m1.setSawOsc();

  //  am=ae.getAudioMixer();
  //  return 0;
  while (true)
    {
      nbcb=ae.getNbCallback();
      if (nbcb-last_nbcb>nb_cb_ch_step)
	{
	  
	  //if (step%3==0) { m.setSineOsc(); }
	  //if (step%3==1) { m.setSawOsc(); }
	  //if (step%3==2) { m.setFuzzyPulseOsc(); }
	  
	  //m0.setSawOsc(); 
	  //m1.setSawOsc(); 
	  //m.setFuzzyPulseOsc();
	  //m0.setSineOsc();
	  //m1.setSineOsc();
	  
	  if (step==16) { step=0; }
	  printf("STEP:%d\n",step);	  

	  //printf("loop\n");    
	  last_nbcb=nbcb;
	  if (P0.getPatternElement(step).getTrig()==true)
	    {
	      float f=P0.getPatternElement(step).getNoteFreq();
	      int   i=f;
		
	      printf("%f %d \n",P0.getPatternElement(step).getNoteFreq(),i);
	      m0.setSynthFreq(i);
	      m0.getADSR().reset();
	      m0.getOscillator()->reset();
	      //m.setSynthFreq(1200);
	    }
	  else
	    {
	      //m.setSynthFreq(800);
	      printf("m.setSynthFreq(0);\n");
	      m0.setSynthFreq(0);
	    }

	  if (P1.getPatternElement(step).getTrig()==true)
	    {
	      float f=P1.getPatternElement(step).getNoteFreq();
	      int   i=f;
		
	      printf("%f %d \n",P1.getPatternElement(step).getNoteFreq(),i);
	      m1.setSynthFreq(i);
	      m1.getADSR().reset();
	      m1.getOscillator()->reset();
	      //m.setSynthFreq(1200);
	    }
	  else
	    {
	      //m.setSynthFreq(800);
	      printf("m.setSynthFreq(0);\n");
	      m1.setSynthFreq(0);
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
  PatternReader PR;
  string fileName="data.pic";
  //PR.setFileName("data.pic");
  PR.setFileName(fileName);
  PR.readPatternData(1,1,P0);
  PR.readPatternData(1,2,P1);

  /*
  P0.setPatternSize(16);
  //PE = P.getPatternElement(0);
  P0.getPatternElement(0).setNote(C3);
  P0.getPatternElement(0).setTrig(true);

  P0.getPatternElement(1).setNote(F3);
  P0.getPatternElement(1).setTrig(true);

  P0.getPatternElement(3).setNote(A3P);
  P0.getPatternElement(3).setTrig(true);

  P0.getPatternElement(5).setNote(D3P);
  P0.getPatternElement(5).setTrig(true);

  P0.getPatternElement(8).setTrig(true);
  P0.getPatternElement(8).setNote(G3);

  P0.getPatternElement(9).setTrig(true);
  P0.getPatternElement(9).setNote(C3);

  P0.getPatternElement(13).setTrig(true);
  P0.getPatternElement(13).setNote(D4P);

  P0.print();
  printf("\n");

  P1.setPatternSize(16);
  //PE = P.getPatternElement(0);
  P1.getPatternElement(0).setNote(C4);
  P1.getPatternElement(0).setTrig(true);

  P1.getPatternElement(1).setNote(G4P);
  P1.getPatternElement(1).setTrig(true);

  P1.getPatternElement(3).setNote(D5P);
  P1.getPatternElement(3).setTrig(true);

  P1.getPatternElement(5).setNote(C4);
  P1.getPatternElement(5).setTrig(true);

  P1.getPatternElement(8).setNote(D4P);
  P1.getPatternElement(8).setTrig(true);

  P1.getPatternElement(9).setNote(C4);
  P1.getPatternElement(9).setTrig(true);

  P1.getPatternElement(13).setNote(D3P);
  P1.getPatternElement(13).setTrig(true);

  P1.print();
  */
}

int main()
{
  //string wave="808-cowbell.wav";
  char * str="808-cowbell.wav";
  cowbell.loadWave(str);
  setPattern();
  openaudio();
  seq();
  //sleep(10);
  //PE.print();
}
