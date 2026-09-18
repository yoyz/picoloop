#ifndef __PATTERNREADER__
#define __PATTERNREADER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Pattern.h"
#include "SongSequencer.h"
#include "Machine.h"
#include "Master.h"
#define DATA_LOADED_FROM_STORAGE         3
#define DATA_EXIST_ON_STORAGE            2
#define DATA_DOES_NOT_EXIST_ON_STORAGE   1
#define DATA_HAS_NOT_BEEN_CHECK          0

class PatternReader
{
 public:
  PatternReader();
  ~PatternReader();

  void init();
  void setBank(int b);
  int  getBank();

  // Redirects saveSong/loadSong/writePattern/readPatternData to read/write
  // directly in `root` instead of GETPICOLOOPUSERSTORAGE()/bank/bank<N> -
  // used for the Android session snapshot, which must live outside the
  // numbered bank tree (see PatternPlayer.cpp's autosaveCurrentState()).
  // Pass "" to go back to the normal bank/bank<N> path.
  void setCustomRoot(const char * root);

  int  saveSong(SongSequencer & SS);
  int  loadSong(SongSequencer & SS);

  void setFileName(std::string filename);
  bool PatternRemove(int PatternNumber,int TrackNumber);
  bool PatternDataExist(int PatternNumber,int TrackNumber);
  bool readPatternData(int PatternNumber,int TrackNumber, Pattern & P);
  bool writePatternDataLine(int PatternNumber,int TrackNumber, Pattern & P, char * line, int machineParam);
  bool readPatternDataLine(int PatternNumber,int TrackNumber, Pattern & P, char * line, int machineParam);
  bool writePattern(int PatternNumber,int TrackNumber,Pattern & P);

  const char * getParameterCharStar(int param);


 private:
  void bankPath(char * out, size_t outSize);

  int    bank;
  char   customRoot[512];
  FILE * fd;
  std::string fn;
  std::vector < std::vector < Pattern > > twoDPVector;
  std::vector < std::vector < int     > > loadedData;
  //vector < vector < int > > savedData;
  
};

#endif
