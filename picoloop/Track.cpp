#include "Track.h"

Track::Track() : MM()
{
  MM.setAmplitude(127);
}

Track::~Track()
{
  printf("Track::~Track()\n");  
}

/*
Machine & Track::getMachine()
{
  return M;
}
*/

MonoMixer & Track::getMonoMixer()
{
  return MM;
}


int Track::tick()
{
  //return M.tick();
  return MM.tick();
}
