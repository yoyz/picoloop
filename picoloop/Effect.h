#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Master.h"
#ifndef __EFFECT____
#define __EFFECT____

class Effect 
{
 public:
  Effect();
  // Virtual: MonoMixer holds an `Effect *` and is expected to delete it.
  // Without this, `delete FX;` would not run the derived destructor.
  virtual ~Effect();

  virtual void init();
  virtual void reset();
  
  //void setAlgo(int val);
  virtual void setDepth(int val);
  virtual void setSpeed(int val);

  // Pure virtual: a derived effect must say what a sample becomes. The old
  // base returned 0, i.e. an effect that was not overridden silently muted the
  // whole track instead of failing to build.
  virtual int32_t process_one_sample(int32_t in) = 0;

  virtual void process(int32_t  * in,int nbsample) = 0;

 protected:
  //EffectDelay     FXDelay;

  
};
#endif
