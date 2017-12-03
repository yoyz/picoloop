#include "SongSequencer.h"

SongSequencer::SongSequencer() : songVector(MAX_SONG_LENGHT_BY_PROJECT,std::vector <unsigned int>(TRACK_MAX))
{
  int x,y;
  for (x=0;x<MAX_SONG_LENGHT_BY_PROJECT;x++)
    for (y=0;y<TRACK_MAX-1;y++)
      {
	songVector[x][y]=0;
      }
  step=0;
  loopStepB=MAX_SONG_LENGHT_BY_PROJECT-1;
}

SongSequencer::~SongSequencer()
{

}


void SongSequencer::shiftTrackLeft(int x, int y)
{
  int i=y;
  for(i=x;i<MAX_SONG_LENGHT_BY_PROJECT;i++)
    {
      if (i>0 && i<MAX_SONG_LENGHT_BY_PROJECT-1)
	songVector[i][y]=songVector[i+1][y];
    }
}

void SongSequencer::shiftTrackRight(int x, int y)
{
  int i=y;
  for(i=MAX_SONG_LENGHT_BY_PROJECT-1;i>x;i--)
    {
      if (i>0 && i<MAX_SONG_LENGHT_BY_PROJECT-1)
	songVector[i][y]=songVector[i-1][y];
    }
}

int SongSequencer::incStep()
{
  step++;
  if (step>loopStepB)
    step=loopStepA;
  return 0;
}

int SongSequencer::getStep()
{
  return step;
}

int SongSequencer::setStep(int s)
{
  loopStepA=s;
  step=s;
  return 0;
}

int SongSequencer::setLoopPoint(int loop)
{
  if (loop<=step)
    step=loopStepA;

  loopStepA=step;
  loopStepB=loop;
  return 0;
}

int SongSequencer::getLoopA()
{
  return loopStepA;
}

int SongSequencer::getLoopB()
{
  return loopStepB;
}

// x,y is the coordinate in the song sequencer
// patternNumber is the pattern from the L/S ( the y position is known )
void SongSequencer::setPatternNumber(int x,int y,int patternNumber)
{
  int pn=patternNumber;
  if      (pn>MAX_PATTERN_BY_PROJECT-1) pn=0;
  else if (pn<0)                        pn=MAX_PATTERN_BY_PROJECT-1;

  if (x<0)                        x=MAX_SONG_LENGHT_BY_PROJECT-1;
  if (x>MAX_SONG_LENGHT_BY_PROJECT-1) x=0;

  printf("x:%d y:%d pn:%d\n",x,y,pn);
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

/*
vector <vector <int> > SongSequencer::getSongVector()
{
  return songVector;
}
*/
