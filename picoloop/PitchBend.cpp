#include "PitchBend.h"
#include "NoteFreq.h"

PitchBend::PitchBend()
{
  depth=0;
  speed=0; 
  note=0;
  formula1=0;
  formula2=0;
  sample_num=0;
}

PitchBend::~PitchBend()
{

}



void PitchBend::setDepth(int d)
{
  depth=d;
  this->calc();

}

void PitchBend::setSpeed(int s)
{
  speed=s;
  this->calc();
}


void PitchBend::setNote(int  n)
{
  note=n;
  this->calc();
}

void PitchBend::reset()
{
  sample_num=0;
}

void PitchBend::calc()
{
  formula1=((note<<7)+64);
  formula2=((((depth-64)<<9)*((speed*4)+1))+1);
}


int  PitchBend::tickNoteDetune()
{
  Sint32 val;
  sample_num++;
  //val=((note<<7)+64)+((depth<<9)/(((sample_num)/((speed)+1))+1)); // <= this one works
  //val=((note<<7)+64)+(((depth-64)<<9)/(((sample_num)/((speed*4)+1))+1)); // <= this one works
  //val=(((note<<7)+64)+(((depth-64)<<9)))/(sample_num+1);

  //val=((note<<7)+64)+((((depth-64)<<9)*((speed*4)+1))+1)/(((sample_num))); // <= new one easier to speedup
  //val=((note<<7)+64)+((((depth-64)<<9)*((speed*4)+1))+1)/(((sample_num))); // <= new one easier to speedup
  val=formula1+formula2/(((sample_num))); // <= new one easier to speedup



  //printf("                                                                   val:%d\n",val);
  return val;
}


