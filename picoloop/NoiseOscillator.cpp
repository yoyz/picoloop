#include "NoiseOscillator.h"

using namespace std;


NoiseOscillator::NoiseOscillator() : table(new Sint16[WAVETABLE_SIZE])
{
  printf("NoiseOscillator::NoiseOscillator()");
  table_size=WAVETABLE_SIZE;
  //table=NULL;
  index=0;
}

NoiseOscillator::~NoiseOscillator()
{
  printf("NoiseOscillator::~NoiseOscillator()\n");
  //  if (table!=NULL)
  //free(table);
  //table=NULL;
}

void NoiseOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  srand(1<<(bitdepth-2));
  //  if (table==NULL)
  //    {
  //      table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      for (i=0;i<table_size;i++)
	{
	  if (rand()%2==0)
	    table[i]=rand()%8192;
	  else
	    table[i]=(rand()%8192)*-1;
	  //printf("NoiseOscillator::init() table[%d]=%d\n",i,table[i]);
	}
      //}  
}


Sint16 NoiseOscillator::tick()
{
  //index=index+(this->getFreq()*table_size)/44100;
  index=index+offset_next_index;
  if (index>=table_size)
    index=index-table_size;
  //index=0;

  //  d++; if (d>1000) d=0;
  //  if (d==0)
  //    printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];

}
