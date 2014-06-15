#include "PatternSequencer.h"

PatternSequencer::PatternSequencer()
{
  step=0;
  stepDivider=1;
}

PatternSequencer::~PatternSequencer()
{
}


void PatternSequencer::reset()
{
  step=0;
  stepDivider=1;
}

void PatternSequencer::setStep(int s)
{
  if(stepDivider==1)
    step=s;
  if(stepDivider==2)
    step=s*2;
  if(stepDivider==4)
    step=s*4;
  if(stepDivider==8)
    step=s*8;
}


int PatternSequencer::getStep()
{
  if (stepDivider==1)
    return step%16;
  if (stepDivider==2)
    return (step/2)%16;
  if (stepDivider==4)
    return (step/4)%16;
  if (stepDivider==8)
    return (step/8)%16;
}


void PatternSequencer::incStep()
{
  step++;
  if (step>=128)
    step=0;
  
}


void PatternSequencer::setStepDivider(int sd)
{
  if (sd==1)
    stepDivider=sd;
  else 
    if (sd==2)
      stepDivider=sd;
    else 
      if (sd==4)
	stepDivider=sd;
      else 
	if (sd==8)
	  stepDivider=sd;
  
  if (sd!=stepDivider)
    stepDivider=1;
}

int PatternSequencer::getStepDivider()
{
  return stepDivider;
}

