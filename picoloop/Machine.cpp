#include "Machine.h"



Machine::Machine() 
{

  DPRINTF("Machine::Machine()");  

}



Machine::~Machine()
{
  DPRINTF("Machine::~Machine()");
}

void Machine::setChannelNumber(int c)
{
  //printf("void Machine::setChannelNumber(%d)\n",c);
  channel=c;
}

int Machine::getChannelNumber()
{
  return channel;
}

void Machine::setI(int what,int val)
{
  
}

void Machine::setF(int what,float val)
{
  
}


int Machine::getI(int what)
{
  return -1;
}


void Machine::init()
{
  DPRINTF("Machine::init()");
}



void Machine::reset()
{
  DPRINTF("Machine::reset()");
}


Sint32 Machine::tick()
{
  DPRINTF("Machine::tick()");
  return 0;
}
Fixed Machine::tick_fixed()
{
  Fixed a=0;
  DPRINTF("Machine::tick_fixed()");
  return a;
}

Fixed * Machine::tick_fixed_buffer()
{
  Fixed * a=NULL;
  DPRINTF("Machine::tick_fixed_buffer()");
  return a;
}



int Machine::checkI(int what,int val)
{
  DPRINTF("WARNING : Machine::checkI()");
  return val;
}

int Machine::checkITwoVal(int what,int val,int val2)
{
  DPRINTF("WARNING : Machine::checkITwoVal()");
  return val;
}


const char * Machine::getMachineParamCharStar(int machineParam,int paramValue)
{
  const char * str_null="NOLL";
  DPRINTF("Machine::getMachineParamCharStar");
  return str_null;
}

const char * Machine::getMachineTwoParamCharStar(int machineParam,int paramValue1, int paramValue2)
{
  const char * str_null="NULL";
  DPRINTF("Machine::getMachineTwoParamCharStar");
  return str_null;
}

