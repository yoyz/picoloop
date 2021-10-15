#ifndef __PATTERNSEQUENCER__
#define __PATTERNSEQUENCER__
#include "Master.h"
class PatternSequencer
{
 public:
  PatternSequencer();
  ~PatternSequencer();

  void reset();

  void setStep(int s);
  int  getStep();
  int  getStepWithoutDivider();

  void setBPMDivider(int bd);
  int  getBPMDivider();

  void setPatternLength(int pl);
  int  getPatternLength();

  void incStep();
  //int m_inc_step=0;
  int m_step=0;
 protected:
  int BPMDivider;
  int step;

  int PatternLength;


};

#endif

