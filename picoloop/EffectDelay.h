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

  void    process(int32_t * in, int nbsample);
  int32_t process_one_sample(int32_t in);

 protected:
  int32_t * buffer;
  int   index;       // Position in buffer for the next Write
  int   indexOffset; // Index for Read
  int   indexCurrent;
  int   offset;
  int   depth;
  //  int   depth;
  int   speed;
  int   sample_num;
  int   pole;
  int   old_pole;
  int   last_index_poleon;
  int   slowit;  
};

#endif
