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
  ~Effect();

  virtual void init();
  virtual void reset();
  
  //void setAlgo(int val);
  virtual void setDepth(int val);
  virtual void setSpeed(int val);

  virtual int32_t process_one_sample(int32_t in);

  virtual void process(int32_t  * in,int nbsample);

 protected:
  //EffectDelay     FXDelay;

  
};
#endif
