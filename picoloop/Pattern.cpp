using namespace std;

#include "PatternElement.h"
#include "Pattern.h"


Pattern::Pattern() : pevector(16,PatternElement())
{
  size=16;
  bpmDivider=1;
  bpm=120;
}

Pattern::~Pattern()
{
  printf("Pattern::~Pattern()\n");
}

void Pattern::init()
{
  int i;
  printf("====Pattern::init()====\n");
  for (i=0;i<16-1;i++)
    {
      pevector[i].init();
    }
}


PatternElement & Pattern::getPatternElement(int patternElementNumber)
{
  return pevector[patternElementNumber];
}

bool Pattern::setPatternElement(int PatternElementNumber,PatternElement PEin)
{
  pevector[PatternElementNumber]=PEin;
}

void Pattern::setPatternSize(int patternSize)
{
  size=patternSize;
}

int Pattern::getSize()
{
  return size;
}

void Pattern::setLeftVolume(int left)
{
  leftVolume=left;
}

void Pattern::setRightVolume(int right)
{
  rightVolume=right;
}


int Pattern::getLeftVolume()
{
  return leftVolume;
}



int Pattern::getRightVolume()
{
  return rightVolume;
}


void Pattern::print()
{
  for ( int i=0; i<pevector.size(); i++)
    {
      string s;
      PatternElement PE;
      PE=pevector[i];
      s=PE.getStr();
      cout << i << ":" << s << "\n";
    }
}


int Pattern::getBPM()
{
  return bpm;
}

void Pattern::setBPM(int val)
{
  if (val>=20 && val <= 300)
    bpm=val;   
}

int Pattern::getBPMDivider()
{
  return bpmDivider;
}

void Pattern::setBPMDivider(int val)
{
  if (val==1  ||
      val==2  ||
      val==4  ||
      val==8)
    bpmDivider=val;
  else
    bpmDivider=1;
}
