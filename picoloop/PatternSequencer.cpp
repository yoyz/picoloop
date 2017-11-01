#include "PatternSequencer.h"

PatternSequencer::PatternSequencer()
{
  step=0;
  BPMDivider=1;
  PatternLength=16;
}

PatternSequencer::~PatternSequencer()
{
}


void PatternSequencer::reset()
{
  step=0;
  BPMDivider=1;
}

void PatternSequencer::setStep(int s)
{
  if(BPMDivider==1)
    step=s;
  if(BPMDivider==2)
    step=s*2;
  if(BPMDivider==4)
    step=s*4;
  if(BPMDivider==8)
    step=s*8;
}


int PatternSequencer::getStep()
{
  if (BPMDivider==1)
    return step%PatternLength;
  if (BPMDivider==2)
    return (step/2)%PatternLength;
  if (BPMDivider==4)
    return (step/4)%PatternLength;
  if (BPMDivider==8)
    return (step/8)%PatternLength;
  DPRINTF("Mal formed int PatternSequencer::getStep()\n");  
  return 0;
}

void PatternSequencer::setPatternLength(int pl)
{
  if (pl>0 && pl<=128)
    PatternLength=pl;
}


int PatternSequencer::getPatternLength()
{
  return PatternLength;
}



int PatternSequencer::getStepWithoutDivider()
{
  return step;
}


void PatternSequencer::incStep()
{
  step++;
  if (step>=128)
    step=step%PatternLength;
    //step=0;
  
}


void PatternSequencer::setBPMDivider(int bd)
{
  if (bd==1)
    BPMDivider=bd;
  else if (bd==2)
    BPMDivider=bd;
  else if (bd==4)
    BPMDivider=bd;
  else if (bd==8)
    BPMDivider=bd;
  
  if (bd!=BPMDivider)
    BPMDivider=1;
}

int PatternSequencer::getBPMDivider()
{
  return BPMDivider;
}

