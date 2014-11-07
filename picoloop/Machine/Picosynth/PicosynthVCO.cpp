#include "PicosynthVCO.h"


PicosynthVCO::PicosynthVCO() : sineOsc1(), 
			       sineOsc2(), 
			       sawOsc1(), 
			       sawOsc2(), 
			       pulseOsc1(), 
			       pulseOsc2(), 
			       triangleOsc1(), 
			       triangleOsc2(), 
			       waveTableSineOsc1(), 
			       waveTableSineOsc2(), 
			       noiseOsc1(), 
			       noiseOsc2(), 
			       sineLfoOsc1(), 
			       sawLfoOsc1()
			       //, noiseosc()
{
  printf("PicosynthVCO::PicosynthVCO()\n");
  s1=NULL;
  s2=NULL;
  vcomix=64;

  lfo_counter=0;
  lfo_refresh=2;

  lfo_depth=0;
  lfo_depth_shift=20;

  lfo_speed=0;

  freqOsc1=0;
  freqOsc2=0;
  
  note=0;
  detune=64;
}

void PicosynthVCO::init()
{
  printf("PicosynthVCO::init() begin s1:=0x%08.8X s2:=0x%08.8X\n",s1,s2);


  lfo_depth=0;
  lfo_depth_shift=20;

  lfo_speed=0;


  /*
  sineOsc1.init();
  sineOsc2.init();

  sawOsc1.init();
  sawOsc2.init();

  pulseOsc1.init();
  pulseOsc2.init();

  triangleOsc1.init();
  triangleOsc2.init();

  waveTableSineOsc1.init();
  waveTableSineOsc2.init();

  noiseOsc1.init();
  noiseOsc2.init();

  sineLfoOsc1.init();
  sawLfoOsc1.init();
  */

  sineOsc1.setWaveForm(PICO_WAVETABLE_SINE);
  sineOsc2.setWaveForm(PICO_WAVETABLE_SINE);

  sawOsc1.setWaveForm(PICO_WAVETABLE_SAW);
  sawOsc2.setWaveForm(PICO_WAVETABLE_SAW);

  pulseOsc1.setWaveForm(PICO_WAVETABLE_PULSE);
  pulseOsc2.setWaveForm(PICO_WAVETABLE_PULSE);

  triangleOsc1.setWaveForm(PICO_WAVETABLE_TRGL);
  triangleOsc2.setWaveForm(PICO_WAVETABLE_TRGL);

  waveTableSineOsc1.setWaveForm(PICO_WAVETABLE_SINE);
  waveTableSineOsc2.setWaveForm(PICO_WAVETABLE_SINE);

  noiseOsc1.setWaveForm(PICO_WAVETABLE_NOISE);
  noiseOsc2.setWaveForm(PICO_WAVETABLE_NOISE);

  sineLfoOsc1.setWaveForm(PICO_WAVETABLE_SINE);
  sawLfoOsc1.setWaveForm(PICO_WAVETABLE_SAW);


  lfo1=&sineLfoOsc1;
  //lfo1=&sawLfoOsc1;

  lfo1->setFreq(0);
  lfo1->setAmplitude(32);

  //  s1 = &sineosc;
  s1 = &pulseOsc1;
  s2 = &sawOsc2;

  s1->setFreq(0);
  s1->setAmplitude(32);
  //  s2 = &pulseosc;
  s2->setFreq(0);
  s2->setAmplitude(32);


  printf("PicosynthVCO::init() end s1:=0x%08.8X s2:=0x%08.8X\n",s1,s2);
}


PicosynthVCO::~PicosynthVCO()
{
  printf("PicosynthVCO::~PicosynthVCO()\n");
}

int PicosynthVCO::checkSevenBitBoundarie(int val)
{
  if (val <= 0)
    return 0;
  if (val >= 127)
    return 127;
  return val;
}


void PicosynthVCO::setVCOMix(int mix)
{

  vcomix=this->checkSevenBitBoundarie(mix);
}

void PicosynthVCO::setVCOPhase(int ph)
{

  phase=this->checkSevenBitBoundarie(ph);
}


void PicosynthVCO::setOscillator(int oscillator_number,int oscillator_type)
{
  int s1freq=s1->getFreq();
  int s2freq=s2->getFreq();

  if (oscillator_number %2==0)
    {
      if (oscillator_type%5==0) s1=&sineOsc1;
      if (oscillator_type%5==1) s1=&sawOsc1;
      if (oscillator_type%5==2) s1=&pulseOsc1;
      if (oscillator_type%5==3) s1=&triangleOsc1;
      if (oscillator_type%5==4) s1=&noiseOsc1;

    }
  if (oscillator_number %2==1)
    {
      if (oscillator_type%5==0) s2=&sineOsc2;
      if (oscillator_type%5==1) s2=&sawOsc2;
      if (oscillator_type%5==2) s2=&pulseOsc2;
      if (oscillator_type%5==3) s2=&triangleOsc2;
      if (oscillator_type%5==4) s2=&noiseOsc2;
    }
  s1->setFreq(s1freq);
  s2->setFreq(s2freq);
}

