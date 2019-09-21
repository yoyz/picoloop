#include "MachineCheck.h"

MachineCheck::MachineCheck()
{
}

MachineCheck::~MachineCheck()
{
}

int MachineCheck::machineExist(int machineType)
{
#ifdef      ENABLE_SYNTH_PICOSYNTH
  if (machineType==SYNTH_PICOSYNTH)   return 1;
#endif
#ifdef      ENABLE_SYNTH_OPL2
  if (machineType==SYNTH_OPL2)        return 1;
#endif
#ifdef      ENABLE_SYNTH_PICODRUM
  if (machineType==SYNTH_PICODRUM)    return 1;
#endif
#ifdef      ENABLE_SYNTH_PBSYNTH
  if (machineType==SYNTH_PBSYNTH)     return 1;
#endif
#ifdef      ENABLE_SYNTH_CURSYNTH
  if (machineType==SYNTH_CURSYNTH)    return 1;
#endif
#ifdef      ENABLE_SYNTH_OPEN303
  if (machineType==SYNTH_OPEN303)     return 1;
#endif
#ifdef      ENABLE_SYNTH_TWYTCHSYNTH
  if (machineType==SYNTH_TWYTCHSYNTH) return 1;
#endif
#ifdef      ENABLE_SYNTH_MDADRUM
  if (machineType==SYNTH_MDADRUM)     return 1;
#endif
#ifdef      ENABLE_SYNTH_SIDSYNTH
  if (machineType==SYNTH_SIDSYNTH)    return 1;
#endif
#ifdef      ENABLE_SYNTH_LGPTSAMPLER
  if (machineType==SYNTH_LGPTSAMPLER) return 1;
#endif
#ifdef      ENABLE_SYNTH_MIDIOUT
  if (machineType==SYNTH_MIDIOUT)     return 1;
#endif
  DPRINTF("MachineCheck::machineExist: %d Not Found\n",machineType);
  return 0;
}
/*
int MachineCheck::machineExist(int machineType)
{
#if !defined(__PBSYNTHONLY__)
  if (machineType==SYNTH_PICOSYNTH)   return 1;
  if (machineType==SYNTH_OPL2)        return 1;
  if (machineType==SYNTH_PICODRUM)    return 1;
#endif
  if (machineType==SYNTH_PBSYNTH)     return 1;
#if    defined(__VECTORFPU__)
  if (machineType==SYNTH_CURSYNTH)    return 1;
  if (machineType==SYNTH_OPEN303)     return 1;
  if (machineType==SYNTH_TWYTCHSYNTH) return 1;
  if (machineType==SYNTH_MDADRUM)     return 1;
  if (machineType==SYNTH_SIDSYNTH)    return 1;
#endif
#if    defined(__RAM512MIB__)
  if (machineType==SYNTH_LGPTSAMPLER) return 1;
#endif
#if    defined(__RTMIDI__)
  if (machineType==SYNTH_MIDIOUT)     return 1;
#endif
  return 0;
}
*/

int MachineCheck::getNext(int machineType)
{
  int found=0;
  int i;

  for (i=machineType+1;i<127;i++)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d",machineType,i);
	  return i;
	}
    }

  for (i=0;i<machineType;i++)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d",machineType,i);
	  return i;
	}
    }
  if (machineExist(machineType)) return i;
  DPRINTF("exit(3) MachineCheck->getNext(%d)",machineType);
  DPRINTF("exit(3) MACHINE NOTE FOUND");
  CLOSE_DPRINTF();
  exit(3);
  return -1;
}

int MachineCheck::getPrevious(int machineType)
{
  int found=0;
  int i;

  for (i=machineType-1;i>=0;i--)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d",machineType,i);
	  return i;
	}
    }

  for (i=127;i>=machineType;i--)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d",machineType,i);
	  return i;
	}
    }

  DPRINTF("exit(3) MachineCheck->getPrevious(%d)",machineType);
  DPRINTF("exit(3) MACHINE NOTE FOUND");
  CLOSE_DPRINTF();
  exit(3);
}
