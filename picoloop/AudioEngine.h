using namespace std;


#include "Master.h"
//#include "Instrument.h"
//#include "Synth.h"
//#include "Wave.h"
#include <SDL/SDL.h>
#include <iostream>
#include <map>
#include "RtAudio.h"
#include "Pattern.h"
#include "PatternReader.h"
#include "Sequencer.h"
#include "InputManager.h"
#include "SDL_GUI.h"
//#include "Oscillator.h"
//#include "SineOscillator.h"
#include "AudioMixer.h"



#ifndef __AUDIOENGINE____
#define __AUDIOENGINE____


/*
  Purpose : 
  mix all channel
  implement the main callback which produce the stream 
 */

//#define BUFFER_FRAME 8192
//#define BUFFER_FRAME 512


class PatternPlayer;

class AudioEngine
{
 public:
  AudioEngine();
  void setDefault();
  void setEngineFreq(int frequency);

  int probeDevice();

  int  startAudio();
  int  stopAudio();
  int  openAudio();
  int  closeAudio();
  void setTick(int t);
  int  getTick();
  AudioMixer   & getAudioMixer();

  void setPatternPlayerObject(PatternPlayer * pp);

  //void callback();
  //  void sdl_callback();
  //void  sdl_audio_callback(void *user_data, Uint8 *audio, int length);
  int callback(void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *data );
  //void set_instrument(Instrument inst);
  void setSynthFreq(int sfreq);
  int setNbTickBeforeStepChange(int val);
  int  getNbCallback();

  int bufferIsGenerated();
  void processBuffer();
  Sint16 * getBufferOut();

 private:
  int          bpm;
  int          freq;
  int          samples;
  int          channels;
  int          polyphony;

  int          defaultFreq;
  int          defaultSamples;
  int          defaultChannels;
  int          defaultPolyphony;
  unsigned int bufferFrames;

  int          FORMAT;
  int          tick;
  int          nb_tick;
  int          nb_tick_before_step_change;
  //Instrument   inst;
  //  SineOscillator S;
  PatternPlayer * PP;
  AudioMixer   AM;
  int          nbCallback;
  FILE       * fd;
  int          debug_audio;
  Sint16     * buffer_out;
  RtAudio      dac;
  int          bufferGenerated;
  RtAudio::StreamParameters rtAudioOutputParams;
  RtAudio::StreamOptions    rtAudioStreamOptions;

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

  int  seq_callback_update_step();
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
  //Wave           cowbell;     // used ?
  //Instrument     inst;        // used ?
  

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

  int nb_tick_before_step_change;
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
