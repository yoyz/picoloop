using namespace std;

#ifndef __TRACK____
#define __TRACK____
#include "Machine.h"

class Track
{
 public:
  Track();

  Machine M;
  
  int tick();
  Machine & getMachine();

};

#endif
  
