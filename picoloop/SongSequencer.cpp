#include "SongSequencer.h"

SongSequencer::SongSequencer() : songVector(MAX_PATTERN_BY_PROJECT,vector <int>(TRACK_MAX))
{
  int x,y;
  for (x=0;x<MAX_PATTERN_BY_PROJECT;x++)
    for (y=0;y<TRACK_MAX-1;y++)
      {
	songVector[x][y]=0;
      }
  step=0;
  loopStepB=MAX_PATTERN_BY_PROJECT-1;
}

SongSequencer::~SongSequencer()
{

}



int SongSequencer::incStep()
{
  step++;
  if (step>loopStepB)
    step=loopStepA;
}

int SongSequencer::getStep()
{
  return step;
}

int SongSequencer::setStep(int s)
{
  loopStepA=s;
  step=s;
}

int SongSequencer::setLoopPoint(int loop)
{
  if (loop<=step)
    step=loopStepA;

  loopStepA=step;
  loopStepB=loop;
}

int SongSequencer::getLoopA()
{
  return loopStepA;
}

int SongSequencer::getLoopB()
{
  return loopStepB;
}


void SongSequencer::setPatternNumber(int x,int y,int patternNumber)
{
  int pn=patternNumber;
  if      (pn>MAX_PATTERN_BY_PROJECT-1) pn=0;
  else if (pn<0)                        pn=MAX_PATTERN_BY_PROJECT-1;
  songVector[x][y]=pn;
  //printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%d\n",patternNumber);
}

int SongSequencer::getPatternNumber(int cp,int trackNumber)
{
  return songVector[cp][trackNumber];
}



int SongSequencer::getPatternNumberAtCursorPosition(int trackNumber)
{
  return songVector[step][trackNumber];
}


vector <vector <int> > SongSequencer::getSongVector()
{
  return songVector;
}
