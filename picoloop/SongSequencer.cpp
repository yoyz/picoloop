#include "SongSequencer.h"

SongSequencer::SongSequencer() : songVector(MAX_PATTERN_BY_PROJECT,vector <int>(TRACK_MAX))
{
  int x,y;
  for (x=0;x<MAX_PATTERN_BY_PROJECT;x++)
    for (y=0;y<TRACK_MAX-1;y++)
      {
	songVector[x][y]=0;
      }
  cursorPosition=0;
}

SongSequencer::~SongSequencer()
{

}



int SongSequencer::incStep()
{
  cursorPosition++;
}

int SongSequencer::getStep()
{
  return cursorPosition;
}

int SongSequencer::setStep(int step)
{
  cursorPosition=step;
}


void SongSequencer::setPatternNumber(int x,int y,int patternNumber)
{
  int pn=patternNumber;
  if      (pn>MAX_PATTERN_BY_PROJECT-1) pn=0;
  else if (pn<0)                        pn=MAX_PATTERN_BY_PROJECT-1;
  songVector[x][y]=pn;
  printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%d\n",patternNumber);
}

int SongSequencer::getPatternNumber(int cp,int trackNumber)
{
  return songVector[cp][trackNumber];
}



int SongSequencer::getPatternNumberAtCursorPosition(int trackNumber)
{
  return songVector[cursorPosition][trackNumber];
}
