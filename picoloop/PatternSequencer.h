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

  void setStepDivider(int sd);
  int  getStepDivider();

  void incStep();

 protected:
  int stepDivider;
  int step;

};

#endif

