using namespace std;

#ifndef __PATTERNREADER__
#define __PATTERNREADER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Pattern.h"


class PatternReader
{
 public:
  PatternReader();

  void setFileName(string filename);
  bool readPatternData(int PatternNumber,int TrackNumber, Pattern & P);
  bool writePattern(int PatternNumber,int TrackNumber,Pattern & P);

 private:
  FILE * fd;
  string fn;
};

#endif
