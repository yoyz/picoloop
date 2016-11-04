#include "MachineCheck.h"

MachineCheck::MachineCheck()
{
}

MachineCheck::~MachineCheck()
{
}

int MachineCheck::machineExist(int machineType)
{
  if (machineType==SYNTH_PICOSYNTH)   return 1;
  if (machineType==SYNTH_OPL2)        return 1;
  if (machineType==SYNTH_PICODRUM)    return 1;
  if (machineType==SYNTH_PBSYNTH)     return 1;
#if    defined(__FPU__)
  if (machineType==SYNTH_CURSYNTH)    return 1;
  if (machineType==SYNTH_OPEN303)     return 1;
  if (machineType==SYNTH_TWYTCHSYNTH) return 1;
  if (machineType==SYNTH_MDADRUM)     return 1;
  if (machineType==SYNTH_SIDSYNTH)    return 1;
#endif

#if    defined(__RTMIDI__)
  if (machineType==SYNTH_MIDIOUT)     return 1;
#endif
  return 0;
}

int MachineCheck::getNext(int machineType)
{
  int found=0;
  int i;

  for (i=machineType+1;i<127;i++)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d\n",machineType,i);
	  return i;
	}
    }

  for (i=0;i<machineType;i++)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d\n",machineType,i);
	  return i;
	}
    }
  printf("exit(3) MachineCheck->getNext(%d)\n",machineType);
  printf("exit(3) MACHINE NOTE FOUND\n");
  exit(3);
}

int MachineCheck::getPrevious(int machineType)
{
  int found=0;
  int i;

  for (i=machineType-1;i>=0;i--)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d\n",machineType,i);
	  return i;
	}
    }

  for (i=127;i>=machineType;i--)
    {
      if (machineExist(i))
	{
	  DPRINTF("getNext(%d)=%d\n",machineType,i);
	  return i;
	}
    }

  printf("exit(3) MachineCheck->getPrevious(%d)\n",machineType);
  printf("exit(3) MACHINE NOTE FOUND\n");
  exit(3);
}
