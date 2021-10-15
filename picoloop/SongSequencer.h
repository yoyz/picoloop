#include <string>
#include <vector>
#include "Master.h"
#ifndef __SONGSEQUENCER__
#define __SONGSEQUENCER__

class SongSequencer
{

public:
  SongSequencer();
  ~SongSequencer();
  int  incStep();
  int  getStep();
  int  setStep(int step);
  int  setLoopPoint(int loopStep);
  int  getLoopA();
  int  getLoopB();
  void shiftTrackLeft(int x,int y);
  void shiftTrackRight(int x, int y);  
  int  getPatternNumber(int cp,int trackNumber);
  int  getPatternNumberAtCursorPosition(int trackNumber);
  void setPatternNumber(int cursorPosition,int trackNumber,int patternNumber);
  int m_master_loop=0;
  int m_inc = 0;
  int m_song_step=0;
  //vector <vector <unsigned int> > getSongVector();

  std::vector <std::vector <unsigned int> > songVector;
  
private:
  int                  step;
  int                  loopStepA;
  int                  loopStepB;

};

#endif
