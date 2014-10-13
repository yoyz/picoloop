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


int SongSequencer::getPatternNumber(int trackNumber)
{
  return songVector[cursorPosition][trackNumber];
}

void SongSequencer::setPatternNumber(int cursorPosition,int trackNumber,int patternNumber)
{
  songVector[cursorPosition][trackNumber]=patternNumber;
}
