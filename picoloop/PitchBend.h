#include <string>
#include "Master.h"
#ifndef __PITCHBEND__
#define __PITCHBEND__

class PitchBend
{
 public:

  PitchBend();
  ~PitchBend();

  void init();
  
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
  Sint16 depth_pos;
  //Sint16 depth_neg;
  Sint32 formula1; // NoteDetune value
  Sint32 formula2; // is the pitch bend apply to note value


  Sint32 sample_num;
  Sint32 div;
  Sint32 speed_calc;
};

#endif

