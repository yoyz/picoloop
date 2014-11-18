#include "PitchBend.h"
#include "NoteFreq.h"

PitchBend::PitchBend()
{
  depth=0;
  speed=0; 
  note=0;
  sample_num=0;
}

PitchBend::~PitchBend()
{

}

void PitchBend::setDepth(int d)
{
  depth=d;
}

void PitchBend::setSpeed(int s)
{
  speed=s;
}


void PitchBend::setNote(int  n)
{
  note=n;
}

void PitchBend::reset()
{
  sample_num=0;
}


int  PitchBend::tickNoteDetune()
{
  int val;
  sample_num++;
  //val=((note<<7)+64)+((depth<<9)/(((sample_num)/((speed)+1))+1)); // <= this one works
  val=((note<<7)+64)+(((depth-64)<<9)/(((sample_num)/((speed*4)+1))+1)); // <= this one works



  //printf("                                                                   val:%d\n",val);
  return val;
}


