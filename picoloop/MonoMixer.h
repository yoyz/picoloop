using namespace std;

#include "Machine.h"

#ifndef __MONOMIXER____
#define __MONOMIXER____

class MonoMixer
{
 public:
  MonoMixer();
  ~MonoMixer();
  void        setAmplitude(int amp);
  //  void        setInput(Machine * M);
  Machine &   getInput();
  Sint16      tick();

 private:
  Machine      M;
  Sint16       amplitude;
};


#endif
