using namespace std;

#include "Machine.h"
#include "Machine/Picosynth/PicosynthMachine.h"
#include "Machine/Picodrum/PicodrumMachine.h"
#include "Machine/Dbopl/DboplMachine.h"
//#include "Effect.h"
#include "EffectDelay.h"
#include <SDL/SDL_types.h>

#ifndef __MONOMIXER____
#define __MONOMIXER____

class MonoMixer
{
 public:
  MonoMixer();
  ~MonoMixer();
  void        init();
  void        setMachineType(int type);
  void        setAmplitude(int amp);
  //  void        setInput(Machine * M);
  Machine *   getInput();
  Effect  *   getEffect();
  Sint16      tick();

 private:

  Machine              *M;
  PicosynthMachine      PM;
  PicodrumMachine       PD;
  dboplMachine          OPLM;

  Effect                *FX;
  EffectDelay           FXDelay;

  Sint16                amplitude;
  int                   machine_type;

  int                   fx_depth;
  int                   fx_speed;
};


#endif
