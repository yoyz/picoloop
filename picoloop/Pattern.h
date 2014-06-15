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
  int size;
  int leftVolume;
  int rightVolume;


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


  vector<PatternElement> pevector;  



  
};
#endif
