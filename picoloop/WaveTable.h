#include <SDL/SDL_types.h>
#include "Master.h"
#ifndef __WAVETABLE__
#define __WAVETABLE__

class WaveTable
{
 public:
  WaveTable();
  ~WaveTable();

  int      setSize(int bufferSize);
  int      allocMemory();
  int      freeMemory();


  Sint16 * getBuffer();
  char   * getName();
  Sint32   getSize();


    
 private:
  Sint32   size;
  Sint16 * buffer;
  char   * name;
    
};

#endif
