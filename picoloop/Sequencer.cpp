using namespace std;

#include "Master.h"
#include "Sequencer.h"
#include "PatternSequencer.h"

Sequencer::Sequencer() : psvector(TRACK_MAX,PatternSequencer()),
			 SS()
{
  song_mode=0;
  tempo=120;
  current_track_x=0;
  current_track_y=0;
}

Sequencer::~Sequencer()
{
}

// X Cursor position in the GUI
int Sequencer::getCurrentTrackX()
{
  return current_track_x;
}

// Y Cursor position in the GUI
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

SongSequencer & Sequencer::getSongSequencer()
{
  return SS;
}


int Sequencer::getSongMode()
{
  return song_mode;
}

void Sequencer::setSongMode(int s)
{
  song_mode=s;
}


