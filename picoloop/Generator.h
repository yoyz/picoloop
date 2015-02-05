using namespace std;

#include <SDL/SDL_types.h>
#include <math.h>
#include "Master.h"

#ifndef __GENERATOR__
#define __GENERATOR__

class Generator 
{
public:
  Generator();
  ~Generator();
  void sine();
  void saw();
  void pulse();
  void triangle();
  void noise();
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

};
#endif
