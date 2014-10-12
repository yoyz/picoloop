#include "DboplMachine.h"

#define SAM 512

//enum {
//  FMTYPE_2_OP_AM,
//  FMTYPE_2_OP_FM
//};


dboplMachine::dboplMachine() : filter()
{
  printf("dboplMachine::dboplMachine()\n");  
  buffer=0;
  cutoff=125;
  resonance=10;
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
}

void dboplMachine::reset()
{
 sample_num=0;
 freq=110.0;
 keyon=0;
}



int dboplMachine::getI(int what)
{
  if (what==NOTE_ON) return keyon;
}

void dboplMachine::setF(int what,float val)
{
    if (what==OSC1_FREQ)           freq=val;
}



void dboplMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;
  FreqMultiple freqM[15];
  Waveform w[4];
  w[0]=SIN;
  w[0]=HALF_SIN;
  w[0]=ABS_SIN;
  w[0]=QUART_SIN;

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
      HO->KeyOn(1,freq);
    }

    if (what==NOTE_ON && val==0) 
    { 
      keyon=0;
      HO->KeyOff(1);
    }

    if (what==OSC1_TYPE)           HO->SetWaveform(1,1,w[val]);
    if (what==OSC2_TYPE)           HO->SetWaveform(1,2,w[val]);
    HO->EnableWaveformControl();


    if (what==ADSR_ENV0_ATTACK)    HO->SetEnvelopeAttack( 1,1,val/16);
    if (what==ADSR_ENV0_DECAY)     HO->SetEnvelopeDecay(  1,1,val/16);
    if (what==ADSR_ENV0_SUSTAIN)   HO->SetEnvelopeSustain(1,1,val/16);
    if (what==ADSR_ENV0_RELEASE)   HO->SetEnvelopeRelease(1,1,val/16);

    if (what==ADSR_ENV1_ATTACK)    HO->SetEnvelopeAttack( 1,2,val/16);
    if (what==ADSR_ENV1_DECAY)     HO->SetEnvelopeDecay(  1,2,val/16);
    if (what==ADSR_ENV1_SUSTAIN)   HO->SetEnvelopeSustain(1,2,val/16);
    if (what==ADSR_ENV1_RELEASE)   HO->SetEnvelopeRelease(1,2,val/16);

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

  if (index>=SAM)
    index=0;


  if (sample_num==0 || 
      index==0 )
    HO->Generate(SAM,buffer);



  s_in=buffer[index];
  s_out=filter.process(s_in);

  index++;
  sample_num++;

  return s_out;
}

