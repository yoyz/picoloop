#include "Effect.h"

#define FX_SIZE 1024*256

//Effect::Effect() : buffer(new int16_t[FX_SIZE])
//Effect::Effect() : buffer(new int16_t[1024*256])
Effect::Effect() //: FXDelay()
{
  DPRINTF("Effect::Effect()\n");
}

Effect::~Effect()
{
}

void Effect::init()
{
  //DPRINTF("Effect::init() buffer=%x.8\n",buffer);
  DPRINTF("Effect::init()\n");
  //FX=&FXDelay;
  //FXDelay.init();
}

void Effect::reset()
{
  //FXDelay.init();
}

void Effect::setDepth(int val)
{
  //FXDelay.setDepth(val);
}

void Effect::setSpeed(int val)
{
  //FXDelay.setSpeed(val);

}

int32_t Effect::process_one_sample(int32_t in)
{  
  //return FXDelay.process(in);
  return 0;
}

void Effect::process(int32_t  * in,int nbsample)
{

}
