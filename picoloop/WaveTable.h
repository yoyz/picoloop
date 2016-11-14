//#include <SDL/SDL_types.h>

#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              

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
