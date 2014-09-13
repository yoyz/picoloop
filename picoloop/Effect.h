using namespace std;
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

  virtual int16_t process(int16_t in);

 protected:
  //EffectDelay     FXDelay;

  
};
#endif
