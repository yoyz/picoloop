#include "Effect.h"

#define FX_SIZE 1024*256

//Effect::Effect() : buffer(new int16_t[FX_SIZE])
//Effect::Effect() : buffer(new int16_t[1024*256])
Effect::Effect() //: FXDelay()
{
  DPRINTF("Effect::Effect()");
}

Effect::~Effect()
{
}

void Effect::init()
{
  //DPRINTF("Effect::init() buffer=%x.8\n",buffer);
  DPRINTF("Effect::init()");
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

// process_one_sample() and process() are pure virtual in Effect.h: every
// derived effect must implement them. The base versions used to return 0 /
// do nothing, so an effect that forgot to override them muted the track.
