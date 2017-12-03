#include <stdio.h>

#include "Master.h"
#include "InputManager.h"
#include "AudioEngine.h"
#include "SDL_GUI.h"
#include "Pattern.h"
#include "Machine.h"
#include "MonoMixer.h"
#include "PatternReader.h"
#include "Sequencer.h"
#include "TweakableKnob.h"
#include <vector>


#ifndef __USERINTERFACE____
#define __USERINTERFACE____

extern vector <Pattern>            P;  
extern vector <Machine   *>        M;
extern vector <MonoMixer *>        MM;
extern vector <Effect    *>        FX;

extern MonoMixer                   SAMM; // Standalone MonoMixer 
                                  //  used to get   a standalone Machine SAM
                                  //  used to check the boundary of value of different element
                                  //  it is used in seq_update_multiple_time_by_step()

extern Machine  *                   SAM; // Standalone Machine
                                  //  it is used in seq_update_multiple_time_by_step()


extern Sequencer      SEQ;         // used to  store/get information about sequencer 
extern AudioEngine    AE;          // used to  init alsa/rtaudio
extern PatternReader  PR;          // used to  read data.pic file
extern PatternElement PE;          // used for copy paste PatternElement
extern PatternElement PECursor;    // used for storing the current PatternElement ( under the cursor )
extern InputManager   IE;          // used to  fetch key
extern SDL_GUI        SG;          // used to  open a gui and display stuff
extern TweakableKnob  TK;          // used to  store "variation" which will be applied on Pattern/PatternElement object

extern int menu_cursor;
extern int menu_ad;

extern int cursor;
extern int pattern_display_offset[TRACK_MAX];


class UserInterface
{
 public:
  UserInterface();
  ~UserInterface();
  virtual void handle_key(int menu);
  virtual void display_board(int menu);  
  virtual void display_board_text();  
  int a;
};

#endif
