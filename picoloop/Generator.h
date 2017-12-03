#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              

#include <math.h>
#include "Master.h"

#ifndef __GENERATOR__
#define __GENERATOR__

class Generator 
{
public:
  Generator();
  ~Generator();

  void initFeedBack();
  Sint16 feedFeedBack(Sint16 input);
  void sine();
  void saw();
  void pulse();
  void triangle();
  void noise();

  void smoothSine();
  void smoothSaw();
  void smoothPulse();
  void smoothTriangle();
  void LFSRNoise();

  
  void one();
  Sint16 * getBuffer();

 private:
  Sint16 * table;
  void     init();
  Sint16   tick();
  //  int      phase;  
  int      table_size;
  int      index;
  int      d;
  Sint16 * feedback;

};
#endif
