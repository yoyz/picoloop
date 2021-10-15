#include "PatternElement.h"
#include "Pattern.h"
#include <string>

#ifndef __PATTERNPLAYER__
#define __PATTERNPLAYER__

std::vector<std::vector<std::vector<std::vector<int> > > > osc_array;

class PatternPlayer
{
 public:
  PatternPlayer();

 private:
  Pattern P;

};

#endif
