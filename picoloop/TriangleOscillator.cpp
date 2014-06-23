#include "TriangleOscillator.h"

using namespace std;


TriangleOscillator::TriangleOscillator()
{
  printf("TriangleOscillator::TriangleOscillator()");
  table_size=WAVETABLE_SIZE;
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

TriangleOscillator::~TriangleOscillator()
{
  printf("TriangleOscillator::~TriangleOscillator()\n");
  if (table!=NULL)
    free(table);
}

void TriangleOscillator::init()
{
  int i;
  float f;
  Sint16 s=0;
  Sint16 bitdepth=16;
  //Sint16 dec=(1<<(bitdepth))/(table_size/4);
  Sint16 dec=(1<<(bitdepth-2))/(table_size/4);
  if (table==NULL)
    {

      table=(Sint16*)malloc(sizeof(Sint16)*table_size);      
      for (i=0;i<(table_size*1)/4;i++)
	{
	  table[i]=s;
	  s=s+dec;
	  //printf("TriangleOscillator::init() table[%d]=%d\n",i,table[i]);
	}
      for (i=(table_size*1)/4;i<(table_size*3)/4;i++)
	{
	  table[i]=s;
	  s=s-dec;
	}
      for (i=(table_size*3)/4;i<table_size;i++)
	{
	  table[i]=s;
	  s=s+dec;
	}

    }  
}


Sint16 TriangleOscillator::tick()
{

  index=index+(this->getFreq()*table_size)/44100;
  if (index>table_size)
    index=index-table_size;
  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];

}
