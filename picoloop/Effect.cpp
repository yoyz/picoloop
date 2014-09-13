#include "Effect.h"

#define FX_SIZE 1024*256

//Effect::Effect() : buffer(new int16_t[FX_SIZE])
//Effect::Effect() : buffer(new int16_t[1024*256])
Effect::Effect() //: FXDelay()
{
  printf("Effect::Effect()\n");
}

Effect::~Effect()
{
}

void Effect::init()
{
  //printf("Effect::init() buffer=%x.8\n",buffer);
  printf("Effect::init()\n");
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

int16_t Effect::process(int16_t in)
{
  
  //return FXDelay.process(in);
}


