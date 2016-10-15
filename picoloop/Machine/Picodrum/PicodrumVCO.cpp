#include "PicodrumVCO.h"


PicodrumVCO::PicodrumVCO() : sineOsc1(), 
			     sineOsc2(), 
			     sawOsc1(), 
			     sawOsc2(), 
			     pulseOsc1(), 
			     pulseOsc2(), 
			     triangleOsc1(), 
			     triangleOsc2(),
			     smsineOsc1(), 
			     smsineOsc2(), 
			     smsawOsc1(), 
			     smsawOsc2(), 
			     smpulseOsc1(), 
			     smpulseOsc2(), 
			     smtriangleOsc1(), 
			     smtriangleOsc2(),
			     lfsrnoiseOsc1(), 
			     lfsrnoiseOsc2(), 
			     noiseOsc1(), 
			     noiseOsc2(), 
			     sineLfoOsc1(), 
			     sawLfoOsc1(),
			     pb(),
			     tanh_table(new Sint16[1024])   
			     //, noiseosc()
{
  DPRINTF("PicodrumVCO::PicodrumVCO()");
  float fi;
  int   ii;
  int   i;

  s1=NULL;
  s2=NULL;
  vcomix=64;

  lfo_counter=0;
  lfo_refresh=2;

  lfo_depth=0;
  lfo_depth_shift=20;

  lfo_speed=0;

  pb_depth=0;
  pb_speed=0;

  freqOsc1=0;
  freqOsc2=0;

  lfo_type=0;
  phase=0;

  for (i=0;i<1024;i++)
    {
      fi=i;
      fi=tanh(fi/1024);
      fi=fi*1024;
      ii=fi;
      tanh_table[i]=ii;
      //DPRINTF("tanh[%d]=%d\n",i,tanh_table[i]);
    }
}

void PicodrumVCO::init()
{
  DPRINTF("PicodrumVCO::init() begin s1:=0x%08.8X s2:=0x%08.8X",s1,s2);


  lfo_depth=0;
  lfo_depth_shift=20;

  lfo_speed=0;



  sineOsc1.setWaveForm(PICO_WAVETABLE_SINE);
  sineOsc2.setWaveForm(PICO_WAVETABLE_SINE);

  sawOsc1.setWaveForm(PICO_WAVETABLE_SAW);
  sawOsc2.setWaveForm(PICO_WAVETABLE_SAW);

  pulseOsc1.setWaveForm(PICO_WAVETABLE_PULSE);
  pulseOsc2.setWaveForm(PICO_WAVETABLE_PULSE);

  triangleOsc1.setWaveForm(PICO_WAVETABLE_TRGL);
  triangleOsc2.setWaveForm(PICO_WAVETABLE_TRGL);

  // waveTableSineOsc1.setWaveForm(PICO_WAVETABLE_SINE);
  // waveTableSineOsc2.setWaveForm(PICO_WAVETABLE_SINE);

  noiseOsc1.setWaveForm(PICO_WAVETABLE_NOISE);
  noiseOsc2.setWaveForm(PICO_WAVETABLE_NOISE);

  sineLfoOsc1.setWaveForm(PICO_WAVETABLE_SINE);
  sawLfoOsc1.setWaveForm(PICO_WAVETABLE_SAW);


  smsineOsc1.setWaveForm(PICO_WAVETABLE_SMSINE);
  smsineOsc2.setWaveForm(PICO_WAVETABLE_SMSINE);

  smsawOsc1.setWaveForm(PICO_WAVETABLE_SMSAW);
  smsawOsc2.setWaveForm(PICO_WAVETABLE_SMSAW);

  smpulseOsc1.setWaveForm(PICO_WAVETABLE_SMPULSE);
  smpulseOsc2.setWaveForm(PICO_WAVETABLE_SMPULSE);

  smtriangleOsc1.setWaveForm(PICO_WAVETABLE_SMTRGL);
  smtriangleOsc2.setWaveForm(PICO_WAVETABLE_SMTRGL);

  lfsrnoiseOsc1.setWaveForm( PICO_WAVETABLE_LFSRNOISE);
  lfsrnoiseOsc2.setWaveForm(PICO_WAVETABLE_LFSRNOISE);


  //lfo1=&sineLfoOsc1;
  lfo1=&sawLfoOsc1;

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


  DPRINTF("PicodrumVCO::init() end s1:=0x%08.8X s2:=0x%08.8X",s1,s2);
}