void PicosynthVCO::setLfoDepth(int val)
{
  lfo_depth=val;
  if (val > 0    && val <=  4  )  { lfo_depth=val ; lfo_depth_shift=33;        }
  if (val > 4    && val <=  8  )  { lfo_depth=val ; lfo_depth_shift=32;        }
  if (val > 8    && val <= 12  )  { lfo_depth=val ; lfo_depth_shift=31;        } 
  if (val > 12   && val <= 16  )  { lfo_depth=val ; lfo_depth_shift=30;        }
  if (val > 16   && val <= 20  )  { lfo_depth=val ; lfo_depth_shift=29;        }
  if (val > 20   && val <= 24  )  { lfo_depth=val ; lfo_depth_shift=28;        }
  if (val > 24   && val <= 28  )  { lfo_depth=val ; lfo_depth_shift=27;        }
  if (val > 28   && val <= 32  )  { lfo_depth=val ; lfo_depth_shift=26;        }
  if (val > 32   && val <= 36  )  { lfo_depth=val ; lfo_depth_shift=25;        }
  if (val > 36   && val <= 40  )  { lfo_depth=val ; lfo_depth_shift=24;        }
  if (val > 40   && val <= 44  )  { lfo_depth=val ; lfo_depth_shift=23;        }
  if (val > 44   && val <= 48  )  { lfo_depth=val ; lfo_depth_shift=22;        }
  if (val > 48   && val <= 54  )  { lfo_depth=val ; lfo_depth_shift=21;        }
  if (val > 54   && val <= 58  )  { lfo_depth=val ; lfo_depth_shift=20;        }
  if (val > 58   && val <= 62  )  { lfo_depth=val ; lfo_depth_shift=19;        }
  if (val > 62   && val <= 66  )  { lfo_depth=val ; lfo_depth_shift=18;        }
  if (val > 66   && val <= 70  )  { lfo_depth=val ; lfo_depth_shift=17;        }
  if (val > 70   && val <= 74  )  { lfo_depth=val ; lfo_depth_shift=16;        }
  if (val > 74   && val <= 78  )  { lfo_depth=val ; lfo_depth_shift=15;        }
  if (val > 78   && val <= 82  )  { lfo_depth=val ; lfo_depth_shift=14;        }
  if (val > 82   && val <= 86  )  { lfo_depth=val ; lfo_depth_shift=13;        }
  if (val > 86   && val <= 90  )  { lfo_depth=val ; lfo_depth_shift=12;        }
  if (val > 90   && val <= 94  )  { lfo_depth=val ; lfo_depth_shift=11;        }
  if (val > 94   && val <= 98  )  { lfo_depth=val ; lfo_depth_shift=10;        }
  if (val > 98   && val <= 102 )  { lfo_depth=val ; lfo_depth_shift=9;         }
  if (val > 102  && val <= 106 )  { lfo_depth=val ; lfo_depth_shift=8;         }
  if (val > 106  && val <= 110 )  { lfo_depth=val ; lfo_depth_shift=7;         }
  if (val > 110  && val <= 114 )  { lfo_depth=val ; lfo_depth_shift=6;         }
  if (val > 114  && val <= 118 )  { lfo_depth=val ; lfo_depth_shift=5;         }
  if (val > 118  && val <= 122 )  { lfo_depth=val ; lfo_depth_shift=4;         }
  if (val > 122  && val <= 128 )  { lfo_depth=val ; lfo_depth_shift=3;         } 


}

void PicosynthVCO::setLfoSpeed(float val)
{
  //lfo_speed=val/24.0;
  lfo1->setFreq(val);
  //lfo1->setAmplitude(0);
}

void PicosynthVCO::reset()
{
  printf("PicosynthVCO::reset() this=0x%08.8X\n",this); // <==== FAILS allways the same this pointers
  s1->reset();
  s2->reset();
  s2->setPhase(phase);
  lfo1->reset();
  
  //this->setLfoDepth(0);
  //this->setLfoSpeed(0);
  //  lfo_counter=0;
}

Oscillator * PicosynthVCO::getOscillatorOne()
{
  printf("Oscillator * PicosynthVCO::getOscillatorOne() Oscillator=0x%08.8X\n",s1);
  return s1;
}



void PicosynthVCO::setNoteDetune(int nt,int dt)
{
  NoteFreq & NF = NoteFreq::getInstance();
  detune=dt;

  s1->setNoteDetune(nt,64);
  s2->setNoteDetune(nt,dt);
  freqOsc1=NF.getINoteFreq(nt);
  freqOsc2=NF.getINoteFreq(nt);
  note=nt;
  /*
  s1->setSynthFreq(sfreq);
  s2->setSynthFreq(sfreq);
  */
  //s1->setFreq(freqOsc1);
  //s2->setFreq(freqOsc2);
  //  s2->setFreq(sfreq*3);
}


/*
void PicosynthVCO::setFSynthFreq(float f_freq)
{

  f_freqOsc1=f_freq;
  f_freqOsc2=f_freq;

  //s1->setSynthFreq(sfreq);
  //s2->setSynthFreq(sfreq);

  s1->setFFreq(ffreqOsc1);
  s2->setFFreq(ffreqOsc2);

}
*/



Sint16 PicosynthVCO::tick()
{
  //  printf("PicosynthVCO::tick() this=0x%08.8X\n",this); 
  // return s1->tick()+s2->tick();
  Sint32 sa;
  Sint32 sb;
  Sint32 sc;
  Sint16 s;
  int    tmp=0;
  int    tmp2=0;

  if (vcomix==0) vcomix=1;
  if (s1==NULL)
    { 
      printf("[s1 is NULL]\n"); 
      //  exit(1); 
    } 

  if (lfo_depth>0 &&
      lfo_speed>0)
    {
      tmp2=lfo1->tick();
      tmp=((tmp2>>7)*lfo_depth>>7)+64;
  
      s1->setNoteDetune(note,tmp);
      s2->setNoteDetune(note,tmp);
    }


  sa=(s1->tick()*((128-vcomix)));
  sb=(s2->tick()*((vcomix)));

  sc=(sa+sb)>>7;


  s=sc;
  return s;
}
