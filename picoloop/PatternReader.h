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
  bool PatternDataExist(int PatternNumber,int TrackNumber);
  bool readPatternData(int PatternNumber,int TrackNumber, Pattern & P);
  bool writePattern(int PatternNumber,int TrackNumber,Pattern & P);

  void init();

 private:
  FILE * fd;
  string fn;
  vector < vector < Pattern > > twoDPVector;
  vector < vector < int > > loadedData;
  vector < vector < int > > savedData;
  
};

#endif
