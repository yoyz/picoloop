#include "DboplMachine.h"

#define SAM 512

//enum {
//  FMTYPE_2_OP_AM,
//  FMTYPE_2_OP_FM
//};


dboplMachine::dboplMachine() : filter(), 	     sineLfoOsc1()
{
  printf("dboplMachine::dboplMachine()\n");  
  buffer=0;
  cutoff=125;
  resonance=10;
  index=0;

  lfo_depth=0;
  lfo_depth_shift=20;
  lfo_speed=0;

}


dboplMachine::~dboplMachine()
{
  printf("dboplMachine::~dboplMachine()\n");  
}


void dboplMachine::init()
{
  printf("dboplMachine::init()\n");  
  int i;

  //HO(44100);
  if (buffer==0)
    {
      HO=new Hiopl(44100);
      buffer = (Sint16*)malloc(sizeof(Sint16)*SAM*8);
    }

  printf("buffer:0x%08.8X\n",buffer);
  for (i=0;i<SAM;i++)
    buffer[i]=0;

  sample_num=0;
  freq=110.0;
  keyon=0;


  filter.init();

  //HO->SetSampleRate(22050);
  HO->SetSampleRate(44100);
  //HO->SetSampleRate(88200);

  HO->SetWaveform(1,1,QUART_SIN);
  HO->SetWaveform(1,2,SIN);

  HO->SetFrequencyMultiple(1,1,x1);

  HO->SetFrequencyMultiple(1,2,x1);

  HO->SetEnvelopeAttack(1,1,2);
  HO->SetEnvelopeDecay(1,1,7);
  HO->SetEnvelopeSustain(1,1,4);
  HO->SetEnvelopeRelease(1,1,15);

  HO->SetEnvelopeAttack(1,2,5);
  HO->SetEnvelopeDecay(1,2,2);
  HO->SetEnvelopeSustain(1,2,12);
  HO->SetEnvelopeRelease(1,2,15);
  //HO->EnableAdditiveSynthesis(1,true);
  //HO->SetModulatorFeedback(1,3);
  //HO->EnableKsr(1,1,true);
  //HO->EnableKsr(1,2,true);
  //HO->SetKsl(1,1,3);
  //HO->SetKsl(1,2,3);
  HO->SetAttenuation(1,1,20);
  HO->SetAttenuation(1,2,8);

  lfo_speed=0.0;

  //sineLfoOsc1.init();
  sineLfoOsc1.setWaveForm(2);
  sineLfoOsc1.setFreq(lfo_speed);
  sineLfoOsc1.setAmplitude(32);
}

void dboplMachine::reset()
{
 sample_num=0;
 freq=110.0;
 keyon=0;
 sineLfoOsc1.reset();
}



int dboplMachine::getI(int what)
{
  if (what==NOTE_ON) return keyon;
}

void dboplMachine::setF(int what,float val)
{
  if (what==OSC1_FREQ)           { freq=val; }
  if (what==LFO1_FREQ)           { lfo_speed=val/4.0; sineLfoOsc1.setFreq(lfo_speed); }
}



void dboplMachine::setI(int what,int val)
{
  float        f_val_cutoff;
  float        f_val_resonance;
  FreqMultiple freqM[15];
  Waveform     w[4];
  w[0]=SIN;
  w[1]=HALF_SIN;
  w[2]=ABS_SIN;
  w[3]=QUART_SIN;

  freqM[0]=xHALF;
  freqM[1]=x1;
  freqM[2]=x2;
  freqM[3]=x3;
  freqM[4]=x4;
  freqM[5]=x5;
  freqM[6]=x6;
  freqM[7]=x7;
  freqM[8]=x8;
  freqM[9]=x9;
  freqM[10]=x10;
  freqM[11]=x12;
  freqM[12]=x15;

  //HO->EnableAdditiveSynthesis(1,true);

    if (what==NOTE_ON && val==1) 
    { 
      keyon=1;
      sineLfoOsc1.reset();
      HO->KeyOn(1,freq);
    }

    if (what==NOTE_ON && val==0) 
    { 
      keyon=0;
      HO->KeyOff(1);
    }

    if (what==OSC1_TYPE)           
      { 
	HO->SetWaveform(1,1,w[val]);
	HO->EnableWaveformControl();
      }
    if (what==OSC2_TYPE)           
      {
	HO->SetWaveform(1,2,w[val]);
	HO->EnableWaveformControl();
      }


    if (what==ADSR_ENV0_ATTACK)    HO->SetEnvelopeAttack( 1,1,val/16);
    if (what==ADSR_ENV0_DECAY)     HO->SetEnvelopeDecay(  1,1,val/16);
    if (what==ADSR_ENV0_SUSTAIN)   HO->SetEnvelopeSustain(1,1,val/16);
    if (what==ADSR_ENV0_RELEASE)   HO->SetEnvelopeRelease(1,1,val/16);

    if (what==ADSR_ENV1_ATTACK)    HO->SetEnvelopeAttack( 1,2,val/16);
    if (what==ADSR_ENV1_DECAY)     HO->SetEnvelopeDecay(  1,2,val/16);
    if (what==ADSR_ENV1_SUSTAIN)   HO->SetEnvelopeSustain(1,2,val/16);
    if (what==ADSR_ENV1_RELEASE)   HO->SetEnvelopeRelease(1,2,val/16);


    if (what==LFO1_DEPTH)           {

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
      
      sineLfoOsc1.setFreq(lfo_speed);
      
    }


    if (what==OSC12_MIX)           HO->SetFrequencyMultiple(1,1,freqM[val/11]);
    if (what==OSC1_PHASE)          HO->SetFrequencyMultiple(1,2,freqM[val/11]);

    if (what==OSC1_AMP)            HO->SetAttenuation(1,1,32-(val/4));
    if (what==OSC2_AMP)            HO->SetAttenuation(1,2,32-(val/4));

    if (what==FM_TYPE)             
      {
	// AM
	if (val==FMTYPE_2_OP_AM)
	  HO->EnableAdditiveSynthesis(1,true);

	// FM
	if (val==FMTYPE_2_OP_FM)
	  HO->EnableAdditiveSynthesis(1,false);
      }



    //if (what==OSC1_FREQ)           freq=val;

    if (what==FILTER1_CUTOFF)      
      { 
	cutoff=val;
	filter.setCutoff(cutoff);
      }
    
    if (what==FILTER1_RES)         
      { 
	resonance=val;
	filter.setResonance(resonance);
      }
    
    if (what==FILTER1_TYPE)     filter.setFilterType(val);
    if (what==FILTER1_ALGO)     filter.setFilterAlgo(val);
    
    
  
}

int dboplMachine::tick()
{
  Sint16 s_in;
  Sint16 s_out;
  int    modulated_freq;

  if (index>=SAM | 
      index<0)
    index=0;


  if (sample_num==0 || 
      index==0 )
    {
      //modulated_freq=(sineLfoOsc1.tick()>>lfo_depth_shift);
      modulated_freq=((sineLfoOsc1.tick()>>5)/(128-lfo_depth));
      
      if (keyon)
	{
	  //HO->KeyOn(1,freq+modulated_freq);
	  HO->SetFrequency(1,freq+modulated_freq,true);
	  //HO->KeyOn(1,freq);
	}
      else
	{
	  HO->SetFrequency(1,freq+modulated_freq,false);
	  //HO->SetFrequency(1,freq,false);
	}
      HO->Generate(SAM,buffer);
    }



  s_in=buffer[index];
  s_out=filter.process(s_in);

  index++;
  sample_num++;

  return s_out;
}

