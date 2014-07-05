using namespace std;

#ifndef __PATTERNSEQUENCER__
#define __PATTERNSEQUENCER__

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

  void incStep();

 protected:
  int BPMDivider;
  int step;

};

#endif

