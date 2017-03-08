using namespace std;

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
//#include <SDL/SDL.h>

#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              

//#include "PatternElement.h"
#include "Master.h"
#include "WaveTableManager.h"
#include "NoteFreq.h"

#ifndef __OSCILLATOR__
#define __OSCILLATOR__

class Oscillator
{
 public:
  Oscillator();
  ~Oscillator();
  void   setAmplitude(int amp);
  void   reset();
  void   setPhase(int phase);
  void   setFreq(int freq);
  int    getFreq();

  void   setNoteDetune(int note,int detune);
  void   setNoteDetune(int notedetune);
  
  void   setWaveForm(int waveform);

  
  Sint16 tick();



  Sint16 last_tick;
  //int    length;
  int    amplitude;
  int    frequency;
  int    bit;
  int    sample_num;
  int    sample_num_index;
  int    table_fill;
  int    table_size;
  int    freq;
  int    note;
  int    detune;
  int    notedetune;
  
  Sint32 offset_next_index; // index=index+(this->getFreq()*table_size)/44100;
                            // index=index+(offset_next)
                            // offset_next=(this->getFreq()*table_size)/44100;
  Uint16 u;
  Uint16 last_u;
  Sint16 s;
  Sint16 ph;

  Sint32 index;


  Sint16 * table;
  //NoteFreq NF;
  
};

#endif

