using namespace std;

#ifndef __SEQUENCER__
#define __SEQUENCER__

class Sequencer
{
 public:
  Sequencer();
  ~Sequencer();

  int getCurrentTrackX();
  int getCurrentTrackY();
  int gctx();              // shortcut to getCurrentTrackX
  int gcty();              // shortcut to getCurrentTrackY

  int setCurrentTrackX(int x);
  int setCurrentTrackY(int y);


 public:

    int         tempo;           // global tempo

    int         current_track_x; // Cursor position of track
    int         current_track_y; // Cursor position of track
};

#endif
