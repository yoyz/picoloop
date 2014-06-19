using namespace std;

#include <string>
#include <stdio.h>
#include "PatternElement.h"

#ifndef __PATTERN__
#define __PATTERN__

#include <iostream>
#include <vector>

class Pattern
{
 public:


  Pattern();
  ~Pattern();
  void init();
  void setPatternSize(int patternSize);
  PatternElement & getPatternElement(int patternElementNumber);
  bool setPatternElement(int PatternElementNumber,PatternElement PEin);
  void print();
  int getSize();

  void setLeftVolume(int left);
  void setRightVolume(int right);

  int getLeftVolume();
  int getRightVolume();

  int  getBPM();
  void setBPM(int val);

  int  getBPMDivider();
  void setBPMDivider(int val);


 protected:

  vector<PatternElement> pevector;  
  int size;
  int leftVolume;
  int rightVolume;
  int bpmDivider;
  int bpm;

  
};
#endif
