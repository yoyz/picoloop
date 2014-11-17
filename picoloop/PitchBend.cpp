#include "PitchBend.h"
#include "NoteFreq.h"

PitchBend::PitchBend()
{
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


void PitchBend::setNote(int  n);
{
  note=n;
}
