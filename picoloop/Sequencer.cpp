using namespace std;

#include "Sequencer.h"
#include "PatternSequencer.h"

Sequencer::Sequencer() : psvector(4,PatternSequencer())
{
  tempo=120;
  current_track_x=0;
  current_track_y=0;
}

Sequencer::~Sequencer()
{
}


int Sequencer::getCurrentTrackX()
{
  return current_track_x;
}

int Sequencer::getCurrentTrackY()
{
  return current_track_y;
}

int Sequencer::gctx()
{
  return this->getCurrentTrackX();
}

int Sequencer::gcty()
{
  return this->getCurrentTrackY();
}


int Sequencer::setCurrentTrackX(int x)
{
  current_track_x=x;
  return x;
}

int Sequencer::setCurrentTrackY(int y)
{
  current_track_y=y;
  return y;
}

PatternSequencer & Sequencer::getPatternSequencer(int ps)
{
  return psvector[ps];
}

