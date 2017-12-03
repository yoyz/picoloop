#include "PatternSequencer.h"
#include "SongSequencer.h"

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

  SongSequencer    & getSongSequencer();
  PatternSequencer & getPatternSequencer(int ps);
  int setCurrentTrackX(int x);
  int setCurrentTrackY(int y);

  int  getSongMode();
  void setSongMode(int s);


 public:

    int         tempo;           // global tempo

    int         current_track_x; // X Cursor position in the GUI
    int         current_track_y; // Y Cursor position in the GUI

    std::vector<PatternSequencer> psvector;
    SongSequencer SS;
    int         song_mode;        // 0 live mode | 1 song mode
};

#endif
