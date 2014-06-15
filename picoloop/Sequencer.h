using namespace std;

#include "PatternSequencer.h"

#ifndef __SEQUENCER__
#define __SEQUENCER__
#include <vector>

class Sequencer
{
 public:
  Sequencer();
  ~Sequencer();

  int getCurrentTrackX();
  int getCurrentTrackY();
  int gctx();              // shortcut to getCurrentTrackX
  int gcty();              // shortcut to getCurrentTrackY

  PatternSequencer & getPatternSequencer(int ps);
  int setCurrentTrackX(int x);
  int setCurrentTrackY(int y);


 public:

    int         tempo;           // global tempo

    int         current_track_x; // Cursor position of track
    int         current_track_y; // Cursor position of track
    vector<PatternSequencer> psvector;  
};

#endif
