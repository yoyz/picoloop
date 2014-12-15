#include "EffectDelay.h"
#include "Master.h"
//#define FX_SIZE 1024*256
//#define FX_SIZE 1024*512
#define FX_SIZE DEFAULTFREQ*8
//#define FX_SIZE 1024*128
//#define FX_SIZE 1024*128

//EffectDelay::EffectDelay() : buffer(new int16_t[FX_SIZE])
//EffectDelay::EffectDelay() : buffer(new int16_t[1024*256])
EffectDelay::EffectDelay()
{
  printf("EffectDelay::EffectDelay()\n");
  int i;

  buffer=0;
  index=0;
  indexOffset=0;
  sample_num=0;
  buffer=NULL;
  //for (i=0;i<FX_SIZE;i++)
  //buffer[i]=0;

}

EffectDelay::~EffectDelay()
{
  //  if (buffer)
  //    free(buffer);
}

void EffectDelay::init()
{
  printf("EffectDelay::init() before malloc buffer=%x.8\n",buffer);
  //buffer=0;
  index=0;
  indexOffset=0;
  sample_num=0;


  //FX=&FXDelay;
  
  if (buffer==0)
  {
    printf("EffectDelay::init() after malloc buffer=%x.8\n",buffer);
    buffer=(int16_t*)malloc(sizeof(int16_t)*FX_SIZE);
  }
  
}

void EffectDelay::reset()
{
  int i=0;
  //buffer=0;
  index=0;
  indexOffset=0;
  sample_num=0;

  for (i=0;i<FX_SIZE;i++)
    buffer[i]=0;
}

void EffectDelay::setDepth(int val)
{
  //depth=127-val;
  depth=val;
  //index=0;
  //indexOffset=0;

  if (depth>127)
    depth=127;
  if (depth<0)
    depth=0;

}

void EffectDelay::setSpeed(int val)
{
  speed=val;
  //index=0;
  //indexOffset=0;

  if (speed>127)
    speed=127;
  if (speed<0)
    speed=0;



  offset=(FX_SIZE)/(128-speed);


  //offset=((FX_SIZE)/(128-speed))+offset;
  //if (offset>FX_SIZE)
  //offset=FX_SIZE-offset;

  indexOffset=index-offset;
  if (indexOffset<0)
    indexOffset=FX_SIZE-offset;
}

/*
int16_t EffectDelay::process(int16_t in)
{
  //int out=buffer[indexOffset]+buffer[index]/depth;
  //int 
  //int out;
  int32_t out;

  //buffer[index]=in;
  //out=(buffer[indexOffset]/2)+buffer[index];
  //if (buffer==0) 
    //this->init();
  buffer[index]=in;
  out=(buffer[indexOffset]/(depth+1))+buffer[index];
  buffer[index]=out;

  index++;
  indexOffset++;

  if (index>FX_SIZE)
    index=0;
  if (indexOffset>FX_SIZE)
    indexOffset=0;

  return out;
}
*/




int16_t EffectDelay::process(int16_t in)
{
  //int out=buffer[indexOffset]+buffer[index]/depth;
  //int 
  //int out;
  int32_t out;
  //out=(((buffer[indexOffset])*depth)/127)+in;
  out=(((buffer[indexOffset])*depth)>>7)+in;
  //buffer[index]=in;
  //out=(buffer[indexOffset]/2)+buffer[index];
  //if (buffer==0) 
    //this->init();
  //out=(buffer[indexOffset]/(depth+1))+in;
  //out=(buffer[indexOffset]/(depth+1))+in;
  
  buffer[index]=out;

  index++;
  indexOffset++;

  if (index>FX_SIZE)
    index=0;
  if (indexOffset>FX_SIZE)
    indexOffset=0;

  return out;
}
