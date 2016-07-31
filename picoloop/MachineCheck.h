#ifndef __MACHINE_CHECK__
#define __MACHINE_CHECK__

#include "Master.h"

class MachineCheck
{
 public:
  MachineCheck();
  ~MachineCheck();
  int getNext(int machineType);
  int getPrevious(int MachineType);
  int machineExist(int machineType);
};

#endif
