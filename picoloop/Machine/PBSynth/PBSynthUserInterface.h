#include "UserInterface.h"
#include "Master.h"
#include "InputManager.h"
#include "SDL_GUI.h"
#include "Pattern.h"
#include "Machine.h"
#include "MonoMixer.h"
#include "PatternReader.h"
#include "Sequencer.h"
#include "TweakableKnob.h"
#include <vector>

#ifndef __PBSYNTHUSERINTERFACE____
#define __PBSYNTHUSERINTERFACE____



extern std::vector <Pattern>            P;  
extern std::vector <Machine   *>        M;
extern std::vector <MonoMixer *>        MM;
extern std::vector <Effect    *>        FX;

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

extern int menu;
extern int menu_cursor;
extern int menu_sub;
/* BEGIN this next one should be removed */
extern int menu_ad;
extern int menu_note;
extern int menu_vco;
extern int menu_osc;
extern int menu_lfo;
extern int menu_fltr;
/* END this next one should be removed */

extern int dirty_graphic;

extern int cursor;

extern int menu_ad_dirty_keyboard;





class PBSynthUserInterface : public UserInterface
{
 public:
  PBSynthUserInterface();
  ~PBSynthUserInterface();
  void handle_key(int menu);
  void handle_key_amp_env();
  void handle_key_note();
  void handle_key_osc();
  void handle_key_vco();
  void handle_key_lfo();
  void handle_key_fltr();

  void display_board_text();

  void display_board(int menu); 
  void display_board_amp_env();
  void display_board_note();
  void display_board_vco();
  void display_board_osc();
  void display_board_lfo();
  void display_board_fltr();

  int a;
};

#endif
