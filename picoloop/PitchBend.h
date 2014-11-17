using namespace std;

#include <string>
#include "Master.h"
#ifndef __PITCHBEND__
#define __PITCHBEND__

class PitchBend
{
 private:
  PitchBend();
  ~PitchBend();

  void setDepth(int d);
  void setSpeed(int s);
  void setNote(int  n);

  Sint16 note;
  Sint16 speed;
  Sint16 depth;
  
  
};

