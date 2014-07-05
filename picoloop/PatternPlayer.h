using namespace std;

#include "PatternElement.h"
#include "Pattern.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#include "Master.h"
#include "Note.h"
#include "PatternReader.h"
//#include "PatternPlayer.h"
#include "AudioEngine.h"
#include "Wave.h"
#include "MonoMixer.h"
#include "SDL_GUI.h"
#include "InputManager.h"
#include "Machine.h"
#include "Sequencer.h"
#include "Machine.h"


#ifndef __PATTERNPLAYER__
#define __PATTERNPLAYER__


#define KEY_REPEAT_INTERVAL 400

#define BOX_COLOR           0xAECD15
#define TRIG_COLOR          0x0E4C15
#define NOTE_COLOR          0x46DC65
#define CURSOR_COLOR        0x1515CD
#define STEP_COLOR          0x242C45

#define SMALLBOX_COLOR      0x442233

#define DISABLE             0
#define ENABLE              1

#define BUTTON_B            SDLK_LALT
#define BUTTON_A            SDLK_LCTRL
#define BUTTON_X            SDLK_SPACE
#define BUTTON_Y            SDLK_LSHIFT

#define BUTTON_UP           SDLK_UP
#define BUTTON_DOWN         SDLK_DOWN
#define BUTTON_LEFT         SDLK_LEFT
#define BUTTON_RIGHT        SDLK_RIGHT

#define BUTTON_SELECT       SDLK_ESCAPE
#define BUTTON_START        SDLK_RETURN


//menu
enum {
  MENU_OFF,
  MENU_ON_PAGE1,
  MENU_ON_PAGE2
};

//menu_cursor
enum {
  M_AD,   // 0
  M_NOTE, // 1
  M_OSC,  // 2 
  M_VCO,  // 3

  M_LS,   // 4
  M_LFO,  // 5
  M_FLTR, // 6
  M_BPM   // 7
};

enum {
  MENU_ENV_ATTACK_RELEASE,
  MENU_ENV_ATTACK_AMP,
  MENU_ENV_AMP_RELEASE
};



class PatternPlayer
{
 public:
  PatternPlayer();
  void display_board_amp_env();
  void display_board_note();
  void display_board_bpm();
  void display_board_load_save();
  void display_board_vco();
  void display_board_lfo();
  void display_board_osc();
  void display_board_fltr();
  void display_board();

  void sub_handle_invert_trig();

  void handle_key_menu();
  void handle_key_sixteenbox();
  void handle_key_amp_env();
  void handle_key_note();
  void handle_key_osc();
  void handle_key_vco();
  void handle_key_lfo();
  void handle_key_fltr();
  void handle_key_bpm();
  void handle_key_load_save();
  void handle_key();


  void seq_update_multiple_time_by_step();
  int  seq_update_by_step();
  void seq_update_track(int t);
  int  seq();

  void load_pattern();
  int  old_main();


 protected:
  //vector <Pattern>            P(TRACK_MAX);  
  vector <Pattern>              P;
  vector <Machine   *>          M;
  vector <MonoMixer *>          MM;
  //vector <Machine   *>        M(TRACK_MAX);
  //vector <MonoMixer *>        MM(TRACK_MAX);

  Sequencer      SEQ;         // used to  store/get information about sequencer 
  AudioEngine    AE;          // used to  init alsa/rtaudio
  PatternReader  PR;          // used to  read data.pic file
  PatternElement PE;          // used for copy paste PatternElement
  InputManager   IE;          // used to  fetch key
  SDL_GUI        SG;          // used to  open a gui and display stuff
  Wave           cowbell;     // used ?
  Instrument     inst;        // used ?
  

  //int save=false;
  int save;
  //int load=false;
  int load;
  
  //int saveall=false;
  int saveall;
  //int loadall=false;
  int loadall;
  
  //int patternRemove=false;
  int patternRemove;
  
  //int bpm_current=120;    // current value for the four ( TRACK_MAX ) tracks
  int bpm_current;    // current value for the four ( TRACK_MAX ) tracks
  //int bpm=0;              // change from -10 to +10
  int bpm;              // change from -10 to +10
  //int nbcb=0;             // current nb audio callback 
  int nbcb;             // current nb audio callback 
  //int last_nbcb=0;        // number of occurence of AudioEngine callback before changing step
  int last_nbcb;        // number of occurence of AudioEngine callback before changing step
  //int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm); // Weird ?
  //int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm_current);
  int nb_cb_ch_step; //=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm_current);
  //int last_nbcb_ch_step=0;// nb audio callback from last step
  int last_nbcb_ch_step;// nb audio callback from last step
  //int debug=1;
  int debug;
  //int t=0;                // index of track
  int t;                // index of track
  


  //int repeat=0;
  int repeat;
  //int quit=0;             // do we need to quit ?
  int quit;             // do we need to quit ?
  //int cursor=0;           // cursor position in a sequencer track
  int cursor;           // cursor position in a sequencer track
  
  //int note=0;
  int note;
  //int note_all=0;
  int note_all;
  
  //int amp=0;              // variation of the amp of monomixer
  int amp;              // variation of the amp of monomixer
  //int amp_all=0;              // variation of the amp of monomixer
  int amp_all;              // variation of the amp of monomixer
  
  //int attack=0;
  int attack;
  //int release=0;
  int release;
  
  //int attack_all=0;
  int attack_all;

  //int release_all=0;
  int release_all;
  
  //int lfo_depth=0;
  int lfo_depth;

  //int lfo_depth_all=0;
  int lfo_depth_all;

  //int lfo_speed=0;
  int lfo_speed;


  //int lfo_speed_all=0;
  int lfo_speed_all;
  
  //int cutoff=0;
  int cutoff;

  //int resonance=0;
  int resonance;
  
  //int cutoff_all=0;
  int cutoff_all;

  //int resonance_all=0;
  int resonance_all;
  
  //int vcomix=0;
  int vcomix;

  //int vcomix_all=0;
  int vcomix_all;
  
  //int osconetype=0;
  int osconetype;

  //int osctwotype=0;
  int osctwotype;
  
  //int osconetype_all=0;
  int osconetype_all;


  //int osctwotype_all=0;
  int osctwotype_all;
  
  
  //int loadsave_cursor_x=0; 
  // index in the load/save menu
  int loadsave_cursor_x;  // index in the load/save menu

  //int loadsave_cursor_y=0; // index in the load/save menu
  int loadsave_cursor_y; // index in the load/save menu
  
  //int start_key=0;        // start key pressed ?
  int start_key;        // start key pressed ?
  //int step=0;             // current step in the sequencer
  //int divider=0;           // divider - => /1 /2 /4 /8  ; divider + => /8 /4 /2 /1
  int divider;           // divider - => /1 /2 /4 /8  ; divider + => /8 /4 /2 /1


  //int menu_cursor=M_AD;      // index int the menu
  int menu_cursor;      // index int the menu

  //int menu=MENU_ON_PAGE1;             // menu mode
  int menu;             // menu mode

  //int menu_note=ENABLE;
  int menu_note;


  //int menu_env=MENU_ENV_ATTACK_RELEASE;
  int menu_env;
  
  //int ct=0;               // current_track
  //int ct_x=0;             
  int ct_x;             


  //int ct_y=0;
  int ct_y;


  //int invert_trig=0;
  int invert_trig;
  
  //int dirty_graphic=1;
  int dirty_graphic;
  
  
  
  // private:
  //Pattern P;
  
};

#endif
