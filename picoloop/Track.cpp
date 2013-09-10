#include "Track.h"

Track::Track() : MM()
{
  MM.setAmplitude(127);
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
