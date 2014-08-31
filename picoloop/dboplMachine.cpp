#include "dboplMachine.h"

#define SAM 512


dboplMachine::dboplMachine()
{
  printf("dboplMachine::dboplMachine()\n");  
  buffer=0;
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

  HO->SetSampleRate(44100);

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


int dboplMachine::get(int what)
{
  if (what==NOTE_ON) return keyon;
}


void dboplMachine::set(int what,int val)
{
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

    if (what==ADSR_ENV0_ATTACK)    HO->SetEnvelopeAttack(1,1,val/16);
    if (what==ADSR_ENV0_DECAY)     HO->SetEnvelopeDecay(1,1,val/16);
    if (what==ADSR_ENV0_SUSTAIN)   HO->SetEnvelopeSustain(1,1,val/16);
    if (what==ADSR_ENV0_RELEASE)   HO->SetEnvelopeRelease(1,1,val/16);

    if (what==ADSR_ENV1_ATTACK)    HO->SetEnvelopeAttack(1,2,val/16);
    if (what==ADSR_ENV1_DECAY)     HO->SetEnvelopeDecay(1,2,val/16);
    if (what==ADSR_ENV1_SUSTAIN)   HO->SetEnvelopeSustain(1,2,val/16);
    if (what==ADSR_ENV1_RELEASE)   HO->SetEnvelopeRelease(1,2,val/16);


    if (what==OSC1_FREQ)           freq=val;
}

int dboplMachine::tick()
{
  Sint16 out;

  if (index>=SAM)
    index=0;

  //float f_out;
  if (sample_num==0 || 
      index==0 )
    HO->Generate(SAM,buffer);

  //  if (index>SAM)
  //HO.Generate(SAM,buffer);



  out=buffer[index];
  index++;
  sample_num++;

  return out;
}

