#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Effect.h"

#ifndef __EFFECTDISABLED____
#define __EFFECTDISABLED____

class EffectDisabled : public Effect
{
 public:
  EffectDisabled();
  ~EffectDisabled();

  void init();
  void reset();
  
  void setAlgo(int val);
  void setDepth(int val);
  void setSpeed(int val);

  int16_t process(int16_t in);

 protected:
  int16_t * buffer;
  int   index;       // Position in buffer for the next Write
  int   indexOffset; // Index for Read
  int   offset;
  int   depth;
  int   speed;
  int   sample_num;
  
};
#endif
