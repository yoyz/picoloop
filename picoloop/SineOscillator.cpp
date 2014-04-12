#include "SineOscillator.h"

using namespace std;


SineOscillator::SineOscillator()
{
  printf("SineOscillator::SineOscillator()");
  table_size=1024;
  table=NULL;
  index=0;
}

SineOscillator::~SineOscillator()
{
  printf("SineOscillator::~SineOscillator()\n");
  if (table!=NULL)
    free(table);
}

void SineOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  if (table==NULL)
    {
      table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      //  table=NULL;
      for (i=0;i<table_size;i++)
	{
	  //f=sin((2*3.14159*i*1)/table_size)*(1<<13);
	  s=sin((2*3.14159*i*1)/table_size)*(1<<14);
	  table[i]=s;

	  //printf("fvalue = %f ",f);
	  printf("SineOscillator::init() table[%d]=%d\n",i,table[i]);
	}
    }
  
}

Sint16 SineOscillator::tick()
{ 
  index=index+(this->getFreq()*1024)/44100;
  if (index>table_size)
    index=index-table_size;
  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];
}


