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
  PatternElement & getPatternElement(int patternElementNumber);
  bool setPatternElement(int PatternElementNumber,PatternElement PEin);
  void print();
  int  getSize();
  void setSize(int s);

  void setLeftVolume(int left);
  void setRightVolume(int right);

  int getLeftVolume();
  int getRightVolume();

  float  getBPM();
  //void setBPM(int val);
  void setBPM(float val);

  int  getBPMDivider();
  void setBPMDivider(int val);

  int  getSwing();
  void setSwing(int val);

 protected:

  std::vector<PatternElement> pevector;  
  int size;
  int leftVolume;
  int rightVolume;
  int bpmDivider;
  //int bpm;
  float bpm;
  int swing;

  
};
#endif
