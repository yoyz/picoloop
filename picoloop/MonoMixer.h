using namespace std;

#include "Machine.h"
#include "PicosynthMachine.h"
#include "dboplMachine.h"
#include <SDL/SDL_types.h>

#ifndef __MONOMIXER____
#define __MONOMIXER____

class MonoMixer
{
 public:
  MonoMixer();
  ~MonoMixer();
  void        init();
  void        setAmplitude(int amp);
  //  void        setInput(Machine * M);
  Machine *   getInput();
  Sint16      tick();

 private:
  Machine              *M;
  PicosynthMachine      PM;
  dboplMachine          OPLM;
  Sint16                amplitude;
};


#endif