PicodrumVCO::~PicodrumVCO()
{
  DPRINTF("PicodrumVCO::~PicodrumVCO()");
}

int PicodrumVCO::checkSevenBitBoundarie(int val)
{
  if (val <= 0)
    return 0;
  if (val >= 127)
    return 127;
  return val;
}


void PicodrumVCO::setPicodrumVCOMix(int mix)
{

  vcomix=this->checkSevenBitBoundarie(mix);
}

void PicodrumVCO::setPicodrumVCOPhase(int ph)
{
  phase=this->checkSevenBitBoundarie(ph);
  s1->setPhase(phase);
  //s2->setPhase(phase);
  //fgfgfgs2->setPhase(phase);
}

void PicodrumVCO::setLfoType(int val)
{
  lfo_type=val;
}


void PicodrumVCO::setOscillator(int oscillator_number,int oscillator_type)
{
  int s1freq=s1->getFreq();
  int s2freq=s2->getFreq();

  if (oscillator_number %2==0)
    {
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SINE)      s1=&sineOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SAW)       s1=&sawOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_PULSE)     s1=&pulseOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_TRGL)      s1=&triangleOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMSINE)    s1=&smsineOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMSAW)     s1=&smsawOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMPULSE)   s1=&smpulseOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMTRGL)    s1=&smtriangleOsc1;

      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_NOISE)     s1=&noiseOsc1;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_LFSRNOISE) s1=&lfsrnoiseOsc1;

    }
  if (oscillator_number %2==1)
    {

      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SINE)      s2=&sineOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SAW)       s2=&sawOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_PULSE)     s2=&pulseOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_TRGL)      s2=&triangleOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMSINE)    s2=&smsineOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMSAW)     s2=&smsawOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMPULSE)   s2=&smpulseOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_SMTRGL)    s2=&smtriangleOsc2;

      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_NOISE)     s2=&noiseOsc2;
      if (oscillator_type%PICO_WAVETABLE_SIZE==PICO_WAVETABLE_LFSRNOISE) s2=&lfsrnoiseOsc2;
    }


  s1->setFreq(s1freq);
  s2->setFreq(s2freq);
}

void PicodrumVCO::setLfoDepth(int val)
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

void PicodrumVCO::setLfoSpeed(float val)
{
  lfo_speed=val;
  //lfo1->setFreq(val);
  lfo1->setFreq(val/2);
  //lfo1->setAmplitude(0);
}

void PicodrumVCO::setPitchBendDepth(int val)
{
  pb_depth=val;
  pb.setDepth(val);  
}

void PicodrumVCO::setPitchBendSpeed(int val)
{
  //pb.setDepth(lfo_depth);  
  pb_speed=val;
  pb.setSpeed(pb_speed);
}


void PicodrumVCO::reset()
{
  DPRINTF("PicodrumVCO::reset() this=0x%08.8X",this); // <==== FAILS allways the same this pointers
  s1->reset();
  s2->reset();
  pb.reset();
  //s2->setPhase(72);
  s1->setPhase(0);
  s2->setPhase(0);
  lfo1->reset();
  
  //this->setLfoDepth(0);
  //this->setLfoSpeed(0);
  //  lfo_counter=0;
}

Oscillator * PicodrumVCO::getOscillatorOne()
{
  DPRINTF("Oscillator * PicodrumVCO::getOscillatorOne() Oscillator=0x%08.8X",s1);
  return s1;
}

