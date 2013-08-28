using namespace std;

#ifndef __PATTERNREADER__
#define __PATTERNREADER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Pattern.h"
#include <vector>

class PatternReader
{
 public:
  PatternReader();

  void setFileName(char * filename);
  bool readPatternData(int PatternNumber,int TrackNumber, Pattern & P);
  bool writePattern(int PatternNumber,int TrackNumber,Pattern & P);

 private:
  FILE * fd;
  char * fn;
};

#endif
