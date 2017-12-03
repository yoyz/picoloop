#include <string>
#include "Master.h"
#ifndef __NOTEFREQ__
#define __NOTEFREQ__


class NoteFreq
{
  private:

  NoteFreq();
  ~NoteFreq();

  Sint32  ** wtjTable; // table of notefreq with the detune 
                     // value of this table are multiplied by WAVETABLE_SIZE
                     // it allow to lookup the next jump of the oscillator

 public:
  static NoteFreq& getInstance();

  void  init();
  float getFNoteFreq(int note);
  int   getINoteFreq(int note);
  int   getWTJump(int   note);
  int   getWTJumpDetune(int note,int detune);
  int   getWTJumpDetune(int notedetune);

  const char * getNoteCharStar(int note);

};

#endif
