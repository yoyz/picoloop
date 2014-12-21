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
  div=1;
  speed_calc=1;
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
  formula2=(depth-64)<<9;
  div=1;
  speed_calc=speed*6;
}


// tickNoteDetune() return a NoteDetune which converge with the Note without PitchBend
//     0-63 negative < pitchBendDefaultValue 64 < 65-127 positive
//
// 0                 < val                      < 88*127=11176 <= out 
// 0                 < formula1                 < 88*127=11176  <= not without pitchbend
// - 88*127=11176    < formula2                 < 88*127=11176 ?? 

int  PitchBend::tickNoteDetune()
{
  Sint32 val;

  sample_num++;

  if (sample_num==speed_calc)
    {
      div++;
      sample_num=0;
    }
  val=formula1+formula2/div; // <= new one easier to speedup



  //printf("                                                                   val:%d\n",val);
  return val;
}


