using namespace std;
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Effect.h"

#ifndef __EFFECTDELAY____
#define __EFFECTDELAY____

class EffectDelay : public Effect
{
 public:
  EffectDelay();
  ~EffectDelay();

  void init();
  void reset();
  
  void setAlgo(int val);
  void setDepth(int val);
  void setSpeed(int val);

  int16_t process(int16_t in);

 protected:
  int16_t * buffer;
  int   index;
  int   indexOffset;
  int   offset;
  int   depth;
  //  int   depth;
  int   speed;
  int   sample_num;
  
};
#endif
