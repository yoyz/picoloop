#include "Track.h"

Track::Track()
{
}

Machine & Track::getMachine()
{
  return M;
}

int Track::tick()
{
  return M.tick();
}
