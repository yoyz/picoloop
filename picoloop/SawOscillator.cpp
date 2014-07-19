#include "SawOscillator.h"

using namespace std;


SawOscillator::SawOscillator() : table(new Sint16[WAVETABLE_SIZE])
{
  printf("SawOscillator::SawOscillator()");
  table_size=WAVETABLE_SIZE;
  //table=NULL;
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
  //  if (table!=NULL)
  //free(table);
  //table=NULL;
}

void SawOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-1))/(table_size/4);
  //if (table==NULL)
  //{
      table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      s=(1<<(bitdepth-1))-((table_size/2));
      for (i=0;i<table_size;i++)
	{
	  table[i]=s;
	  s=s-dec;
	  //printf("SawOscillator::init() table[%d]=%d\n",i,table[i]);
	}
      //}  
}


Sint16 SawOscillator::tick()
{

  //index=index+(this->getFreq()*table_size)/44100;
  index=index+offset_next_index;
  if (index>table_size)
    index=index-table_size;
  //index=0;
  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];

}