void PicodrumVCO::setNoteDetune(int nt,int dt)
{
  NoteFreq & NF = NoteFreq::getInstance();
  detune=dt;
  note=nt;

  freqOsc1=NF.getINoteFreq(note);
  freqOsc2=NF.getINoteFreq(note);


  //s1->setNoteDetune(note,64);
  //s2->setNoteDetune(note,dt);
  s1->setFreq(freqOsc1);
  s2->setFreq(freqOsc2);
  pb.setNote(note);

  DPRINTF("freqOsc1:%d",freqOsc1);
  /*
  s1->setSynthFreq(sfreq);
  s2->setSynthFreq(sfreq);
  */
  //s1->setFreq(freqOsc1);
  //s2->setFreq(freqOsc2);
  //  s2->setFreq(sfreq*3);
}



/*
void PicodrumVCO::setSynthFreq(float sfreq)
{

  freqOsc1=sfreq;
  freqOsc2=sfreq;
  s1->setFreq(freqOsc1);
  s2->setFreq(freqOsc2);
  //  s2->setFreq(sfreq*3);
}
*/



Sint16 PicodrumVCO::tick()
{
  Sint32 sa;
  Sint32 sb;
  Sint32 sc;
  Sint32 sinput1;
  Sint32 sinput2;
  Sint16 s;
  int    tmp=0;
  Sint32 pbtick;

  if (vcomix==0) vcomix=1;
  if (s1==NULL)
    { 
      DPRINTF("[s1 is NULL]"); 
      //  exit(1); 
    } 

  if (lfo_speed && lfo_depth)
    {
      s1->setFreq(lfo_speed);
      sinput1=s1->tick();
      //s2->setFreq(freqOsc1+abs(sinput1/((128-lfo_depth)*2)));
      if (pb_speed>0)
	{
	  pbtick=pb.tickNoteDetune();
	  s2->setFreq(pbtick+abs(sinput1/((128-lfo_depth)*2)));
	  //s2->setNoteDetune(note*128+detune+abs(sinput1/((128-lfo_depth)*1)));
	}
      else
	{
	  //s2->setFreq(freqOsc1+abs(sinput1/((128-lfo_depth)*2)));
	  //s2->setNoteDetune(note*128+detune+abs(sinput1/((128-lfo_depth)*1)));
	  s2->setNoteDetune(note*128+detune+abs(sinput1/((128-lfo_depth)*1)));
	  //s2->setNoteDetune(note*128+detune+abs((sinput1*tanh_table[lfo_depth*8])/1024));
	}
      sinput2=s2->tick();
      //sc=sinput2;
      sc=sinput2>>2;
    }
  else
    {
      if (pb_speed>0)
	{
	  pbtick=pb.tickNoteDetune();
	  s1->setFreq(pbtick);
	  s2->setFreq(pbtick);
	}

      sinput1=s1->tick();
      sa=(sinput1*((128-vcomix)));
      sinput2=s2->tick();
      sb=(sinput2*((vcomix)));
      //sc=(sa+sb)>>8;
      sc=(sa+sb)>>9;
    }


  if (sc> 32000) sc= 32000;
  if (sc<-32000) sc=-32000;



  s=sc;

  return s;
}

// Sint16 PicodrumVCO::tick()
// {
//   Sint32 sa;
//   Sint32 sb;
//   Sint32 sc;
//   Sint32 sinput1;
//   Sint32 sinput2;
//   Sint16 s;
//   int    tmp=0;
//   if (vcomix==0) vcomix=1;
//   if (s1==NULL)
//     { 
//       DPRINTF("[s1 is NULL]\n"); 
//       //  exit(1); 
//     } 

//   if (lfo_speed && lfo_depth)
//     {
//       s1->setFreq(lfo_speed);
//       sinput1=s1->tick();
//       s2->setFreq(freqOsc1+abs(sinput1/((128-lfo_depth)*2)));
//       sinput2=s2->tick();
//       //sc=sinput2;
//       sc=sinput2>>1;
//     }
//   else
//     {
//       sinput1=s1->tick();
//       sa=(sinput1*((128-vcomix)));
//       sinput2=s2->tick();
//       sb=(sinput2*((vcomix)));
//       //sc=(sa+sb)>>8;
//       sc=(sa+sb)>>9;
//     }


//   if (sc> 32000) sc= 32000;
//   if (sc<-32000) sc=-32000;



//   s=sc;

//   return s;
// }
