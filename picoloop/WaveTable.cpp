#include "WaveTable.h"
using namespace std;

WaveTable::WaveTable()
{
  size=0;
  buffer=0;
  name=0;
}

WaveTable::~WaveTable()
{
  if (buffer!=0)
    {
      free(buffer);
      buffer=0;
    }  
}


int WaveTable::allocMemory()
{
  if (size>0)
    {
      buffer=(Sint16*)malloc(sizeof(Sint16)*size);
      if (buffer==0)
	{
	  printf("Error allocating memory\n");
	  return 0;
	}
    }
  return size;
}

int WaveTable::freeMemory()
{
  if (buffer!=0)
    {
      free(buffer);
      buffer=0;
    }
}

int WaveTable::setSize(int bufferSize)
{
  if (bufferSize>=0)
    size=bufferSize;
  
  if (buffer!=0)
    this->freeMemory();
  return this->allocMemory();
}

Sint16 * WaveTable::getBuffer()
{
  return buffer;
}


