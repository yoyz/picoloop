#include "TweakableKnob.h"

TweakableKnob::TweakableKnob()
{
  machineParamArray=0;
  machineParamArrayAll=0;
}

TweakableKnob::~TweakableKnob()
{
  if (machineParamArray==0)
    free(machineParamArray);
  if (machineParamArrayAll==0)
    free(machineParamArrayAll);
}
  
void TweakableKnob::reset()
{
  int i;
  for (i=0;i<NBELEM_TWEAKABLEKNOB;i++)
    {
      machineParamArray[i]=0;
      machineParamArrayAll[i]=0;
    }
}  


void TweakableKnob::init()
{
  if (machineParamArray==0)
    machineParamArray=(int*)malloc(sizeof(int)*NBELEM_TWEAKABLEKNOB);
  if (machineParamArrayAll==0)
    machineParamArrayAll=(int*)malloc(sizeof(int)*NBELEM_TWEAKABLEKNOB);
  this->reset();
}


void TweakableKnob::set(int machineParam,int value)
{
  machineParamArray[machineParam]=value;
}


void TweakableKnob::setAll(int machineParam,int value)
{
  machineParamArrayAll[machineParam]=value;
}

int TweakableKnob::get(int machineParam)
{
  return machineParamArray[machineParam];
}

int TweakableKnob::getAll(int machineParam)
{
  return machineParamArrayAll[machineParam];
}

int TweakableKnob::getNonZero()
{
  int ret=-1;
  int i;
  for (i=0;i<NBELEM_TWEAKABLEKNOB;i++)
    {
      if (machineParamArray[i])
	return i;
    }
  return ret;
}

int TweakableKnob::getAllNonZero()
{
  int ret=-1;
  int i;
  for (i=0;i<NBELEM_TWEAKABLEKNOB;i++)
    {
      if (machineParamArrayAll[i])
	return i;
    }
  return ret;
}
