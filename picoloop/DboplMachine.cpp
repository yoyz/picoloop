#include "DboplMachine.h"

#define SAM 512


dboplMachine::dboplMachine() : bq()
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
  freq=110;
  keyon=0;

  bq.setBiquad(0, 0.2, 0.5, 0.1);

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
  //HO->KeyOn(1,freq);
}

void dboplMachine::reset()
{
 sample_num=0;
 freq=110;
 keyon=0;
}

Biquad & dboplMachine::getBiquad()
{
  return bq;
}


int dboplMachine::get(int what)
{
  if (what==NOTE_ON) return keyon;
}


void dboplMachine::set(int what,int val)
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


    if (what==ADSR_ENV0_ATTACK)    HO->SetEnvelopeAttack(1,1,val/16);
    if (what==ADSR_ENV0_DECAY)     HO->SetEnvelopeDecay(1,1,val/16);
    if (what==ADSR_ENV0_SUSTAIN)   HO->SetEnvelopeSustain(1,1,val/16);
    if (what==ADSR_ENV0_RELEASE)   HO->SetEnvelopeRelease(1,1,val/16);

    if (what==ADSR_ENV1_ATTACK)    HO->SetEnvelopeAttack(1,2,val/16);
    if (what==ADSR_ENV1_DECAY)     HO->SetEnvelopeDecay(1,2,val/16);
    if (what==ADSR_ENV1_SUSTAIN)   HO->SetEnvelopeSustain(1,2,val/16);
    if (what==ADSR_ENV1_RELEASE)   HO->SetEnvelopeRelease(1,2,val/16);

    if (what==OSC12_MIX)           HO->SetFrequencyMultiple(1,1,freqM[val/11]);
    if (what==OSC1_PHASE)          HO->SetFrequencyMultiple(1,2,freqM[val/11]);

  if (what==FILTER1_CUTOFF)      
    { 
      f_val_cutoff=val;
      f_val_resonance=resonance;
      cutoff=val;
      this->getBiquad().setBiquad(0, (f_val_cutoff/256)-0.005, (f_val_resonance/8)+0.005, 0.0);
      //this->getBiquad().setFc((f_val/256)+0.005);  
      this->getBiquad().calcBiquad(); 

  }
  if (what==FILTER1_RES)         
    { 
      f_val_cutoff=cutoff;
      f_val_resonance=val;
      resonance=val;
      //this->getBiquad().setQ((f_val/8)+0.005);   
      this->getBiquad().setBiquad(0, (f_val_cutoff/256)-0.005, (f_val_resonance/8)+0.005, 0.0);
      this->getBiquad().calcBiquad(); 
    }



    if (what==OSC1_FREQ)           freq=val;
}

int dboplMachine::tick()
{
  Sint16 s_in;
  Sint16 s_out;

  if (index>=SAM)
    index=0;

  //float f_out;
  if (sample_num==0 || 
      index==0 )
    HO->Generate(SAM,buffer);

  //  if (index>SAM)
  //HO.Generate(SAM,buffer);


  //s_in=buffer[index];
  //s_out=s_in;

  s_in=buffer[index];
  s_out=bq.process(s_in);

  index++;
  sample_num++;

  return s_out;
}

