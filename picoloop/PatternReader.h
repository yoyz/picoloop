using namespace std;

#ifndef __PATTERNREADER__
#define __PATTERNREADER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Pattern.h"

#define DATA_DOES_NOT_EXIST_ON_STORAGE   3
#define DATA_LOADED_FROM_STORAGE         2
#define DATA_EXIST_ON_STORAGE            1
#define DATA_HAS_NOT_BEEN_CHECK          0

class PatternReader
{
 public:
  PatternReader();
  ~PatternReader();

  void init();
  void setBank(int b);
  int  getBank();
  void setFileName(string filename);
  bool PatternRemove(int PatternNumber,int TrackNumber);
  bool PatternDataExist(int PatternNumber,int TrackNumber);
  bool readPatternData(int PatternNumber,int TrackNumber, Pattern & P);
  bool writePattern(int PatternNumber,int TrackNumber,Pattern & P);



 private:
  int    bank;
  FILE * fd;
  string fn;
  vector < vector < Pattern > > twoDPVector;
  vector < vector < int     > > loadedData;
  //vector < vector < int > > savedData;
  
};

#endif
