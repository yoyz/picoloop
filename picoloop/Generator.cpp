#include "Generator.h"

#define FBSIZE 256

Generator::Generator() : table(new Sint16[WAVETABLE_SIZE]),
			 feedback(new Sint16[FBSIZE])
{
  table_size=WAVETABLE_SIZE;
  table[0]=0;
}

Generator::~Generator()
{
  delete(table);
  delete(feedback);
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
  DPRINTF("Generator::sine() 0x%08.8X\n",table);
  for (i=0;i<table_size;i++)
    {
      s=sin((2*3.14159*i*1)/table_size)*(1<<bitdepth-2);
      table[i]=s;
      //DPRINTF("table[%d]=%d\n",i,s);
    }
}


void Generator::smoothSine()
{
  int i;
  float f;
  Sint16 s1;
  Sint16 s2;
  Sint16 bitdepth=16-1;
  DPRINTF("Generator::sine() 0x%08.8X\n",table);
  for (i=0;i<table_size;i++)
    {
      s1=sin((2*3.14159*i*1)/table_size)*(1<<bitdepth-2);
      s2=sin((2*3.14159*i*2)/table_size)*(1<<bitdepth-4);

      
      table[i]=s1-s2;
      //DPRINTF("table[%d]=%d\n",i,s);
    }
}


void Generator::saw()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec;

  DPRINTF("Generator::saw() 0x%08.8X\n",table);
  initFeedBack();
  
  s=(1<<(bitdepth-2));
  dec=(1<<(bitdepth-2))/(table_size/2);

  for (i=0;i<table_size;i++)
    {
      table[i]=s;
      s=s-dec;
    }
}

void Generator::smoothSaw()
{
  int i;
  float f;
  Sint16 s;
  Sint16 s1;
  Sint16 s2;
  Sint16 s3;
  Sint16 s4;
  Sint16 s5;
  Sint16 s6;
  Sint16 s7;
  Sint16 s8;
  Sint16 s9;
  Sint16 s10;

  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/2);
  
  DPRINTF("Generator::pulse() 0x%08.8X\n",table);

  //initFeedBack();
  for (i=0;i<table_size;i++)
    {
      s1 =sin((2*3.14159*i*1)/table_size) *(1<<bitdepth-2);
      s2 =sin((2*3.14159*i*2)/table_size) *(1<<bitdepth-3);
      s3 =sin((2*3.14159*i*3)/table_size) *(1<<bitdepth-4);
      s4 =sin((2*3.14159*i*4)/table_size) *(1<<bitdepth-5);
      s5 =sin((2*3.14159*i*5)/table_size) *(1<<bitdepth-5);
      s6 =sin((2*3.14159*i*6)/table_size) *(1<<bitdepth-6);
      s7 =sin((2*3.14159*i*7)/table_size) *(1<<bitdepth-7);
      s8 =sin((2*3.14159*i*8)/table_size) *(1<<bitdepth-8);
      s9 =sin((2*3.14159*i*9)/table_size) *(1<<bitdepth-9);
      s10=sin((2*3.14159*i*10)/table_size)*(1<<bitdepth-10);

      table[i]=(s1-s2+s3-s4+s5-s6+s7-s8+s9-s10)/2;
      //table[i]=feedFeedBack(((1<<(bitdepth-2))/2));
      //DPRINTF("table[%d]=%d\n",i,table[i]);
    }
}



void Generator::pulse()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/2);
  
  DPRINTF("Generator::pulse() 0x%08.8X\n",table);

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
}



 
void Generator::smoothPulse()
{
  int i;
  float f;
  Sint16 s;
  Sint16 s1;
  Sint16 s2;
  Sint16 s3;
  Sint16 s4;
  Sint16 s5;
  Sint16 s6;
  Sint16 s7;
  Sint16 s8;
  Sint16 s9;
  Sint16 s10;

  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/2);
  
  DPRINTF("Generator::pulse() 0x%08.8X\n",table);

  //initFeedBack();
  for (i=0;i<table_size;i++)
    {
      s1 =sin((2*3.14159*i*1 )/table_size) *(1<<bitdepth-2);
      s2 =sin((2*3.14159*i*2 )/table_size) *(1<<bitdepth-2);
      s3 =sin((2*3.14159*i*3 )/table_size) *(1<<bitdepth-2);
      s4 =sin((2*3.14159*i*4 )/table_size) *(1<<bitdepth-2);
      s5 =sin((2*3.14159*i*3 )/table_size) *(1<<bitdepth-2);
      s6 =sin((2*3.14159*i*4 )/table_size) *(1<<bitdepth-2);
      s7 =sin((2*3.14159*i*7 )/table_size) *(1<<bitdepth-2);
      s8 =sin((2*3.14159*i*8 )/table_size) *(1<<bitdepth-2);
      s9 =sin((2*3.14159*i*9 )/table_size) *(1<<bitdepth-2);
      s10=sin((2*3.14159*i*10)/table_size) *(1<<bitdepth-2);

      //table[i]=s1-s2/4-s3/2;
      if (s1>=0)
	table[i]=(s1+(abs(s2/2)+abs(s3/3)+abs(s4/4)+abs(s5/5)+abs(s6/6)))/2; //+abs(s7/7)); //+abs(s8/8)+abs(s9/9)+abs(s10/10));
      else
	table[i]=(s1-(abs(s2/2)+abs(s3/3)+abs(s4/4)+abs(s5/5)+abs(s6/6)))/2; //+abs(s7/7)); //+abs(s8/8)+abs(s9/9)+abs(s10/10));
      //-4*s4-5*s5-6*s6-7*s7-8*s8-9*s9;
      //table[i]=feedFeedBack(((1<<(bitdepth-2))/2));
      //DPRINTF("table[%d]=%d\n",i,table[i]);
    }
}


