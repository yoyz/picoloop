#include "PulseOscillator.h"

using namespace std;


PulseOscillator::PulseOscillator()
{
  printf("PulseOscillator::PulseOscillator()");
  table_size=WAVETABLE_SIZE;
  table=NULL;
  index=0;

  frequency=440;
  amplitude=127;
  sample_num=0;
}

PulseOscillator::~PulseOscillator()
{
  printf("PulseOscillator::~PulseOscillator()\n");
  if (table!=NULL)
    free(table);

}


void PulseOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-1))/(table_size/2);
  if (table==NULL)
    {
      table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      for (i=0;i<table_size/2;i++)
	{
	  table[i]=(1<<(bitdepth-2));
	  //	  table[i]=s;
	  //printf("PulseOscillator::init table[%d]=%d\n",i,table[i]);
	}
      for (i=table_size/2;i<table_size;i++)
	{
	  table[i]=(1<<(bitdepth-2))*-1;
	  //	  table[i]=s;
	  //printf("PulseOscillator::init table[%d]=%d\n",i,table[i]);
	}

    }  
}

Sint16 PulseOscillator::tick()
{
  index=index+(this->getFreq()*table_size)/44100;
  if (index>table_size)
    index=index-table_size;
  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];

}
