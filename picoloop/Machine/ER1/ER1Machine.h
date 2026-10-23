#include "../../Machine.h"
#include "er1_voice.h"

#ifndef __ER1MACHINE____
#define __ER1MACHINE____

/*
  Korg ER-1 style analog-modeling percussion voice, adapted to the picoloop
  Machine interface.

  The parameter storage is deliberately reusing existing PatternElement
  fields (see ER1Machine.cpp) so that no change is needed in PatternElement,
  PatternReader or seq_update_track().
*/
class ER1Machine : public Machine
{
 public:
  ER1Machine();
  ~ER1Machine();

  void   init();
  void   reset();
  Sint32 tick();

  void   setI(int what,int val);
  void   setF(int what,float val);
  int    getI(int what);
  int    checkI(int what,int val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 private:
  enum { ER1_SAM = 32 };

  void   applyPitch();
  void   applyLevel();

  ER1Voice voice;

  Sint32 buffer[ER1_SAM];
  float  stereo[ER1_SAM * 2];
  int    index;

  int    note;
  int    scale;
  int    velocity;   // 0..127, scales Level (extension)
  double level;      // 0..100, from OSC1_AMP
};

#endif
