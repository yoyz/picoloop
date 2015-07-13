#include "Track.h"

Track::Track() : MM()
{
}



Track::~Track()
{
  DPRINTF("Track::~Track()\n");  
}


/*
Machine & Track::getMachine()
{
  return M;
}
*/

MonoMixer * Track::getMonoMixer()
{
  DPRINTF("MonoMixer * Track::getMonoMixer() this=0x%08.8X\n",this);
  return &MM;
}


int Track::tick()
{
  //return M.tick();
  return MM.tick();
}
