using namespace std;

#include "MonoMixer.h"

#ifndef __TRACK____
#define __TRACK____
//#include "Machine.h"


class Track
{
 public:
  Track();

  //  Machine M;
  MonoMixer MM;
  int tick();
  //Machine & getMachine();
  MonoMixer * getMonoMixer();

};

#endif
  
