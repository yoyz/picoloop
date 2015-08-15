#include "Machine.h"



Machine::Machine() 
{

  DPRINTF("Machine::Machine()\n");  

}



Machine::~Machine()
{
  DPRINTF("Machine::~Machine()\n");
}


void Machine::setI(int what,int val)
{
  
}

void Machine::setF(int what,float val)
{
  
}


int Machine::getI(int what)
{
  
}


void Machine::init()
{
  DPRINTF("Machine::init()\n");
}



void Machine::reset()
{
  DPRINTF("Machine::reset()\n");
}


int Machine::tick()
{
  DPRINTF("Machine::tick()\n");
}


int Machine::checkI(int what,int val)
{
  DPRINTF("WARNING : Machine::checkI()\n");
  return val;
}

int Machine::checkITwoVal(int what,int val,int val2)
{
  DPRINTF("WARNING : Machine::checkITwoVal()\n");
  return val;
}


const char * Machine::getMachineParamCharStar(int machineParam,int paramValue)
{
  const char * str_null="NOLL";
  DPRINTF("Machine::getMachineParamCharStar\n");
  return str_null;
}

const char * Machine::getMachineTwoParamCharStar(int machineParam,int paramValue1, int paramValue2)
{
  const char * str_null="NULL";
  DPRINTF("Machine::getMachineTwoParamCharStar\n");
  return str_null;
}

