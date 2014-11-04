using namespace std;

#include <string>
#include "Master.h"
#ifndef __NOTEFREQ__
#define __NOTEFREQ__


class NoteFreq
{

 public:
  NoteFreq();
  ~NoteFreq();

  float getFNoteFreq(int note);
  int   getINoteFreq(int note);
  int   getWTJump(int   note);
  int   getWTJumpDetune(int note,int detune);
};

#endif
