#include "SawOscillator.h"

using namespace std;


SawOscillator::SawOscillator()
{
  printf("SawOscillator::SawOscillator()");
  table_size=1024;
  table=NULL;
  index=0;


  frequency=440;
  amplitude=127;
  sample_num=0;
  //  phase=1;
  s=0;
  u=0;
  last_u=0;
}

SawOscillator::~SawOscillator()
{
  printf("SawOscillator::~SawOscillator()\n");
  if (table!=NULL)
    free(table);
}

void SawOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-1))/(table_size/2);
  if (table==NULL)
    {
      table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      s=(1<<(bitdepth-1))-((table_size/2));
      for (i=0;i<table_size;i++)
	{
	  table[i]=s;
	  s=s-dec;
	  printf("SawOscillator::init() table[%d]=%d\n",i,table[i]);
	}
    }  
}


Sint16 SawOscillator::tick()
{

  index=index+(this->getFreq()*table_size)/44100;
  if (index>table_size)
    index=index-table_size;
  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];

}
