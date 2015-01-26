#include "Machine.h"



Machine::Machine() 
{

  printf("Machine::Machine()\n");  

}



Machine::~Machine()
{
  printf("Machine::~Machine()\n");
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
  printf("Machine::init()\n");
}



void Machine::reset()
{
  printf("Machine::reset()\n");
}


int Machine::tick()
{
  printf("Machine::tick()\n");
}


int Machine::checkI(int what,int val)
{
  printf("WARNING : Machine::checkI()\n");
  return val;
}

const char * Machine::getMachineParamCharStar(int machineParam,int paramValue)
{
  const char * str_null="NULL";
  printf("Machine::getMachineParamCharStar\n");
  return str_null;
}

