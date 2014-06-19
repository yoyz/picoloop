#include "PatternSequencer.h"

PatternSequencer::PatternSequencer()
{
  step=0;
  BPMDivider=1;
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
    return step%16;
  if (BPMDivider==2)
    return (step/2)%16;
  if (BPMDivider==4)
    return (step/4)%16;
  if (BPMDivider==8)
    return (step/8)%16;
}


void PatternSequencer::incStep()
{
  step++;
  if (step>=128)
    step=0;
  
}


void PatternSequencer::setBPMDivider(int bd)
{
  if (bd==1)
    BPMDivider=bd;
  else 
    if (bd==2)
      BPMDivider=bd;
    else 
      if (bd==4)
	BPMDivider=bd;
      else 
	if (bd==8)
	  BPMDivider=bd;
  
  if (bd!=BPMDivider)
    BPMDivider=1;
}

int PatternSequencer::getBPMDivider()
{
  return BPMDivider;
}

