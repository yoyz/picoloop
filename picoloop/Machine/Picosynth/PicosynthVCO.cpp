#include "PicosynthVCO.h"


PicosynthVCO::PicosynthVCO() : osc1(),
			       osc2(),
			       oscLfo1(),
			       pb()
{
  DPRINTF("PicosynthVCO::PicosynthVCO()");
  s1=NULL;
  s2=NULL;
  lfo1=NULL;
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
  
  note=0;
  detune=64;


  lfo_type=0;
}

void PicosynthVCO::init()
{
  DPRINTF("PicosynthVCO::init() begin s1:=0x%08.8X s2:=0x%08.8X",s1,s2);


  lfo_depth=0;
  lfo_depth_shift=20;

  lfo_speed=0;

  pb_depth=0;
  pb_speed=0;

  osc1.setWaveForm(PICO_WAVETABLE_SAW);
  osc2.setWaveForm(PICO_WAVETABLE_SAW);
  oscLfo1.setWaveForm(PICO_WAVETABLE_SINE);

  lfo1=&oscLfo1;
  lfo1->setFreq(0);
  lfo1->setAmplitude(32);

  s1 = &osc1;
  s1->setFreq(0);
  s1->setAmplitude(32);

  s2 = &osc2;
  s2->setFreq(0);
  s2->setAmplitude(32);


  DPRINTF("PicosynthVCO::init() end s1:=0x%08.8X s2:=0x%08.8X",s1,s2);
}


PicosynthVCO::~PicosynthVCO()
{
  DPRINTF("PicosynthVCO::~PicosynthVCO()");
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

void PicosynthVCO::setLfoType(int val)
{
  lfo_type=val;
}


void PicosynthVCO::setOscillator(int oscillator_number,int oscillator_type)
{
  int s1freq=s1->getFreq();
  int s2freq=s2->getFreq();
  
  if (oscillator_number %2==0)
    {
      //if (oscillator_type%PICO_WAVETABLE_SIZE>=0)
	s1->setWaveForm(oscillator_type%PICO_WAVETABLE_SIZE);
    }
  if (oscillator_number %2==1)
    {
      //if (oscillator_type%PICO_WAVETABLE_SIZE>=0)
	s2->setWaveForm(oscillator_type%PICO_WAVETABLE_SIZE);
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


void PicosynthVCO::setLfoSpeed(int val)
{
  lfo_speed=val;
  lfo1->setFreq(lfo_speed/2);
  //lfo1->setAmplitude(0);
}


void PicosynthVCO::setPitchBendDepth(int val)
{
  pb_depth=val;
  pb.setDepth(val);  
}

void PicosynthVCO::setPitchBendSpeed(int val)
{
  //pb.setDepth(lfo_depth);  
  pb_speed=val;
  pb.setSpeed(pb_speed);
}




void PicosynthVCO::reset()
{
  DPRINTF("PicosynthVCO::reset() this=0x%08.8X",this); // <==== FAILS allways the same this pointers
  s1->reset();
  s2->reset();
  s2->setPhase(phase);
  pb.reset();
  lfo1->reset();
  
  //this->setLfoDepth(0);
  //this->setLfoSpeed(0);
  //lfo_counter=0;
}

Oscillator * PicosynthVCO::getOscillatorOne()
{
  DPRINTF("Oscillator * PicosynthVCO::getOscillatorOne() Oscillator=0x%08.8X",s1);
  return s1;
}



//void PicosynthVCO::setNoteDetune(int nt,int dt)
void PicosynthVCO::setNoteDetune(int nt1,int nt2,int dt)
{
  NoteFreq & NF = NoteFreq::getInstance();
  detune=dt;

  // detune works from 0-127 value
  // some detune is done too on the PB and the LFO in the ->tick() function
  s1->setNoteDetune(nt1,64);
  s2->setNoteDetune(nt2,dt);

  freqOsc1=NF.getINoteFreq(nt1);
  freqOsc2=NF.getINoteFreq(nt2);
  note=nt1;

  pb.setNote(note); 
}



Sint16 PicosynthVCO::tick()
{
  //  DPRINTF("PicosynthVCO::tick() this=0x%08.8X\n",this); 
  // return s1->tick()+s2->tick();
  Sint32 sa;
  Sint32 sb;
  Sint32 sc;
  Sint32 s;
  Sint32 lfo_tick=0;
  Sint32 lfo_tick_normdownshift=0;
  Sint32 pbtick;

  if (vcomix==0) vcomix=1;
  if (s1==NULL)
    { 
      DPRINTF("[s1 is NULL]"); 
      //  exit(1); 
    } 
  
  if (lfo_type==0)
    {
      if (lfo_depth>0  && 
	  lfo_speed>0
	  )

	{
	  // Lfo activated
	  lfo_tick=lfo1->tick();
	  //lfo_tick_normdownshift=((lfo_tick>>7)*lfo_depth>>7)+64;
	  lfo_tick_normdownshift=((lfo_tick>>7)*lfo_depth>>7)+0;
	  
	  //DPRINTF("lfo_tick:%d lfo_tick_normdownshift:%d\n",lfo_tick,lfo_tick_normdownshift);
	  
	}
      else
	{
	  // Fixed value 64 for the detuning of oscillator 1 
	  // 0 << 64 << 127
	  lfo_tick_normdownshift=64;
	}
      s1->setNoteDetune((note<<7)+lfo_tick_normdownshift);
    }
  
  if (lfo_type==1)
    {  
      pbtick=pb.tickNoteDetune();
      s1->setFreq(pbtick);
      s2->setFreq(pbtick);
    }

  
  sa=(s1->tick()*((128-vcomix)));
  sb=(s2->tick()*((vcomix)));

  sc=(sa+sb)>>7;

  s=sc;
  return s;
}
