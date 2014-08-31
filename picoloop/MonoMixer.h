using namespace std;

#include "Machine.h"
#include "PicosynthMachine.h"
#include "PicodrumMachine.h"
#include "DboplMachine.h"
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
  Sint16      tick();

 private:
  Machine              *M;
  PicosynthMachine      PM;
  PicodrumMachine       PD;
  dboplMachine          OPLM;
  Sint16                amplitude;
  int                   machine_type;
};


#endif
