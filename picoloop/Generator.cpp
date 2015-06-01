#include "Generator.h"

#define FBSIZE 256

Generator::Generator() : table(new Sint16[WAVETABLE_SIZE]),
			 feedback(new Sint16[FBSIZE])
{
  table_size=WAVETABLE_SIZE;
}

Generator::~Generator()
{
}

Sint16 Generator::feedFeedBack(Sint16 input)
{
  Sint32 output32=0;
  Sint16 output=0; 
  int    i;

  
  feedback[FBSIZE-1]=input;
  for (i=0;i<FBSIZE;i++)
    output32=output32+feedback[i];

  output=output32/FBSIZE;
  
  for (i=0;i<FBSIZE;i++)
    feedback[i]=feedback[i+1];
  
  return output;
}

void Generator::initFeedBack()
{
  int i;
  for (i=0;i<FBSIZE;i++)
    feedback[i]=0;
}

void Generator::sine()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16-1;
  printf("Generator::sine() 0x%08.8X\n",table);
  for (i=0;i<table_size;i++)
    {
      s=sin((2*3.14159*i*1)/table_size)*(1<<bitdepth-2);
      table[i]=s;
      //printf("table[%d]=%d\n",i,s);
    }
}

void Generator::saw()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec;

  printf("Generator::saw() 0x%08.8X\n",table);
  initFeedBack();
  
  s=(1<<(bitdepth-2));
  dec=(1<<(bitdepth-2))/(table_size/2);

  for (i=0;i<table_size;i++)
    {
      //table[i]=s;
      //table[i]=feedFeedBack(s);
      table[i]=s;
      s=s-dec;
    }

  initFeedBack();
  for (i=0;i<table_size-FBSIZE;i++)
    table[i]=feedFeedBack(table[i]);
  for (i=table_size-FBSIZE;i<table_size;i++)
    table[i]=feedFeedBack(0);

}


void Generator::pulse()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/2);
  
  printf("Generator::pulse() 0x%08.8X\n",table);

  //initFeedBack();
  for (i=0;i<table_size/2;i++)
    {
      table[i]=((1<<(bitdepth-2))/2);
      //table[i]=feedFeedBack(((1<<(bitdepth-2))/2));
    }
  for (i=table_size/2;i<table_size;i++)
    {
      table[i]=((1<<(bitdepth-2))*-1)/2;
      //table[i]=feedFeedBack(((1<<(bitdepth-2))*-1)/2);
    }

  initFeedBack();
  for (i=0;i<table_size-FBSIZE;i++)
    table[i]=feedFeedBack(table[i]);
  for (i=table_size-FBSIZE;i<table_size;i++)
    table[i]=feedFeedBack(0);
}


void Generator::triangle()
{
  int i;
  float f;
  Sint16 s=0;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/4);
      
  printf("Generator::triangle() 0x%08.8X\n",table);
  
  //table=(Sint16*)malloc(sizeof(Sint16)*table_size);      
  for (i=0;i<(table_size*1)/4;i++)
    {
      table[i]=s;
      s=s+dec;
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


void Generator::noise()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;

  printf("Generator::noise() 0x%08.8X\n",table);

  srand(1<<(bitdepth-2));
  for (i=0;i<table_size;i++)
    {
      if (rand()%2==0)
	table[i]=rand()%8192;
      else
	table[i]=(rand()%8192)*-1;      
    }  
}

void Generator::one()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;

  printf("Generator::one() 0x%08.8X\n",table);

  for (i=0;i<table_size;i++)
    {
      table[i]=1<<bitdepth-1;
    }  
}



Sint16 * Generator::getBuffer()
{
  return table;
}
