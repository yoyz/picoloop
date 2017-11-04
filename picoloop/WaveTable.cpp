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
  int i;
  if (size>0)
    {
      if (buffer==0)
	buffer=(Sint16*)malloc(sizeof(Sint16)*size);
      if (buffer==0)
	{
	  DPRINTF("Error allocating memory");
	  exit(1);
	  //return 0;
	}
      else
	{
	  DPRINTF("WaveTable::allocMemory(), buffer:0x%08.8X",buffer);
	}
    }
  for (i=0;i<size;i++) buffer[i]=0;
    
  return size;
}

int WaveTable::freeMemory()
{
  if (buffer!=0)
    {
      free(buffer);
      buffer=NULL;
    }
  return 0;
}

int WaveTable::setSize(int bufferSize)
{
  if (size==bufferSize)
    return 0;

  
  if (bufferSize>=0)
    size=bufferSize;
  
  if (buffer!=0)
    {
      DPRINTF("WARNING: Erasing previous memory\n");
      this->freeMemory();

    }
  return this->allocMemory();
}

Sint16 * WaveTable::getBuffer()
{
  DPRINTF("WaveTable::getBuffer(), buffer:0x%08.8X",buffer);
  return buffer;
}