void Generator::triangle()
{
  int i;
  float f;
  Sint16 s=0;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/4);

  DPRINTF("Generator::triangle() 0x%08.8X\n",table);

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

void Generator::smoothTriangle()
{
  int i;
  float f;
  Sint16 s;
  Sint16 s1;
  Sint16 s2;
  Sint16 s3;
  Sint16 s4;
  Sint16 bitdepth=16;
  Sint16 dec=(1<<(bitdepth-2))/(table_size/2);
  
  DPRINTF("Generator::pulse() 0x%08.8X\n",table);

  //initFeedBack();
  for (i=0;i<table_size;i++)
    {
      s1=sin((2*3.14159*i*1)/table_size)*(1<<bitdepth-2);
      s2=sin((2*3.14159*i*2)/table_size)*(1<<bitdepth-3);
      s3=sin((2*3.14159*i*3)/table_size)*(1<<bitdepth-4);
      s4=sin((2*3.14159*i*4)/table_size)*(1<<bitdepth-5);
      table[i]=s1-s2-s3-s4;
      //table[i]=feedFeedBack(((1<<(bitdepth-2))/2));
      //DPRINTF("table[%d]=%d\n",i,table[i]);
    }
}


void Generator::noise()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;

  DPRINTF("Generator::noise() 0x%08.8X\n",table);

  srand(1<<(bitdepth-2));
  for (i=0;i<table_size;i++)
    {
      if (rand()%2==0)
	table[i]=rand()%8192;
      else
	table[i]=(rand()%8192)*-1;
    }
}




void Generator::LFSRNoise()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;

  Sint16 mask[16];
  
  //mask[0]= 0b0011010101101010;

  mask[0]= 0x346A;
  for (i=1;i<16;i++)
    mask[i]= ( mask[i-1] & 0x80 ) ? ( mask[i-1] << 1 )   : ( ( mask[i-1] << 1 ) + 1 );
  // mask[0]= 0b 0011 0101 0110 1010;
  // mask[1]= 0b0110101011010100;
  // mask[2]= 0b1101010110101000;
  // mask[3]= 0b1010101101010001;
  // mask[4]= 0b0101011010100011;
  // mask[5]= 0b1010110101000110;
  // mask[6]= 0b0101101010001101;
  // mask[7]= 0b1011010100011010;
  // mask[8]= 0b0110101000110101;
  // mask[9]= 0b1101010001101010;
  // mask[10]=0b1010100011010101;
  // mask[11]=0b0101000110101011;
  // mask[12]=0b1010001101010110;
  // mask[13]=0b0100011010101101;
  // mask[14]=0b1000110101011010;
  // mask[15]=0b0001101010110101;
  
  DPRINTF("Generator::noise() 0x%08.8X\n",table);

  srand(1<<(bitdepth-2));
  for (i=0;i<table_size;i++)
    {
      if (rand()%2==0)
	table[i]=mask[i%16];
	//table[i]=rand()%8192;
	else
	  table[i]=-mask[i%16];
	//table[i]=(rand()%8192)*-1;      
      //table[i]=table[i] & mask[i%16];
      //table[i]
    }  
}

void Generator::one()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;

  DPRINTF("Generator::one() 0x%08.8X\n",table);

  for (i=0;i<table_size;i++)
    {
      table[i]=1<<bitdepth-1;
    }  
}



Sint16 * Generator::getBuffer()
{
  return table;
}
