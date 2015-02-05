#include "Generator.h"

Generator::Generator() : table(new Sint16[WAVETABLE_SIZE])
{
  table_size=WAVETABLE_SIZE;
}

Generator::~Generator()
{
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

  s=(1<<(bitdepth-2));
  dec=(1<<(bitdepth-2))/(table_size/2);

  for (i=0;i<table_size;i++)
    {
      table[i]=s;
      s=s-dec;
    }
}


void Generator::pulse()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/2);

  printf("Generator::pulse() 0x%08.8X\n",table);

  for (i=0;i<table_size/2;i++)
    {
      table[i]=((1<<(bitdepth-2))/2);
    }
  for (i=table_size/2;i<table_size;i++)
    {
      table[i]=((1<<(bitdepth-2))*-1)/2;
    }
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
