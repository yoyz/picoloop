#include "OneOscillator.h"

using namespace std;


OneOscillator::OneOscillator() : table(new Sint16[WAVETABLE_SIZE])
{
  index=0;
  DPRINTF("OneOscillator::OneOscillator() %d",index);
  table_size=WAVETABLE_SIZE;
  //table=NULL;
  //table=(Sint16*)malloc(sizeof(Sint16)*table_size);
  //table=new Sint16[table_size];
 
  //table = (Sint16*)malloc(sizeof(Sint16)*table_size);
}

OneOscillator::~OneOscillator()
{
  DPRINTF("OneOscillator::~OneOscillator() 0x%08.8X",table);
  if (table)
    delete[] table;
    //delete(table);
    
  //if (table!=NULL)
    //&& index!=0)
    //free(table);
    //delete(table);
  //table=NULL;
}

void OneOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16-1;
  //if (table==NULL)
  //{
  //  table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      DPRINTF("OneOscillator::init() 0x%08.8X",table);
      //table=new Sint16[table_size];
      //      if (table==NULL)
      //{
      //	  DPRINTF("ERROR\n");
      //	  exit(1);
      //	}
      //    }
      //  table=NULL;
      for (i=0;i<table_size;i++)
	{
	  //f=sin((2*3.14159*i*1)/table_size)*(1<<13);
	  s=1<<bitdepth-1;
	  table[i]=s;

	  //DPRINTF("fvalue = %f ",f);
	  //DPRINTF("OneOscillator::init() table[%d]=%d\n",i,table[i]);
	}
      //}
  
}

Sint16 OneOscillator::tick()
{ 
  //index=index+(this->getFreq()*table_size)/44100;
  index=index+offset_next_index;
  if (index>table_size)
    index=index-table_size;
    //index=0;

  //DPRINTF("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];
}


