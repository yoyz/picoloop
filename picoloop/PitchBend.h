using namespace std;

#include <string>
#include "Master.h"
#ifndef __PITCHBEND__
#define __PITCHBEND__

class PitchBend
{
 public:

  PitchBend();
  ~PitchBend();

  void setDepth(int d);
  void setSpeed(int s);
  void setNote(int  n);
  void calc();

  void reset();
  int  tickNoteDetune();

 private:

  Sint16 note;
  Sint16 speed;
  Sint16 depth;
  Sint32 formula1;
  Sint32 formula2;


  Sint32 sample_num;
  
};

#endif

