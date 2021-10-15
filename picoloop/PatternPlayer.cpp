using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <mutex>

#include "Master.h"
//#include "Note.h"
#include "PatternReader.h"
#include "PatternPlayer.h"
#include "AudioEngine.h"
//#include "Wave.h"
#include "MonoMixer.h"
#include "SDL_GUI.h"
#include "InputManager.h"
#include "Machine.h"
#include "MachineHelper.h"
#include "Sequencer.h"

#include "Generator.h"
#include "WaveTableManager.h"
#include "WaveTable.h"

#include "NoteFreq.h"
#include "TweakableKnob.h"
#include "MachineCheck.h"
#include "UserInterface.h"
#ifdef __LINUX__
#include <sched.h>
#endif
#ifdef ENABLE_SYNTH_PICOSYNTH
#include "Machine/Picosynth/PicosynthUserInterface.h"
#endif

#ifdef ENABLE_SYNTH_PICODRUM
#include "Machine/Picodrum/PicodrumUserInterface.h"
#endif
#ifdef ENABLE_SYNTH_OPL2
#include "Machine/Dbopl/DboplUserInterface.h"
#endif

#ifdef ENABLE_SYNTH_PBSYNTH
#include "Machine/PBSynth/PBSynthUserInterface.h"
#endif

#ifdef ENABLE_SYNTH_LGPTSAMPLER
#include "Machine/Lgptsampler/LgptsamplerUserInterface.h"
#include "Machine/Lgptsampler/DEBSystem.h"
#endif

#ifdef ENABLE_SYNTH_CURSYNTH
#include "Machine/Cursynth/CursynthUserInterface.h"
#endif

#ifdef ENABLE_SYNTH_OPEN303
#include "Machine/Open303/Open303UserInterface.h"
#endif

#ifdef ENABLE_SYNTH_TWYTCHSYNTH
#include "Machine/Twytch/TwytchsynthUserInterface.h"
#endif

#ifdef ENABLE_SYNTH_MDADRUM
#include "Machine/MDADrum/MDADrumUserInterface.h"
#endif

#ifdef ENABLE_SYNTH_SIDSYNTH
#include "Machine/SIDSynth/SIDSynthUserInterface.h"
#endif

#ifdef ENABLE_SYNTH_MIDIOUT
#include "Machine/MidiOutSystem/MidiOutUserInterface.h"
#endif

#ifdef __RTMIDI__
#include "MidiOutSystem.h"
#include "MidiInSystem.h"
#endif

#include "SYSTEM.h"


#ifdef PSVITA
#include <psp2/kernel/processmgr.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <psp2/ctrl.h>
#include <stdio.h>
#include <psp2/display.h>
#include <psp2/kernel/processmgr.h>
#include <stdio.h>
#include "psp2shell.h"
#endif

#ifdef PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <psppower.h>
#include "psp_callback.h"

/* 
   Padding for PSP
   if it is not correctly padded ( I don't know what is correct or not ) 
   The EBOOT.BPB binary won't load with an error message at launch
   The error code is weird like this one : 80010002
*/
//char pada='a';
//char padb='b';
//char padc='c';
//char padd='d';
//char pade='e';
//char padf='f';

#define VERS    1 //Talk about this
#define REVS    0



PSP_MODULE_INFO("PatternPlayer", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-2048) ;
//PSP_HEAP_SIZE_KB(-4096) ;
#endif // PSP

#if defined(__LINUX__) && !defined(OPENDINGUX)
#include <execinfo.h>
#include <signal.h>
#endif


#define MENU_CONFIG_Y_PALETTE            0
#define MENU_CONFIG_Y_BANK               1
#define MENU_CONFIG_Y_AUDIOPULSECLOCKOUT 2
#define MENU_CONFIG_Y_AUDIOOUTPUT        3
#define MENU_CONFIG_Y_MIDIOUTPUT         4
#define MENU_CONFIG_Y_MIDIINPUT          5
#define MENU_CONFIG_Y_MIDISYNCINOUT      6


#if defined(__RTMIDI__)
#define MENU_CONFIG_Y_MAX           6
#else
#define MENU_CONFIG_Y_MAX           3
#endif


class Cursor
{
public:
  int x;
  int y;
  Cursor() { x=0; y=0; }
};


class Menu
{
public:
  Menu();
  ~Menu();

  void next();         // Env  => Note
  void previous();     // Note => Env

  void nextPage();     // Env  => L/S
  void previousPage(); // L/S  => Env

  void nextEntry();    // Env.AR => Env.DS
  void previousEntry();// Env.DS => Env.AR

  int   menu_cursor_x; // store the 0-5,6-11 menu index
  int   menu_cursor_y; // store the value of the submenu

  int   menu_page;     // menu_cursor_x=0-5  => page1
                       // menu_cursor_x=6-11 => page2
                       // menu_cursor_x=-1   => NO_PAGE  // TODO CLEANUP
};


UserInterface * UI;
#ifdef ENABLE_SYNTH_PICOSYNTH
PicosynthUserInterface   PSUI;
#endif

#ifdef ENABLE_SYNTH_PICODRUM
PicodrumUserInterface    PDUI;
#endif

#ifdef ENABLE_SYNTH_OPL2
DboplUserInterface       DBUI;
#endif

#ifdef ENABLE_SYNTH_PBSYNTH
PBSynthUserInterface     PBUI;
#endif

#ifdef ENABLE_SYNTH_LGPTSAMPLER
LgptsamplerUserInterface LGUI;
#endif

#ifdef ENABLE_SYNTH_CURSYNTH
CursynthUserInterface    CSUI;
#endif

#ifdef ENABLE_SYNTH_OPEN303
Open303UserInterface     O303UI;
#endif

#ifdef ENABLE_SYNTH_TWYTCHSYNTH
TwytchsynthUserInterface TWUI;
#endif

#ifdef ENABLE_SYNTH_MDADRUM
MDADrumUserInterface     MDUI;
#endif

#ifdef ENABLE_SYNTH_SIDSYNTH
SIDSynthUserInterface    SSUI;
#endif

#ifdef ENABLE_SYNTH_MIDIOUT
MidiOutUserInterface MIDIUI;
#endif

#ifdef ENABLE_SYNTH_LGPTSAMPLER
// Begin used only by LGPT Sampler
DEBSystem DEB;
int argc;
char ** argv;
// End used only by LGPT Sampler
#endif

std::mutex lock_audiocallback;

//UI.handle_key(1,1);
//UI.a=1;
//UI=PUI;
//PUI.a=1;

//int32_t pal[0x0AF0FE,0xAECD15,0x0E4C15,0x46DC65,0x1515CD,0x242C45,0x442233];

#define MAX_PALETTE 5

// DISABLEDBOX_COLOR, ENABLEDBOX_COLOR, TRIG_COLOR, NOTE_COLOR, CURSOR_COLOR, STEP_COLOR, SMALLBOX_COLOR, TEXTCOLOR, BACKGROUND

// black, green, pink:
int32_t pal0[MAXCOLOR]={0x0,      0xC8A0A8, 0x59AADA, 0xFFA11C, 0x39CE90, 0x36B82B, 0x3E789A, 0xFFFFFF, 0x0 };
// blue:
int32_t pal1[MAXCOLOR]={0x3088B0, 0xB7E4F8, 0x236481, 0x184D63, 0xE8D100, 0x3DBFF4, 0xFF8000, 0xFFFFFF, 0x3088B0 };
// automn:
int32_t pal2[MAXCOLOR]={0x633A17, 0x55310E, 0x925518, 0xCF7B30, 0xE16700, 0xC00000, 0xFAA46A, 0xEEEEEE, 0x633A17 };
// grey:
int32_t pal3[MAXCOLOR]={0x777777, 0xDDDDDD, 0x444444, 0x222222, 0x555555, 0x777777, 0x888888, 0xEEEEEE, 0x777777 };
// grey nanoloop:
int32_t pal4[MAXCOLOR]={0xE3E7E3, 0xA8A8A8, 0x969696, 0xA4A4A4, 0xFFFFFF, 0x767676, 0x222222, 0x222222, 0xE3E7E3 };
//// old grey nanoloop:
//// int32_t pal4[MAXCOLOR]={0x882288, 0xA8A8A8, 0x969696, 0xA4A4A4, 0xC9C9C9, 0x767676, 0x222222, 0x222222, 0xE3E7E3 };
// yellow gameboy nanoloop:
int32_t pal5[MAXCOLOR]={0xF6F6C5, 0xABA75E, 0x969266, 0xAEAD70, 0xFFFFDD, 0x7E7D49, 0x282A20, 0x282A20, 0xF6F6C5 };

int32_t *pal=pal0;
int      menu_config_palette=0;

// highlight the loaded and saved active track 
// load_save_highligth_current[0..t-1]=-1 by default
// load_save_highligth_current[0..t-1]>=0 active played track 
int load_save_highligth_current[TRACK_MAX];

//SDL_Thread *SDL_CreateThread(int (*fn)(void *), void *data);

SDL_Thread *thread_midiclock_send = NULL;

vector <Pattern>            P(TRACK_MAX);  
vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);
vector <Effect    *>        FX(TRACK_MAX);

int debugcounter=0;               // general purpose debug counter
int config_key_pressed=0;           // > 1, a key was pressed,
int config_first_time=1;           // > 1, if the config was not launched

MonoMixer                   SAMM; // Standalone MonoMixer 
                                  //  used to get   a standalone Machine SAM
                                  //  used to check the boundary of value of different element
                                  //  it is used in seq_update_multiple_time_by_step()

Machine  *                   SAM; // Standalone Machine
                                  //  used in seq_update_multiple_time_by_step()

MachineCheck                 MC;  // Allow to switch to the next,previous machine
                                  //  used in seq_update_multiple_time_by_step()


Sequencer      SEQ;         // used to  store/get information about sequencer 
AudioEngine    AE;          // used to  init alsa/rtaudio
AudioMixer   * AM;
PatternReader  PR;          // used to  read data.pic file
Pattern        PA;          // used for copy paste Pattern
PatternElement PE;          // used for copy paste PatternElement
PatternElement PECursor;    // used for storing the current PatternElement ( under the cursor )
InputManager   IE;          // used to  fetch key
SDL_GUI        SG;          // used to  open a gui and display stuff
TweakableKnob  TK;          // used to  store "variation" which will be applied on Pattern/PatternElement object
//NoteFreq       NF;
//Wave           cowbell;     // used ?
//Instrument     inst;        // used ?

//int running;

int save=false;
int load=false;

int saveall=false;
int loadall=false;

int patternRemove=false;
int config_loaded=0;

//int bpm_current=120;    // current value for the four ( TRACK_MAX ) tracks
float bpm_current=120.0;  // current value for the four ( TRACK_MAX ) tracks
int   bpm_lock=1;         // if (bpm_lock) can not change bpm by load

int   bpm_sync_need_refresh=0; // 0 default trig a refresh of bpm
int   bpm_sync_manual=0;       // 0 default mode, 1 we are trying to sync manually
float bpm_sync_inc=0;          // use to inc/dec a bit the bpm so -1.2<bpm_sync<1.2 SELECT+LEFT|RIGHT
float bpm_current_sync_saved=0;   // use to store the bpm_current whenbpm_sync_manual == 1

//int nbcb=0;             // current nb audio callback 
//int last_nbcb=0;        // number of occurence of AudioEngine callback before changing step
//int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm); // Weird ?
//int nb_cb_ch_step;              //=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm_current);

int nb_tick_before_step_change;     //=(60*DEFAULT_FREQ)/(bpm_current*4);

const char * str_midi_clock_mode[]   = {"Internal","SyncOut","SyncIn"};
const char * str_audio_pulse_clock[] = {"NoPulse","LeftChannel","RightChannel"};


int counter_send_midi_clock=0;      // send n clock and decrement the counter each time 
int counter_send_midi_clock_six=0;  // send n clock and decrement the counter each time

int counter_recv_midi_clock=0;      // recv n clock and decrement the counter each time 
int counter_recv_midi_clock_six=0;  // recv n clock and decrement the counter each time

int counter_delta_midi_clock=0;
int delta_midi_clock=0;

int mmc_stop=0;                     // we have received a mmc stop
int mmc_start=0;                    // we have received a mmc stop
int mmc_continue=0;                 // we have received a mmc stop
int sequencer_playing=1;            // the sequencer is currently playing,
                                    //  so we move from step 0 to 1, 1 to 2 ...

long long clock_interval;           // interval in ns before two midiclock
                                    // clock_interval = 60000000000/(initialBpm*24);
                                    // midicloro.cpp:185
long long clock_delta;              // time remaining between two midi clock pulse

//int last_nbcb_ch_step=0;// nb audio callback from last step
int debug=1;
//int t=0;                // index of track



int repeat=0;
int quit=0;             // do we need to quit ?

int cursor=0;           // user cursor position in the display
                        // from 0 to 15
                        // handled by handle_key_sixteenbox

int pattern_display_offset[TRACK_MAX]={0}; // 0,16,32,48.. 
                                           // offset to display the sequencer
                                           // cursor+pattern_display_offset[cty] is used to
                                           // know where data had to be read/write

int pattern_cursor_max_pos[TRACK_MAX]={15};// between 0-15
                                           // this value is the max the 'cursor' could be
                                           // if the pattern 0 is 12 step long,
                                           // pattern_cursor_max_pos[0]=11
                                           // if the pattern 1 is 18 step long ( 16 + 2 )
                                           // pattern_cursor_max_pos[1]=1


//int loadsave_cursor_x=0; // index in the load/save menu
//int loadsave_cursor_y=0; // index in the load/save menu
Cursor loadsave_cursor;
Cursor song_cursor;

//int song_cursor_x=0; // index in the song menu
//int song_cursor_y=0; // index in the song menu

int loadsave_cursor_mode=0; // 0 loadsave mode cursor // 1 song mode cursor
int pattern_song_inc=0;     // increment/decrement the current value of song_cursor_x/song_cursor_y
int pattern_song_reload=0;  // if > 1 reload from current position
int pattern_song_loop=0;    // if > 1 loop from current song position to loop point

int song_need_saving=0;     // if >=1 the song file need to be stored on disk

int bank=0;
int bank_inc=0;
int bank_to_load=0;

int playing_with_menu_sub=0;  // (B+LEFT) or (B+RIGHT) & menu!=MENU_OFF allow to change sub_menu
int playing_with_mmc=0;       // Used to restart the sequencer in [BPM] menu

/*
  Variable use in the "configuration menu"
 */
int   menu_config_bank=0;           // will choose the bank to load at startup
int   menu_config_audioOutput=0;    // audioOutputNumber define in the config menu
int   menu_config_midiOutput=0;     //  midiOutputNumber define in the config menu
int   menu_config_midiInput=0;      //  midiInputNumber  define in the config menu
int   menu_config_midiClockMode=0;  // 0 internal, 1 midi sync out, 2 midi sync in 
int   menu_config_audiopulseclock_out=0; // 0 no sync, 1 sync left, 2 sync right
int   menu_config_y=0;              // current selected item in menu_config

/*
  Variable of the standard menu
 */

int menu_cursor=GLOBALMENU_AD;      // index int the menu
int menu=MENU_ON_PAGE1;             // menu MENU_ON_PAGE1,MENU_ON_PAGE2,MENU_OFF
int menu_note=ENABLE;
int menu_sub=0;
/* BEGIN this next one should be removed */
// int menu_ad=MENU_AD_AMP_ATTACK_RELEASE;
// int menu_osc=MENU_OSC_OSC1OSC2;
// int menu_vco=MENU_VCO_OSCMIX_PHASE;
// int menu_lfo=MENU_LFO_LFOPITCH;
// int menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;
/* END this next one should be removed */

int menu_fx=MENU_FX_DEPTH_SPEED;
int menu_ls=MENU_LS_PATTERN;

int menu_bpm=MENU_PAGE0_SUB0;
int menu_ad_dirty_keyboard=0;



int dirty_graphic=1;           // 1 : the UI need to be updated

int seq_update_by_step_next=0; // 0 : the UI audio and seq are sync
                               // 1 : the audio callback increment the variable
                               //   : the sequencer is notified by this shared variable,
                               //   : other stuff need to be updated 
                               //   : seq_update_by_step(), load,save...

int current_swing=50;  

int first_clock=1;

int          nb_tick_before_midi_send_clock;     // number of audio sample between each midi sync clock
int          nb_tick_before_six_midi_send_clock; // number of audio sample between six  midi sync clock
int          nb_tick_midi_send_clock;            // counter between each     clock increment by audio sample
int          nb_tick_midi_send_clock_mulsix;     // counter between each six clock increment by audio sample
int          midi_tick_number;


struct timeval timev_now,timev_prev,timev_lastclock;
time_t difft=0;

int opendebugprintf=0;
FILE * fdebugprintf=NULL;


void seq_update_track(int t);
void write_to_file(std::vector<std::vector<int> >& vij, std::string fname);
void read_from_file(std::vector<std::vector<int> >& vij, std::string fname);

void read_from_file(std::vector<std::vector<int> >& vij, std::string fname){
  FILE* fp = fopen(fname.c_str(), "r");
  int dim1, dim2;
  fscanf(fp, "%d,%d\n", &dim1, &dim2);
  //printf("dims: %d %d\n", dim1, dim2);
  if(dim1 != (int)vij.size()){
    vij.resize(dim1);
  }
  for(int i = 0; i < dim1; ++i){
    if(dim2 != (int)vij[i].size()){
      vij[i].resize(dim2);
    }
    for(int j = 0; j < dim2 - 1; ++j){
      fscanf(fp,"%d,", &vij[i][j]);
    }
    fscanf(fp,"%d\n", &vij[i][dim2-1]);
  }
  fclose(fp);
}
void write_to_file(std::vector<std::vector<int> >& vij, std::string fname){
  FILE* fp = fopen(fname.c_str(), "w");
  int dim1 = vij.size();
  int dim2 = 0;
  if(dim1>0){
    dim2 = vij[0].size();  
  }
  fprintf(fp, "%d,%d\n",dim1, dim2);
  for(int i=0; i < dim1; ++i){
    for(int j =0; j < dim2-1; ++j){
      fprintf(fp, "%d,", vij[i][j]);
    }
    fprintf(fp, "%d\n", vij[i][dim2-1]);
    //fprintf(fp, "\n");
  }
  fclose(fp);
}
int handle_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all);


#if defined(__LINUX__) && !defined(OPENDINGUX)
void handler(int sig)
{
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}
#endif


void processBuffer_updateMidiSendClockCounter()
{
  if (menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCOUT)
    {
      nb_tick_midi_send_clock++;
      nb_tick_midi_send_clock_mulsix++;

      // arm a counter to send the six midi sync signal
      // there is a / 6 and it prevent midi clock skew
      if (nb_tick_midi_send_clock_mulsix>nb_tick_before_six_midi_send_clock-1)
       {
         counter_send_midi_clock_six++;

         // This trick allow to transpose the send of the midi clock
         // it can be used on a system which buffer too much audio
         // it is linked to the BPM menu
         nb_tick_midi_send_clock=-counter_delta_midi_clock*50;
         nb_tick_midi_send_clock_mulsix=-counter_delta_midi_clock*50;

         midi_tick_number=0;
         counter_delta_midi_clock=0;   
       }

      // arm a counter to send a midi sync signal
      if (midi_tick_number<5 &&
         nb_tick_midi_send_clock>nb_tick_before_midi_send_clock-1)
       {
         counter_send_midi_clock++;
         nb_tick_midi_send_clock=0;
         midi_tick_number++;
       }
    }
}



long difftime(struct timeval & a0, struct timeval & a1)
{
  
  gettimeofday(&a0, NULL);
  return ((a0.tv_sec * 1000000 + a0.tv_usec) -
	  (a1.tv_sec * 1000000 + a1.tv_usec));

}

#ifdef __RTMIDI__
void seq_send_midiclock()
{

  MidiOutSystem & MOS=MidiOutSystem::getInstance();
  if (counter_send_midi_clock)
    {
      MOS.clock();
      counter_send_midi_clock--;
      DPRINTF("*****MIDICLOCK   *****");
    }

}

void seq_send_midiclock_six()
{
  MidiOutSystem & MOS=MidiOutSystem::getInstance();
  if (counter_send_midi_clock_six)
    {
      MOS.clock();
      counter_send_midi_clock_six--;
      DPRINTF("*****MIDICLOCK_6 *****");
    }
}

int thread_seq_send_midiclock( void * data)
{
  while(IE.shouldExit()!=1)
    {
      seq_send_midiclock();
      seq_send_midiclock_six();

      SDL_Delay(1);  
    }
  return 0;
}
#endif



void seq_send_midiclock_old()
{

  // clock_interval                    = 20 833 334 for 120 BPM
  // clock delta    should be maxed at       21 000
  
  if (first_clock)
    {
      gettimeofday(&timev_prev, NULL);
      gettimeofday(&timev_lastclock, NULL);
      first_clock=0;
      clock_delta=0;
    }
  gettimeofday(&timev_now,NULL);


  //printf("********** %.15lld %.15lld %.15lld\n", 	 difftime(timev_now,timev_lastclock), 	 clock_interval, 	 clock_delta);

  if ((difftime(timev_now,timev_lastclock)+clock_delta)*1000>clock_interval)
    {
#ifdef __RTMIDI__
      DPRINTF("*****MIDICLOCK   *****");

      MidiOutSystem & MOS=MidiOutSystem::getInstance();
      MOS.clock();
#endif
      
      clock_delta=clock_delta+(difftime(timev_now,timev_lastclock))-clock_interval/1000;
      timev_lastclock=timev_now;
    }
  
}


void display_refresh()
{
  SG.refresh();
}


void disable_load_save_highlight()
{
  int t;
  for (t=0;t<TRACK_MAX;t++)
    load_save_highligth_current[t]=-1;    
}


void refresh_pecursor_ui(int i);
//int noteOffTrigger[TRACK_MAX];

// SAMM is an AudioMixer which give access to a "Machine" pointer
void update_SAMM(int trackNumber,int stepNumber)
{
  int machine_type=P[trackNumber].getPatternElement(stepNumber).get(MACHINE_TYPE);
  if (!MC.machineExist(machine_type))
    {
      machine_type=MC.getNext(machine_type);
      P[trackNumber].getPatternElement(stepNumber).set(MACHINE_TYPE,machine_type);
    }
  SAMM.setMachineType(machine_type);	  
  refresh_pecursor_ui(stepNumber);
  SAM=SAMM.getInput();
}


void load_pattern();

void refresh_swing()
{
  // Swing
  // 0    => 50/50
  // 64   => 66/66
  // 127  => 75/25

  long long swing_sevenbit=(current_swing*128)/100;
  long long swing_inv_sevenbit=128-swing_sevenbit;
  long long step=SEQ.getPatternSequencer(0).getStepWithoutDivider();
  long long nb_sample=-1;
  long long nb_sample_even;
  long long nb_sample_odd;

  // Try to calculate even and odd number of sample for each step without loosing one sample
  // we may lose one sample easily with int/long number with * / arithmetic operator
  // for example : 15*44100/120=5512.5 which is 5512 in int
  nb_sample_even=(nb_tick_before_step_change*swing_sevenbit)/64;
  if (swing_sevenbit==64)
    nb_sample_odd=nb_sample_even;
  if (swing_sevenbit>64)
    nb_sample_odd=nb_tick_before_step_change-(nb_sample_even-nb_tick_before_step_change);
  if (swing_sevenbit<64)
    nb_sample_odd=nb_tick_before_step_change+(nb_tick_before_step_change-nb_sample_even);

  //nb_sample_odd=nb_tick_before_step_change-nb_sample_even;
  
  if (step%2)
    nb_sample=nb_sample_even;
  else
    nb_sample=nb_sample_odd;
  
  AE.setNbTickBeforeStepChange(nb_sample);
  nb_tick_before_midi_send_clock=nb_sample/6;
  nb_tick_before_six_midi_send_clock=nb_sample;

}

void refresh_bpm()
{
  float nb_tick_before_step_change_f;
  float clock_interval_f;
  //nb_tick_before_step_change_f=(60.0*DEFAULTFREQ)/(bpm_current*4.0);
  // seem to be not needed, it's me that does not
  // understand how LGPT calculate tempo
#ifndef LGPTTEMPO   
  nb_tick_before_step_change_f=(15.0*DEFAULTFREQ)/bpm_current;
  nb_tick_before_step_change=nb_tick_before_step_change_f;
#else
  nb_tick_before_step_change_f=(15.0*DEFAULTFREQ)/bpm_current;
  nb_tick_before_step_change=nb_tick_before_step_change_f;
  nb_tick_before_step_change=nb_tick_before_step_change/6;
  nb_tick_before_step_change=nb_tick_before_step_change*6;
#endif
  refresh_swing();
  //printf("%d %f\n",nb_tick_before_step_change,nb_tick_before_step_change_f);
}


// handle the :
//  - fix of the cursor,display_offset[] linked to a load of a new pattern
//  - fix the pattern_cursor_max_pos[] of the current track to be
//    able to move only on available step

void refresh_cursor_display_offset_cursor_max_pos(int track)
{
  int  cty=SEQ.getCurrentTrackY();
  int plen=P[track].getSize();
  
  if (pattern_display_offset[track]>(plen/16)*16)
    pattern_display_offset[track]=0;
  
  if (cursor+pattern_display_offset[track]>=P[track].getSize())
    cursor=0;
  
  if (cursor>pattern_cursor_max_pos[track])
    cursor=0;

  if (pattern_cursor_max_pos[track]>15)
    pattern_cursor_max_pos[track]=15;

}

// handle the :
//  - init the lenght of the pattern and setup the lenght of the sequencer
//  - fix the pattern_cursor_max_pos[] of the current track to be
//    able to move only on available step
void init_cursor_display_offset_cursor_max_pos(int track)
{
  int plen=P[track].getSize();
  
  SEQ.getPatternSequencer(track).setPatternLength(plen);
  pattern_cursor_max_pos[track]=plen-1-pattern_display_offset[track];
  if (pattern_cursor_max_pos[track]>15)
    pattern_cursor_max_pos[track]=15;
}



//char * tmp_str;

void display_board_trig()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // User Cursor postion      
  SG.drawBoxNumber(cursor,pal[CURSOR_COLOR]);

  // Draw the sequencer position
  if (step/16==pattern_display_offset[cty]/16)
    SG.drawBoxNumber(step-pattern_display_offset[cty],pal[STEP_COLOR]);  

  //for (i=0;i<16;i++)
    for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
    {	  
      // Draw trigged box trig color   
      if (P[cty].getPatternElement(i+pattern_display_offset[cty]).get(NOTE_ON))
	{
	  // Display note which will be trigged
	  SG.drawBoxNumber(i,pal[TRIG_COLOR]);

	  // Display the user cursor position on top of the trig
	  if (cursor==i)
	    SG.drawBoxNumber(cursor,pal[CURSOR_COLOR]);
	  
	  // Display the sequencer position on top of the trig
	  if (i+pattern_display_offset[cty]==step)
	    SG.drawBoxNumber(step-pattern_display_offset[cty],pal[STEP_COLOR]);  
	}
    }  
}

void display_board_two_param(int machineParam1,int machineParam2)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  display_board_trig();

  //for (i=0;i<16;i++)
  for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
    {	  // Draw trigged box trig color   
      if (P[cty].getPatternElement(i+pattern_display_offset[cty]).get(NOTE_ON))
	{
	  SG.smallBoxNumber(i,P[cty].getPatternElement(i+pattern_display_offset[cty]).get(machineParam1),128,pal[SMALLBOX_COLOR]);
	  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i+pattern_display_offset[cty]).get(machineParam2),pal[SMALLBOX_COLOR]);
	}
    }  
}


void display_board_one_param_text(int machineParam1)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  PatternElement PET;

  const char * space="    ";

  int  size_of_zero=5;
  char line1[size_of_zero+1];

  const char * line1_to_process;
  int    line1_size;

  display_board_trig();

  //for (i=0;i<16;i++)
  for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
    {	  
      // Draw trigged box trig color   
      PET=P[cty].getPatternElement(i+pattern_display_offset[cty]);
      
      if (PET.get(NOTE_ON))
	{
	  update_SAMM(cty,i+pattern_display_offset[cty]);
	  strcpy(line1,space);
	  line1_to_process=SAM->getMachineParamCharStar(machineParam1,PET.get(machineParam1));
	  line1_size=strlen(line1_to_process);
	  
	  if (line1_size>size_of_zero)
	    strncpy(line1,line1_to_process,size_of_zero);
	  else
	    strncpy(line1,line1_to_process,line1_size);
	  line1[size_of_zero]='\0';

	  SG.drawTTFTextNumberFirstLine(i,line1);	  
	}
    }  
}


void display_board_two_param_text(int machineParam1,int machineParam2)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  const char * space="     ";

  int  size_of_zero=5;
  char line1[size_of_zero+1];
  char line2[size_of_zero+1];

  const char * line1_to_process;
  const char * line2_to_process;
  int    line1_size;
  int    line2_size;

  display_board_trig();

  //for (i=0;i<16;i++)
    for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
    {	  // Draw text on Trigged step
      if (P[cty].getPatternElement(i+pattern_display_offset[cty]).get(NOTE_ON))
	{
	  strcpy(line1,space);	  
	  strcpy(line2,space);	  
	  update_SAMM(cty,i+pattern_display_offset[cty]);
	  strcpy(line1,space);
	  line1_to_process=SAM->getMachineParamCharStar(machineParam1,P[cty].getPatternElement(i+pattern_display_offset[cty]).get(machineParam1));
	  line2_to_process=SAM->getMachineParamCharStar(machineParam2,P[cty].getPatternElement(i+pattern_display_offset[cty]).get(machineParam2));
	  
	  line1_size=strlen(line1_to_process);
	  line2_size=strlen(line2_to_process);
	  
	  if (line1_size>size_of_zero)
	    strncpy(line1,line1_to_process,size_of_zero);
	  else
	    strcpy(line1,line1_to_process);
	  
	  if (line2_size>size_of_zero)
	    strncpy(line2,line2_to_process,size_of_zero);
	  else
	    strcpy(line2,line2_to_process);	  

	  SG.drawTTFTextNumberFirstLine( i,line1);	  
	  SG.drawTTFTextNumberSecondLine(i,line2);

	}
    }  
}



void display_board_two_param_number(int machineParam1,int machineParam2)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  const char * space="    ";

  int  size_of_zero=5;
  char line1[size_of_zero+1];
  char line2[size_of_zero+1];

  char * line1_to_process;
  char * line2_to_process;
  int    line1_size;
  int    line2_size;

  int    param1;
  int    param2;

  display_board_trig();

  //for (i=0;i<16;i++)
  for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
    {	  // Draw text on Trigged step
      if (P[cty].getPatternElement(i+pattern_display_offset[cty]).get(NOTE_ON))
	{
	  update_SAMM(cty,i+pattern_display_offset[cty]);
	  strcpy(line1,space);
	  param1=P[cty].getPatternElement(i+pattern_display_offset[cty]).get(machineParam1);
	  param2=P[cty].getPatternElement(i+pattern_display_offset[cty]).get(machineParam2);

	  
	  sprintf(line1,"  %d",param1);
	  sprintf(line2,"  %d",param2);

	  SG.drawTTFTextNumberFirstLine( i,line1);	  
	  SG.drawTTFTextNumberSecondLine(i,line2);

	}
    }  
}

void display_board_one_param_number(int machineParam1)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  const char * space="    ";

  int  size_of_zero=5;
  char line1[size_of_zero+1];
  char line2[size_of_zero+1];

  char * line1_to_process;
  char * line2_to_process;
  int    line1_size;
  int    line2_size;

  int    param1;
  int    param2;

  display_board_trig();

  //for (i=0;i<16;i++)
  for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
    {	  // Draw text on Trigged step
      if (P[cty].getPatternElement(i+pattern_display_offset[cty]).get(NOTE_ON))
	{
	  update_SAMM(cty,i+pattern_display_offset[cty]);
	  strcpy(line1,space);
	  param1=P[cty].getPatternElement(i+pattern_display_offset[cty]).get(machineParam1);

	  
	  sprintf(line1,"  %d",param1);

	  SG.drawTTFTextNumberFirstLine( i,line1);	  

	}
    }  
}



// Used by MDA to display 
// the bank name and the waveform name in the bank
void display_board_one_and_two_param_text(int machineParam1,int machineParam2)
{
  int  i;
  int  j;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  const char * space="     ";
  int    size_of_zero=5;
  char   line1[128];
  char   line2[128];

  const char * line1_to_process;
  const char * line2_to_process;
  int    line1_size;
  int    line2_size;

  int  right_x_display_offset=       COLLUMN40;
  int  right_y_display_offset_line7= LINE08;
  int  right_y_display_offset_line8= LINE09;

  
  display_board_trig();

  //for (i=0;i<16;i++)
  for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
    {	  // Draw trigged box trig color   
      j=i+pattern_display_offset[cty];
      if (P[cty].getPatternElement(j).get(NOTE_ON))
	{
	  update_SAMM(cty,j);
	  
	  line1_to_process=SAM->getMachineParamCharStar(machineParam1,
							P[cty].getPatternElement(j).get(machineParam1));
      
	  line2_to_process=SAM->getMachineTwoParamCharStar(machineParam2,
						       P[cty].getPatternElement(j).get(machineParam1),
						       P[cty].getPatternElement(j).get(machineParam2));
	  // if (line1_to_process==NULL | line2_to_process==NULL)
	  //   break;
	  line1_size=strlen(line1_to_process);
	  line2_size=strlen(line2_to_process);

	  if (line1_size>size_of_zero)
	    {
	      strncpy(line1,line1_to_process,line1_size);
	      line1[size_of_zero]='\0';
	    }
	  else
	    strcpy(line1,line1_to_process);

	  if (line2_size>size_of_zero)
	    {
	      strncpy(line2,line2_to_process,line2_size);
	      line2[size_of_zero]='\0';
	    }
	  else
	    strcpy(line2,line2_to_process);

	  SG.drawTTFTextNumberFirstLine( i,line1);	  
	  SG.drawTTFTextNumberSecondLine(i,line2);

	  // display two additional line of text in the right of the screen
	  // the display is "cursor position" dependant
	  if (cursor==i)
	    {
	      SG.guiTTFText(right_x_display_offset,
			    right_y_display_offset_line7,line1_to_process);
	      SG.guiTTFText(right_x_display_offset,
			    right_y_display_offset_line8,line2_to_process);
	    }
	  
	}
    }  
}

void display_board_amp_env()
{
  UI->display_board(GLOBALMENU_AD);
}
/*
void display_board_note()
{
  UI->display_board(GLOBALMENU_NOTE);
}
*/
void display_board_note()
{
  int  i;
  int  j;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  NoteFreq & NF = NoteFreq::getInstance();

  display_board_trig();


  // Note C3 
  if (menu_note==ENABLE)
    {	  
      //for (i=0;i<16;i++)
      for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
	{
	  j=i+pattern_display_offset[cty];
	  
	  if (P[cty].getPatternElement(j).get(NOTE_ON))
	    SG.drawTTFTextNumberFirstLine(i,NF.getNoteCharStar(P[cty].getPatternElement(j).get(NOTE1)));
	}
    }
  // Note Cursor
  if (menu_note==DISABLE)      
    {
      for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
	{
	  j=i+pattern_display_offset[cty];
	  
	  if (P[cty].getPatternElement(j).get(NOTE_ON))
	    SG.smallBoxNumber(i,
			      (P[cty].getPatternElement(j).get(NOTE1)%12-1)*10,
			      (128-(P[cty].getPatternElement(j).get(NOTE1)/12)*10),
			      SMALLBOX_COLOR);
	}
    }
}


void display_board_vco()
{
 UI->display_board(GLOBALMENU_VCO);
}




void display_board_lfo()
{
 UI->display_board(GLOBALMENU_LFO);
}

void display_board_osc()
{
 UI->display_board(GLOBALMENU_OSC);
}

void display_board_fltr()
{
  UI->display_board(GLOBALMENU_FLTR);
}


void display_board_fx()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_FX)
    {      
      if (menu_fx==MENU_FX_DEPTH_SPEED)
	{
	  display_board_two_param(FX1_DEPTH,FX1_SPEED);
	}
    }
}




void display_board_bpm()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();


  if (menu_cursor==GLOBALMENU_BPM)
    {
      //display_board_trig();
      display_board_note();
    }
  
}

void display_board_psh()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  
  // need to display something, so why not ?
  
  if (menu_cursor==GLOBALMENU_PSH)
    {
      display_board_note();
    }                  
}





void display_board_mac()
{
  int  i;
  int  j;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  int  mt;
  display_board_trig();


    if (menu_cursor==GLOBALMENU_MAC)
    {
      //for (i=0;i<16;i++)
      for (i=0;i<pattern_cursor_max_pos[cty]+1;i++)
	{
	  j=i+pattern_display_offset[cty];
	  if (P[cty].getPatternElement(j).get(NOTE_ON))
	    {
	      //SG.drawTTFTextNumberFirstLine(i, P[cty].getPatternElement(j).getMachineTypeCharStar());
	      mt=P[cty].getPatternElement(j).get(MACHINE_TYPE);
	      SG.drawTTFTextNumberFirstLine(i, getMachineTypeCharStar(mt));
	    }
	}
    }
}

void display_config()
{
  char str_palette[128];
  char str_bank[128];
  char str_pulsesyncout[128];
  char str_audiooutput[128];
  char str_midioutput[128];
  char str_midiinput[128];
  char str_midiclockmode[128];
  char str_audiooutput_name[128];
  char str_midioutput_name[128];
  char str_midiinput_name[128];

  char str_menuconfig[128];


  
  static char * audioOutputDeviceName;
  static char * midiOutputDeviceName;
  static char * midiInputDeviceName;
  static int    audioOutputDevice;
  static int     midiOutputDevice;
  static int     midiInputDevice;


  static char   cursor_line1=' ';
  static char   cursor_line2=' ';
  static char   cursor_line3=' ';
  static char   cursor_line4=' ';
  static char    cursor_line5=' ';

  static char * test_str;
  
  static char cursor_line[8];  
  
  int i=0;

  for (i=0;i<8;i++)
    cursor_line[i]=' ';

  cursor_line[menu_config_y]='>';


  
  
  
#ifdef __RTMIDI__
  MidiOutSystem & MOS=MidiOutSystem::getInstance();
  MidiInSystem  & MIS=MidiInSystem::getInstance();
#endif
  if (config_key_pressed>=1 ||
      config_first_time)
    {
      audioOutputDeviceName=AE.getAudioOutputName(menu_config_audioOutput);
      audioOutputDevice=AE.getNumberOfAudioOutputDevice();
#ifdef __RTMIDI__
      midiOutputDeviceName=MOS.getMidiOutputName(menu_config_midiOutput);
      midiOutputDevice=MOS.getNumberOfMidiOutputDevice();      

      midiInputDeviceName=MIS.getMidiInputName(menu_config_midiInput);
      midiInputDevice=MIS.getNumberOfMidiInputDevice();      

#endif
      config_key_pressed=0;
    }

  
  //sprintf(str_menuconfig    ,"menuconfig    : %d %d"        ,menu_config_y,debugcounter++);
  sprintf(str_palette       ,"%c Current palette    : %d "       ,cursor_line[0],menu_config_palette);
  sprintf(str_bank          ,"%c Current Bank       : %d "       ,cursor_line[1],menu_config_bank);
  sprintf(str_pulsesyncout  ,"%c AudioOut PulseSync : %s "       ,cursor_line[2],str_audio_pulse_clock[menu_config_audiopulseclock_out]);
  sprintf(str_audiooutput   ,"%c AudioOutput        : %d/%d : %s",cursor_line[3],menu_config_audioOutput,audioOutputDevice,audioOutputDeviceName);
#ifdef __RTMIDI__
  sprintf(str_midioutput    ,"%c MidiOutput         : %d/%d : %s",cursor_line[4],menu_config_midiOutput,midiOutputDevice,midiOutputDeviceName);
  sprintf(str_midiinput     ,"%c MidiInput          : %d/%d : %s",cursor_line[5],menu_config_midiInput,midiInputDevice,midiInputDeviceName);
  sprintf(str_midiclockmode ,"%c MidiClockMode      : %s"        ,cursor_line[6],str_midi_clock_mode[menu_config_midiClockMode]); 

#endif
  SG.clearScreen();  
  SG.guiTTFText(COLLUMN03,LINE00,  str_palette);
  SG.guiTTFText(COLLUMN03,LINE01,  str_bank);
  SG.guiTTFText(COLLUMN03,LINE02,  str_pulsesyncout);
  SG.guiTTFText(COLLUMN03,LINE03,  str_audiooutput);

#ifdef __RTMIDI__
  SG.guiTTFText(COLLUMN03,LINE04,  str_midioutput);
  SG.guiTTFText(COLLUMN03,LINE05,  str_midiinput);
  SG.guiTTFText(COLLUMN03,LINE06, str_midiclockmode);
#endif
  display_refresh();
}

void handle_key_config()
{
  IE.handleKey();

  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

#ifdef __RTMIDI__
  MidiOutSystem & MOS=MidiOutSystem::getInstance();
  MidiInSystem  & MIS=MidiInSystem::getInstance();
#endif 

  if (lastKey   == BUTTON_A && 
      lastEvent == KEYPRESSED)
    {
      config_loaded=1;
    }

  if (lastKey   == BUTTON_DOWN && 
      lastEvent == KEYRELEASED)
    {
      menu_config_y++;
    }

  if (lastKey   == BUTTON_UP && 
      lastEvent == KEYRELEASED)
    {
      menu_config_y--;
    }

  
  if (lastKey   == BUTTON_RIGHT && 
      lastEvent == KEYRELEASED)
    {

      // palette
      if (menu_config_y==MENU_CONFIG_Y_PALETTE)
	{
	  menu_config_palette++;
	  if (menu_config_palette > MAX_PALETTE)
	    menu_config_palette=0;
	}

      // bank loading
      if (menu_config_y == MENU_CONFIG_Y_BANK)
	{
	  menu_config_bank++;
	  if (menu_config_bank>MAX_BANK-1)
	      menu_config_bank=0;
	}

      // AudioPulseClock output
      if (menu_config_y==MENU_CONFIG_Y_AUDIOPULSECLOCKOUT)
	{
	  menu_config_audiopulseclock_out++;
	  if (menu_config_audiopulseclock_out>2)
	    menu_config_audiopulseclock_out=0;
	}

      // audio output
      if (menu_config_y==MENU_CONFIG_Y_AUDIOOUTPUT)
	{
	  menu_config_audioOutput++;
	  if (menu_config_audioOutput > AE.getNumberOfAudioOutputDevice())
	    menu_config_audioOutput=0;
	}
#ifdef __RTMIDI__
      // midi output
      if (menu_config_y==MENU_CONFIG_Y_MIDIOUTPUT)
	{
	  menu_config_midiOutput++;
	  if (menu_config_midiOutput > MOS.getNumberOfMidiOutputDevice())
	    menu_config_midiOutput=0;
	}
      // midi input
      if (menu_config_y==MENU_CONFIG_Y_MIDIINPUT)
	{
	  menu_config_midiInput++;
	  if (menu_config_midiInput > MIS.getNumberOfMidiInputDevice())
	    menu_config_midiInput=0;
	}
      // midi sync
      if (menu_config_y==MENU_CONFIG_Y_MIDISYNCINOUT)
	{
	  menu_config_midiClockMode++;
	  if (menu_config_midiClockMode > MENU_CONFIG_Y_MIDICLOCK_SYNCMAX)
	    menu_config_midiClockMode=0;
	}

      
#endif
      config_key_pressed++;
    }

  if (lastKey   == BUTTON_LEFT && 
      lastEvent == KEYRELEASED)
    {

      // palette
      if (menu_config_y==MENU_CONFIG_Y_PALETTE)
	{
	  menu_config_palette--;
	  if (menu_config_palette < 0)
	    menu_config_palette=MAX_PALETTE;
	}

      // bank loading
      if (menu_config_y==MENU_CONFIG_Y_BANK)
	{
	  menu_config_bank--;
	  if (menu_config_bank<0)
	      menu_config_bank=MAX_BANK-1;
	}

      // AudioPulseClock output
      if (menu_config_y==MENU_CONFIG_Y_AUDIOPULSECLOCKOUT)
	{
	  menu_config_audiopulseclock_out--;
	  if (menu_config_audiopulseclock_out<0)
	    menu_config_audiopulseclock_out=2;
	}

      // audio output
      if (menu_config_y==MENU_CONFIG_Y_AUDIOOUTPUT)
	{
	  menu_config_audioOutput--;
	  if (menu_config_audioOutput < 0) 
	    menu_config_audioOutput=AE.getNumberOfAudioOutputDevice();
	}
#ifdef __RTMIDI__
      // midi output
      if (menu_config_y==MENU_CONFIG_Y_MIDIOUTPUT)
	{
	  menu_config_midiOutput--;
	  if (menu_config_midiOutput < 0) 

	    menu_config_midiOutput=MOS.getNumberOfMidiOutputDevice();
	}
      
      // midi input
      if (menu_config_y==MENU_CONFIG_Y_MIDIINPUT)
	{
	  menu_config_midiInput--;
	  if (menu_config_midiInput < 0) 

	    menu_config_midiInput=MIS.getNumberOfMidiInputDevice();
	}

      // midi sync
      if (menu_config_y==MENU_CONFIG_Y_MIDISYNCINOUT)
	{
	  menu_config_midiClockMode--;
	  if (menu_config_midiClockMode < 0) 
	    menu_config_midiClockMode=MENU_CONFIG_Y_MIDICLOCK_SYNCMAX;	    
	}
      
      
#endif      
      config_key_pressed++;      
    }

  // Number of configuration Element if there is Midi support or not MENU_CONFIG_Y_MAX
  //#ifdef __RTMIDI__
  if (menu_config_y>MENU_CONFIG_Y_MAX) menu_config_y=0;
  if (menu_config_y<0)                 menu_config_y=MENU_CONFIG_Y_MAX;
  /*
#else
  if (menu_config_y>2)                 menu_config_y=0;
  if (menu_config_y<0)                 menu_config_y=2;
#endif
  */

  if (IE.shouldExit())
    {
#ifdef __RTMIDI__
      MidiOutSystem & MOS=MidiOutSystem::getInstance();
      MidiInSystem & MIS=MidiInSystem::getInstance();
      MOS.closePort();
      MIS.closePort();
      exit(0);
#endif
    }
  IE.clearLastKeyEvent();

}

void handle_config()
{
  // Handler which make change to the configuration
  if (config_first_time || config_key_pressed)
    {
      DPRINTF("config_key_pressed:%d",config_key_pressed);
#ifdef __RTMIDI__
      /*
      MidiOutSystem & MOS=MidiOutSystem::getInstance();
      MOS.chooseMidiPortDeviceNumber(menu_config_midiOutput);

      MidiInSystem & MIS=MidiInSystem::getInstance();
      MIS.chooseMidiPortDeviceNumber(menu_config_midiInput);
      */
#endif 
      bank=menu_config_bank;
      AE.setAudioOutput(menu_config_audioOutput);
      config_first_time=0;

      if (menu_config_palette==0) { pal=pal0; }
      if (menu_config_palette==1) { pal=pal1; }
      if (menu_config_palette==2) { pal=pal2; }
      if (menu_config_palette==3) { pal=pal3; }
      if (menu_config_palette==4) { pal=pal4; }
      if (menu_config_palette==5) { pal=pal5; }
    }
  if (IE.shouldExit())
    exit(0);
}

void display_board_bank()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  //int  bank=PR.getBank();




  char str_bank_current[128];
  char str_bank_toload[128];
  char str_song[128];


  static const char * txt_tab[] = 
    { 
      "00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
      "10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
      "20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
      "30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
      "40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
      "50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
      "60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
      "70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
      "80","81","82","83","84","85","86","87","88","89","8A","8B","8C","8D","8E","8F",
      "90","91","92","93","94","95","96","97","98","99","9A","9B","9C","9D","9E","9F",
      "A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","AA","AB","AC","AD","AE","AF",
      "B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA","BB","BC","BD","BE","BF",
      "C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB","CC","CD","CE","CF",
      "D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC","DD","DE","DF",
      "E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED","EE","EF",
      "F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE","FF",
    }; 


  if (menu         ==  MENU_OFF        && 
      menu_cursor  ==  GLOBALMENU_BANK 
      )
    {
     DPRINTF("HIT");    
      SG.clearScreen();

      sprintf(str_bank_current,"Current Bank %d ",bank);
      sprintf(str_bank_toload,"Bank to load %d ",bank_to_load);
      
      SG.guiTTFText(COLLUMN03,LINE01, str_bank_current);
      SG.guiTTFText(COLLUMN03,LINE02, str_bank_toload);

    }

  if (menu         !=  MENU_OFF        && 
      menu_cursor  == GLOBALMENU_BANK)
    {
      display_board_trig();
    }

}


void display_board_load_save()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  int  bank=PR.getBank();

  int loadsave_cursor_x_div_sixteen=loadsave_cursor.x/16;
  int loadsave_cursor_x_divmul_sixteen=loadsave_cursor_x_div_sixteen*16;

  int song_cursor_x_div_sixteen=song_cursor.x/16;
  int song_cursor_x_divmul_sixteen=song_cursor_x_div_sixteen*16;


   char str_bank[32]  = {0};
   char str_song1[32] = {0};
   char str_song2[32] = {0};
   char str_song3[32] = {0};
  

  static const char * txt_pattern_modulo_sixteen_slot[] = 
    { 
    "  0-15 ",
    " 16-31 ",
    " 32-47 ",
    " 48-63 ",
    " 64-79 ",
    " 80-95 ",
    " 96-111",
    "112-127",
    "128-143",
    "144-159",
    "160-175",
    "176-191",
    "192-207",
    "208-223",
    "224-239",
    "240-255", 
    "256-271",
    "272-287",
    "288-303",
    "304-319",
    "320-335",
    "336-351",
    "352-367",
    "368-383",
    "384-399",
    "400-415",
    "416-431",
    "432-447",
    "448-463",
    "464-479",
    "480-495",
    "496-511",
    "512-527",
    "528-543",
    "544-559",
    "560-575",
    "576-591",
    "592-607",
    "608-623",
    "624-639",
    "640-655",
    "656-671",
    "672-687",
    "688-703",
    "704-719",
    "720-735",
    "736-751",
    "752-767",
    "768-783",
    "784-799",
    "800-815",
    "816-831",
    "832-847",
    "848-863",
    "864-879",
    "880-895",
    "896-911",
    "912-927",
    "928-943",
    "944-959",
    "960-975",
    "976-991",
    "992-1007",
    "1008-1023"
    }; 

  /*
  static const char * txt_tab[] = 
    { 
    "0","1","2","3",
    "4","5","6","7",
    "8","9","A","B",
    "C","D","E","F" 
    }; 
  */

  static const char * txt_tab[] = 
    { 
      "00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
      "10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
      "20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
      "30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
      "40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
      "50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
      "60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
      "70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
      "80","81","82","83","84","85","86","87","88","89","8A","8B","8C","8D","8E","8F",
      "90","91","92","93","94","95","96","97","98","99","9A","9B","9C","9D","9E","9F",
      "A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","AA","AB","AC","AD","AE","AF",
      "B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA","BB","BC","BD","BE","BF",
      "C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB","CC","CD","CE","CF",
      "D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC","DD","DE","DF",
      "E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED","EE","EF",
      "F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE","FF",
    }; 

  DPRINTF("display_board_load_save begin");
  if (menu         ==  MENU_OFF        && 
      menu_cursor  ==  GLOBALMENU_LS   && 
      menu_ls      ==  MENU_LS_PATTERN
      )
    {
      DPRINTF("HIT");
      SG.clearScreen();
      
      sprintf(str_bank, "Bank %d",bank);
      sprintf(str_song1,"SongPosition %d ",SEQ.getSongSequencer().getStep());
      sprintf(str_song2,"LoopA %d ",SEQ.getSongSequencer().getLoopA());
      sprintf(str_song3,"LoopB %d ",SEQ.getSongSequencer().getLoopB());

      SG.guiTTFText(COLLUMN03,LINE01, str_bank);
      //SG.guiTTFText(COLLUMN03,LINE02, txt_pattern_modulo_sixteen_slot[loadsave_cursor_x_div_sixteen]);      

      // ok
      SG.guiTTFText(COLLUMN03,LINE15, str_song1);      
      //SG.guiTTFText(COLLUMN03,LINE16, txt_pattern_modulo_sixteen_slot[song_cursor_x_div_sixteen]);
      // bad
      SG.guiTTFText(COLLUMN33,LINE16, str_song2);      
      SG.guiTTFText(COLLUMN33,LINE17, str_song3);



      // Display box loaded/unloaded for load/save mode
      for (x=loadsave_cursor_x_divmul_sixteen;
	   x<(loadsave_cursor_x_divmul_sixteen)+16;
	   x++)

	
	for (y=0;y<TRACK_MAX;y++)
	  {
	    if (PR.PatternDataExist(x,y))
	      {
		// Display if it is a loaded pattern
		if (load_save_highligth_current[y]==x)
		  SG.middleBoxNumberUp(x%16,y,pal[ENABLEDBOX_COLOR]);
		else
		  SG.middleBoxNumberUp(x%16,y,pal[NOTE_COLOR]);
	      }
	    else
	      SG.middleBoxNumberUp(x%16,y,pal[STEP_COLOR]);
	  }

      // Display box song
      for (x=0;x<16;x++)
	for (y=0;y<TRACK_MAX;y++)
	  {
	    SG.middleBoxNumberDown(x%16,y,pal[STEP_COLOR]);
	  }



      // we are in song mode
      // Display the current song position by a vertical bar
      for (y=0;y<TRACK_MAX;y++)
	{
	  if (SEQ.getSongSequencer().getStep()/16 == song_cursor_x_div_sixteen)
	    //if (loadsave_cursor_mode==CURSOR_SONG)
	    SG.middleBoxNumberDown(SEQ.getSongSequencer().getStep()%16,
				   y,
				   pal[NOTE_COLOR]);
	}




      // we are in loadsave mode
      // Display your current position
      if (loadsave_cursor_mode==CURSOR_LOADSAVE)
	SG.middleBoxNumberUp(loadsave_cursor.x%16,
			     loadsave_cursor.y,
			     pal[TRIG_COLOR]);

      // we are in song mode
      // Display your current position
      if (loadsave_cursor_mode==CURSOR_SONG)
	SG.middleBoxNumberDown(song_cursor.x%16,
			       song_cursor.y,
			       pal[TRIG_COLOR]);

      
      
      // txt in loadsave box 
      // Display text 00..09..FF
      for (x=loadsave_cursor_x_divmul_sixteen;
	   x<(loadsave_cursor_x_divmul_sixteen)+16;
	   x++)
	for (y=0;y<TRACK_MAX;y++)
	  {
	    SG.drawTTFTextLoadSaveBoxNumberUp(x%16,y,txt_tab[x]);
	  }
      //SG.drawTTFTextLoadSaveBoxNumer(x,y,tmp_txt);

      // txt in Song box 
      // Display text 00..09..FF
      for (x=song_cursor_x_divmul_sixteen;
	   x<(song_cursor_x_divmul_sixteen)+16;
	   x++)
	{
	  for (y=0;y<TRACK_MAX;y++)
	    SG.drawTTFTextLoadSaveBoxNumberDown(x%16,y,txt_tab[SEQ.getSongSequencer().getPatternNumber(x,y)]);
	}
      
    }


  // need to display something, so why not ?
  if (menu!=MENU_OFF && 
      menu_cursor==GLOBALMENU_LS
      )
    {
      display_board_note();
      //display_board_trig();
    }
  DPRINTF("display_board_load_save end");
}




// navigation bar allow user to navigate between 16 step in the 128 available
// call from display_board
void display_board_navigation_bar()
{
  int  cty=SEQ.getCurrentTrackY();
  int  current_step=SEQ.getPatternSequencer(cty).getStep();
  int  current_offset=pattern_display_offset[cty];
  int  pattern_length=SEQ.getPatternSequencer(cty).getPatternLength();

  // Left  bracket
  SG.smallBox((OFFSET_X_PATTERN_CURSOR-10),
	      (OFFSET_Y_PATTERN_CURSOR),
	      pal[TRIG_COLOR]);
  SG.smallBox((OFFSET_X_PATTERN_STEP-10),
	      (OFFSET_Y_PATTERN_STEP),
	      pal[TRIG_COLOR]);
  
  // Right bracket
  SG.smallBox((OFFSET_X_PATTERN_CURSOR+(pattern_length>>4)*8),
	      (OFFSET_Y_PATTERN_CURSOR),
	      pal[TRIG_COLOR]);
  SG.smallBox((OFFSET_X_PATTERN_STEP  +(pattern_length>>4)*8),
	      (OFFSET_Y_PATTERN_STEP),
	      pal[TRIG_COLOR]);
  
  // cursor/step
  SG.smallBox((OFFSET_X_PATTERN_CURSOR+(current_step>>4)*8),
	      (OFFSET_Y_PATTERN_CURSOR),
	      pal[CURSOR_COLOR]);    
  SG.smallBox((OFFSET_X_PATTERN_STEP+  (current_offset>>4)*8),
	      (OFFSET_Y_PATTERN_STEP),
	      pal[STEP_COLOR]);
  
}

void display_board_text_global()
{
  int  i=0;
  char str_line1[64]="";
  char str_line2[64]="";
  char str_line3[64]="";
  char str_line4[64]="";
  char str_line5[64]="";
  char str_line6[64]="";
  char str_menu[64]="";



  //int  right_x_display_offset=      200*SCREEN_MULT;
  int  right_x_display_offset=       COLLUMN40;
  int  right_y_display_offset_line1= LINE02;
  int  right_y_display_offset_line2= LINE03;
  int  right_y_display_offset_line3= LINE04;
  int  right_y_display_offset_line4= LINE05;
  int  right_y_display_offset_line5= LINE06;
  int  right_y_display_offset_line6= LINE07;

  int  menu_x_display_offset=       COLLUMN01;
  int  menu_y_display_offset=       LINE25;

  int  cty=SEQ.getCurrentTrackY();
  int  stepdiv=SEQ.getPatternSequencer(cty).getBPMDivider();
  int  current_step=SEQ.getPatternSequencer(cty).getStep();
  int  current_offset=pattern_display_offset[cty];
  int  pattern_length=SEQ.getPatternSequencer(cty).getPatternLength();

  sprintf(str_line1,    "Track/%d ",cty);

  //sprintf(str_line5,    "Length %.3d/%.3d/%.3d",SEQ.getPatternSequencer(cty).getStep(),SEQ.getPatternSequencer(cty).getPatternLength(),pattern_display_offset[cty]);






  if (menu_note==ENABLE &&
      menu_cursor==GLOBALMENU_NOTE)
      sprintf(str_line2,"NOTE");

  if (menu_note==DISABLE &&
      menu_cursor==GLOBALMENU_NOTE)
      sprintf(str_line2,"DOT");

  if (menu_cursor==GLOBALMENU_BPM)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,  "BPM %.1f ",bpm_current);
      if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line2,  "SWING %d",current_swing);
      if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line3,  "DIV  /%d",stepdiv);
      if (menu_sub==MENU_PAGE0_SUB2) sprintf(str_line2,  "MIDI DELTA SEND %d",delta_midi_clock);
    }

    if (menu_cursor==GLOBALMENU_PSH)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,  "PLEN16/PLEN1 : %d",SEQ.getPatternSequencer(cty).getPatternLength());
      if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line2,  "  PSH4/PSH1  ");
      //if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line2,  "SWING %d",current_swing);
      //if (menu_sub==MENU_PAGE0_SUB2) sprintf(str_line2,  "MIDI DELTA SEND %d",delta_midi_clock);
    }



  if (menu_cursor==GLOBALMENU_LS   && menu==MENU_ON_PAGE2 ) sprintf(str_menu,"[L/S] BANK  PSH   MAC   FX    BPM    ");
  if (menu_cursor==GLOBALMENU_BANK && menu==MENU_ON_PAGE2 ) sprintf(str_menu," L/S [BANK] PSH   MAC   FX    BPM    ");
  if (menu_cursor==GLOBALMENU_PSH  && menu==MENU_ON_PAGE2 ) sprintf(str_menu," L/S  BANK [PSH]  MAC   FX    BPM    ");
  if (menu_cursor==GLOBALMENU_MAC  && menu==MENU_ON_PAGE2 ) sprintf(str_menu," L/S  BANK  PSH  [MAC]  FX    BPM    ");
  if (menu_cursor==GLOBALMENU_FX   && menu==MENU_ON_PAGE2 ) sprintf(str_menu," L/S  BANK  PSH   MAC  [FX]   BPM    ");
  if (menu_cursor==GLOBALMENU_BPM  && menu==MENU_ON_PAGE2 ) sprintf(str_menu," L/S  BANK  PSH   MAC   FX   [BPM]   ");

  //if (menu==0)                                              sprintf(str_menu,"                                     ");

  if (menu_cursor==GLOBALMENU_LS)               sprintf(str_line4,"L/S     ");
  if (menu_cursor==GLOBALMENU_BANK)             sprintf(str_line4,"BANK    ");
  if (menu_cursor==GLOBALMENU_PSH)              sprintf(str_line4,"PSH     ");
  if (menu_cursor==GLOBALMENU_MAC)              sprintf(str_line4,"MAC     ");
  if (menu_cursor==GLOBALMENU_BPM)              sprintf(str_line4,"BPM     ");
  if (menu_cursor==GLOBALMENU_FX)               sprintf(str_line4,"FX      ");

  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line1,str_line1);

  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line2,str_line2);
  
  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line3,str_line3);

  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line4,str_line4);

  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line5,str_line5);

  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line6,str_line6);


  // Display menu at the back of the screen
  // menu==0 don't need to display the menu
  // menu==1 is a menu for the "Machine" and is handled by the machine
  // menu==2 is a global menu
  if (menu==MENU_ON_PAGE2)
    {
      SG.guiTTFText(menu_x_display_offset,
		    menu_y_display_offset,
		    str_menu);
    }


}

void display_board_colored_box()
{
  int  i;
  int start,rstart,end,rend;
  int  cty=SEQ.getCurrentTrackY();
  
  // Draw step which will be playable
  // from step 0 to step 15 or 0 to step 12 for example
  start=pattern_display_offset[cty]; // 0,16,32...
  end=SEQ.getPatternSequencer(cty).getPatternLength(); // 16,18,32...
  if (end-start > 15) end=start+16;
  for (i=start;i<end;i++)
    {
      //DPRINTF("DBOARD1:%d PDO:%d",i,pattern_display_offset[cty]);
      SG.drawBoxNumber(i-pattern_display_offset[cty],pal[ENABLEDBOX_COLOR]); }

  // draw step in DISABLED color if
  // for example if the sequencer is only 12 step, the 4 last step are disabled
  for (i=i;i<pattern_display_offset[cty]+16;i++)   
    {
      //DPRINTF("DBOARD2:%d PDO:%d",i,pattern_display_offset[cty]);
      SG.drawBoxNumber(i-pattern_display_offset[cty],pal[DISABLEDBOX_COLOR]);
    }

}


void display_board()
{
  dirty_graphic=0;
  
  SG.clearScreen();                 // remove all 
  display_board_navigation_bar();   // show begin/current/end step 0, 16, 32
  display_board_text_global();      
  UI->display_board_text();
  display_board_colored_box();      // display the box 0..15, or less
     

  if (menu_cursor==GLOBALMENU_AD)   display_board_amp_env();
  if (menu_cursor==GLOBALMENU_NOTE) display_board_note();
  if (menu_cursor==GLOBALMENU_OSC)  display_board_osc();
  if (menu_cursor==GLOBALMENU_VCO)  display_board_vco();
  if (menu_cursor==GLOBALMENU_LFO)  display_board_lfo();
  if (menu_cursor==GLOBALMENU_FLTR) display_board_fltr();

  if (menu_cursor==GLOBALMENU_LS)   display_board_load_save();
  if (menu_cursor==GLOBALMENU_BANK) display_board_bank();
  if (menu_cursor==GLOBALMENU_PSH)  display_board_psh();
  if (menu_cursor==GLOBALMENU_MAC)  display_board_mac();
  if (menu_cursor==GLOBALMENU_FX)   display_board_fx();
  if (menu_cursor==GLOBALMENU_BPM)  display_board_bpm();
}



void sub_handle_invert_trig()
{
  int    lastEvent;
  int    lastKey;

  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  if (lastKey   == BUTTON_A && 
      lastEvent == KEYPRESSED)
    {
      TK.set(INVERT_TRIG,1);
      //invert_trig=1;
      //TK.set(NOTE_ON,(TK.get(NOTE_ON)+1)%2);
     DPRINTF("key lalt");      
      dirty_graphic=1;
      IE.clearLastKeyEvent();
    }  
}

void handle_key_patternlength()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();


  int    cty=SEQ.getCurrentTrackY();
  int    step=SEQ.getPatternSequencer(cty).getStep();
  int    plen=SEQ.getPatternSequencer(cty).getPatternLength();
  int    need_to_change_cursor_max_pos=0;


  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();



  // +16
  if (lastKey   == BUTTON_R && 
      lastEvent == KEYPRESSED)
    {
      if (pattern_display_offset[cty]<plen-16)
	pattern_display_offset[cty]+=16;
      else
	pattern_display_offset[cty]=0;



      DPRINTF("key BUTTON_R");      
      dirty_graphic=1;
      IE.clearLastKeyEvent();

      need_to_change_cursor_max_pos=1;
    }  

  // -16
  if (lastKey   == BUTTON_L && 
      lastEvent == KEYPRESSED)
    {
      if (pattern_display_offset[cty]>0)
	pattern_display_offset[cty]-=16;
      else
	{
	  pattern_display_offset[cty]=((plen/16)*16)-16;
	  if (plen%16!=0)
	    pattern_display_offset[cty]+=16;
	}

	//pattern_display_offset[cty]=((plen/16)-1)*16;


      DPRINTF("key BUTTON_L");      
      dirty_graphic=1;
      IE.clearLastKeyEvent();

      need_to_change_cursor_max_pos=1;
    }  

  if (need_to_change_cursor_max_pos)
    {
      pattern_cursor_max_pos[cty]=plen-1-pattern_display_offset[cty];
      //DPRINTF("$$$$$$$$$$$$$$$$$$$$$$$$pattern_cursor_max_pos:%d", pattern_cursor_max_pos[cty]);
      if (cursor>pattern_cursor_max_pos[cty])
	cursor=0;
      if (pattern_cursor_max_pos[cty]>15)
	pattern_cursor_max_pos[cty]=15;
      refresh_cursor_display_offset_cursor_max_pos(cty);
      need_to_change_cursor_max_pos=0;
    }
}


void handle_key_stop_start_sequencer()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

    // We stop the sequencer A+B in [BPM] menu
  if (menu             != MENU_OFF       &&
      menu_cursor      == GLOBALMENU_BPM &&
      sequencer_playing==1               &&
      (keyState[BUTTON_B]      &&
       keyState[BUTTON_A]    ) &&
      !(keyState[BUTTON_START])
      )
    {
      IE.clearLastKeyEvent();
      IE.clearStateAndRepeat();
      mmc_stop=1;
      playing_with_mmc=1;
      DPRINTF("MMC-STOP:%d MMC_START:%d",mmc_stop,mmc_start);
    }

    // We force the sequencer A+START in [BPM] menu
  if (menu             != MENU_OFF       &&
      menu_cursor      == GLOBALMENU_BPM &&
      (keyState[BUTTON_START]  &&
       keyState[BUTTON_A]    )
      )
    {
      IE.clearStateAndRepeat();
      IE.clearLastKeyEvent();
      mmc_start=1;
      playing_with_mmc=1;
      DPRINTF("MMC-STOP:%d MMC_START:%d",mmc_stop,mmc_start);
    }
   


}


void handle_key_menu()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

  int last_menu=0xffff;
  int last_menu_cursor=0xffff;
  int menu_switching=0;         // when 1 we will switch from one menu to one another
  
  // Enter and cycle thru menu pages
  if (lastKey   ==  BUTTON_SELECT && 
      lastEvent ==  KEYRELEASED)
    {
      last_menu        = menu;
      last_menu_cursor = menu_cursor;

      switch (menu)
	{

	case MENU_OFF:      
	  menu_switching=1;
	  if (menu_cursor<6) 
	      menu=MENU_ON_PAGE1;
	  if (menu_cursor>=6)
	      menu=MENU_ON_PAGE2;
	  break;

	case MENU_ON_PAGE1: 
	  menu=MENU_ON_PAGE2;
	  menu_switching=1;
	  if (menu_cursor<6)
	      menu_cursor+=6; 
	  else
	      menu_cursor=6;
	  break;

	case MENU_ON_PAGE2: 
	  menu=MENU_ON_PAGE1; 
	  menu_switching=1;
	  if (menu_cursor>=6)
	    menu_cursor-=6; 
	  else
	    menu_cursor=0;
	  break;

	}
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      DPRINTF("[gmenu : %d cmenu : %d]",menu,menu_cursor);
    }
  
  //We enter a menu : ENV/NOTE/VCO etc...
  // 
  if (lastKey      ==  BUTTON_B       && 
      lastEvent    ==  KEYRELEASED    &&      
      (menu        ==  MENU_ON_PAGE1  ||
       menu        ==  MENU_ON_PAGE2))
    {
      DPRINTF("[gmenu : %d cmenu : %d]",menu,menu_cursor);
      // enter a menu only if mmc was not trig AND
      //                   we are not curently changing the submenu
      // helper_change_sub_menu could() modify "playing_with_menu_sub"
      if (playing_with_menu_sub==0 &&
	  playing_with_mmc==0)
	{
	  last_menu        = menu;
	  menu=MENU_OFF;
	  menu_switching=1;
	  loadsave_cursor.y=SEQ.getCurrentTrackY();
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	  DPRINTF("[gmenu : %d cmenu : %d]",menu,menu_cursor);
	}
      else // set back the playing_with_menu_sub to it's default value 0
	{
	  playing_with_menu_sub=0;
	  playing_with_mmc=0;
	}
    }



  //Move into on menu
  //select current active track
  if (
      (menu==MENU_ON_PAGE1  ||
       menu==MENU_ON_PAGE2) &&
      (!
       (keyState[BUTTON_B]    ||
	keyState[BUTTON_A]    ||
	keyState[BUTTON_START]))
      )      
    {
      if(keyState[BUTTON_LEFT]&&
	 !keyState[BUTTON_SELECT])
	{
	  if (keyRepeat[BUTTON_LEFT]    == 1 || 
	      keyRepeat[BUTTON_LEFT]%64 == 0)
	    {
	      menu_cursor--;
	      menu_switching=1;
	      menu_sub=0;
	    }
	  //	  if (menu_cursor<GLOBALMENU_AD  && menu==MENU_ON_PAGE1) menu_cursor=GLOBALMENU_VCO;
	  //	  if (menu_cursor<GLOBALMENU_OSC && menu==MENU_ON_PAGE2) menu_cursor=GLOBALMENU_BPM;
	  if (menu_cursor<0     && menu==MENU_ON_PAGE1) menu_cursor=5;
	  if (menu_cursor<6     && menu==MENU_ON_PAGE2) menu_cursor=11;

	  dirty_graphic=1;
	 DPRINTF("\t\t[menu_cursor:%d]",menu_cursor);
	 DPRINTF("key left");            
	}      
      
      if(keyState[BUTTON_RIGHT] &&
	 !keyState[BUTTON_SELECT])
	{
	  if (keyRepeat[BUTTON_RIGHT]    == 1 || 
	      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG == 0)
	    {
	      menu_cursor++;
	      menu_switching=1;
	      menu_sub=0;
	    }
	  //if (menu_cursor>GLOBALMENU_VCO  && menu==MENU_ON_PAGE1) menu_cursor=GLOBALMENU_AD;
	  //if (menu_cursor>GLOBALMENU_BPM  && menu==MENU_ON_PAGE2) menu_cursor=GLOBALMENU_OSC;
	  if (menu_cursor>5      && menu==MENU_ON_PAGE1) { menu_cursor=0; 	      menu_switching=1; }
	  if (menu_cursor>11     && menu==MENU_ON_PAGE2) { menu_cursor=6;             menu_switching=1; }

	  dirty_graphic=1;
	 DPRINTF("\t\t[menu_cursor:%d]",menu_cursor);
	 DPRINTF("key right");            
	}


      // MANUAL TEMPO SYNC SELECT+LEFT SELECT+RIGHT
      if(keyState[BUTTON_RIGHT] &&
	 keyState[BUTTON_SELECT])
	{
	  if (keyRepeat[BUTTON_RIGHT]    == 1 || 
	      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG == 0)
	    {
	      // store the bpm in a safe place
	      if (bpm_sync_manual==0)
		{ bpm_sync_manual=1; bpm_current_sync_saved=bpm_current; }
	      bpm_sync_inc+=0.1;
	      if (bpm_sync_inc>1.2)
		bpm_sync_inc=1.2;
	      bpm_current=bpm_current_sync_saved+bpm_sync_inc;
	      bpm_sync_need_refresh=1;
	    }
	  DPRINTF("[key select+right ]");
	  dirty_graphic=1;
	}

      if(keyState[BUTTON_LEFT] &&
	 keyState[BUTTON_SELECT])
	{
	  if (keyRepeat[BUTTON_LEFT]    == 1 || 
	      keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG == 0)
	    {
	      // store the bpm in a safe place
	      if (bpm_sync_manual==0)
		{ bpm_sync_manual=1; bpm_current_sync_saved=bpm_current; }
	      bpm_sync_inc-=0.1;
	      if (bpm_sync_inc<-1.2)
		bpm_sync_inc=-1.2;
	      bpm_current=bpm_current_sync_saved+bpm_sync_inc;
	      bpm_sync_need_refresh=1;
	    }
	  DPRINTF("[key select+left ]");
	  dirty_graphic=1;
	}
      // Reset manual tempo sync
      if (lastKey     ==  BUTTON_SELECT  && 
	  lastEvent   ==  KEYRELEASED    )
	{
	  if (bpm_sync_manual==1)
	    {
	      bpm_sync_manual=0;
	      bpm_sync_inc=0;
	      bpm_current=bpm_current_sync_saved;
	      bpm_sync_need_refresh=1;
	    }
	  IE.clearLastKeyEvent();
	}



      
      if(keyState[BUTTON_UP] &&
	 !keyState[BUTTON_SELECT])
	{
	  if (keyRepeat[BUTTON_UP]    == 1 || 
	      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG == 0)
	    {
	      if (SEQ.getCurrentTrackY()>0)
		{
		  SEQ.setCurrentTrackY(SEQ.getCurrentTrackY()-1);
		  menu_switching=1;
		}
	      else 
		{
		  SEQ.setCurrentTrackY(TRACK_MAX-1);
		  menu_switching=1;
		}
	    }
	 DPRINTF("[key up : change track : %d]",SEQ.getCurrentTrackY());
	  dirty_graphic=1;
	}

      if(keyState[BUTTON_DOWN] &&
	 !keyState[BUTTON_SELECT])
	{
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%64==0)
	    {
	      if (SEQ.getCurrentTrackY()<TRACK_MAX-1)
		{
		  SEQ.setCurrentTrackY(SEQ.getCurrentTrackY()+1);
		  menu_switching=1;
		}
	      else 
		{
		  SEQ.setCurrentTrackY(0);	      
		  menu_switching=1;
		}
	    }
	 DPRINTF("[key down : change track : %d]",SEQ.getCurrentTrackY());
	  dirty_graphic=1;
	}
    }

  if (menu_switching>0)
    {
     DPRINTF("Switching back to all default menu");
      menu_note=ENABLE;
      // menu_ad=0;
      // menu_osc=0;
      // menu_vco=0;
      // menu_lfo=0;
      // menu_fltr=0;
      // menu_fx=0;
    }


}


void handle_key_sixteenbox()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

  int    last_cursor=cursor;
  int          cty=SEQ.getCurrentTrackY();


  //MOVE the cursor in the sixteen box : LEFT UP DOWN RIGHT   
  if ((menu==MENU_OFF && menu_cursor==GLOBALMENU_AD    ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_NOTE  ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_VCO   ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_OSC   ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_MAC   ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_LFO   ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_FLTR  ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_BPM   ||
       menu==MENU_OFF && menu_cursor==GLOBALMENU_FX   
       ) &&
      !keyState[BUTTON_B]                   &&
      !keyState[BUTTON_A]                   &&
      !keyState[BUTTON_START]               &&
      !keyState[BUTTON_SELECT]
      )
    {                 

      if(keyState[BUTTON_UP])
	{
	  if (keyRepeat[BUTTON_UP]   ==1 || 
	      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)	    
	    cursor=cursor-4;
	  //if (cursor < 0) cursor=cursor +16;
	  //if (cursor < 0) cursor=cursor +16;
	  if (cursor < 0) cursor=cursor +16;	 
	  if (cursor>pattern_cursor_max_pos[cty])
	    cursor=((pattern_cursor_max_pos[cty]/4)*4)+last_cursor;
	  if (cursor>pattern_cursor_max_pos[cty])
	    cursor=cursor-4;

	  //cursor=cursor-
	  DPRINTF("key down : up");
	  dirty_graphic=1;
	}
      
      if(keyState[BUTTON_DOWN])
	{
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor=( cursor+4 ) %16;
	  if (cursor>pattern_cursor_max_pos[cty])
	    cursor=cursor%4;
	  if (cursor>pattern_cursor_max_pos[cty])
	    cursor=0;
	 DPRINTF("key down : down");
	  dirty_graphic=1;
	}


      // move cursor to the left
      if(keyState[BUTTON_LEFT])
	{
	  if (keyRepeat[BUTTON_LEFT]==1 || 
	      keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor--;
	  
	  //if (cursor<0) cursor=15;
	  if (cursor<0) cursor=pattern_cursor_max_pos[cty];
	 DPRINTF("key left");            
	  dirty_graphic=1;
	}
      
      if (keyState[BUTTON_RIGHT])
	{
	  if (keyRepeat[BUTTON_RIGHT]   ==1 || 
	      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor++;
	  //if (cursor>15) cursor=0;
	  if (cursor>pattern_cursor_max_pos[cty]) cursor=0;
	 DPRINTF("key right");      
	  dirty_graphic=1;
	}
    }
}
// called by The Machine/XXX/XXXUserInterface.cpp
void helper_change_sub_menu(int nb_menu)
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

  // change sub menu without START B+RIGHT | B+LEFT
  if (keyState[BUTTON_B]                   &&
      keyState[BUTTON_RIGHT]               &&
      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONGEST==1 &&
      menu!=MENU_OFF
      )
    {
      if (menu_sub+1 <= nb_menu)
	menu_sub++;
      else
	menu_sub=0;
      playing_with_menu_sub=1;
      IE.clearLastKeyEvent();
    }
    // change sub menu without START B+RIGHT | B+LEFT
  if (keyState[BUTTON_B]                   &&
      keyState[BUTTON_LEFT]                &&
      keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONGEST==1 &&
      menu!=MENU_OFF
      )
    {
      if (menu_sub-1 < 0)
	menu_sub=nb_menu;
      else
	menu_sub--;
      playing_with_menu_sub=1;
      IE.clearLastKeyEvent();
    }

  // change sub menu with START
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED    )
    {     
      if (menu_sub+1 <= nb_menu)
	menu_sub++;
      else
	menu_sub=0;
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]",menu_sub);
    }     
}


void helper_handle_key_two_button(int menu_cursor_tc,
				  int menu_ad_tc,
				  int key_repeat_interval,
				  int param1,
				  int param2,
				  int inc_param1,
				  int inc_param2
				  )
{
  if (menu          == MENU_OFF && 
      menu_cursor   == menu_cursor_tc     &&
      menu_sub       == menu_ad_tc)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
 
      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    key_repeat_interval, param1, -inc_param1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   key_repeat_interval, param1,  inc_param1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      key_repeat_interval, param2,   inc_param2, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    key_repeat_interval, param2,  -inc_param2, 0);

    }  

  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == menu_cursor_tc     &&
      menu_sub      == menu_ad_tc)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    key_repeat_interval, param1, -inc_param1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   key_repeat_interval, param1,  inc_param1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      key_repeat_interval, param2,   inc_param2, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    key_repeat_interval, param2,  -inc_param2, 1);
    }  

}

void helper_handle_key_one_button(int menu_cursor_tc,
				  int menu_ad_tc,
				  int key_repeat_interval,
				  int param1,
				  int inc_param1
				  )
{
  if (menu          == MENU_OFF && 
      menu_cursor   == menu_cursor_tc     &&
      menu_sub       == menu_ad_tc)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
 
      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    key_repeat_interval, param1, -inc_param1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   key_repeat_interval, param1,  inc_param1, 0);

    }  

  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == menu_cursor_tc     &&
      menu_sub      == menu_ad_tc)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    key_repeat_interval, param1, -inc_param1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   key_repeat_interval, param1,  inc_param1, 1);

    }  

}


int handle_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all)
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();



  if (keyState[buttonPressed]  && keyState[buttonKeyRepeat])
    if (keyRepeat[buttonKeyRepeat]==1 ||  keyRepeat[buttonKeyRepeat]%repeatInterval==0)
      { 
	if (all==0)
	  { TK.set(machineParam,paramValue);    dirty_graphic=1; }
	if (all==1)
	  { TK.setAll(machineParam,paramValue); dirty_graphic=1; }
      }
  return 0;
}


void handle_key_amp_env()
{
  UI->handle_key(GLOBALMENU_AD);
}

void handle_key_note()
{
  UI->handle_key(GLOBALMENU_NOTE);
}

void handle_key_osc()
{
  UI->handle_key(GLOBALMENU_OSC);
}


void handle_key_fx()
{

  helper_handle_key_two_button(GLOBALMENU_FX,0,KEY_REPEAT_INTERVAL_SMALLEST,
			       FX1_DEPTH,
			       FX1_SPEED,
			       1,1);
}

void handle_key_vco()
{
    UI->handle_key(GLOBALMENU_VCO);
}


void handle_key_mac()
{
  helper_handle_key_one_button(GLOBALMENU_MAC,0,KEY_REPEAT_INTERVAL_LONG,
			       MACHINE_TYPE,
			       1);
}

void handle_key_lfo()
{
  UI->handle_key(GLOBALMENU_LFO);  
}



void handle_key_psh()
{
    helper_handle_key_two_button(GLOBALMENU_PSH,MENU_PAGE0_SUB0,KEY_REPEAT_INTERVAL_LONG,
			       PATTERN_LENGTH,
			       PATTERN_LENGTH,
			       1,16);

    helper_handle_key_two_button(GLOBALMENU_PSH,MENU_PAGE0_SUB1,KEY_REPEAT_INTERVAL_LONG,
				 PATTERN_SHIFT,
				 PATTERN_SHIFT,
				 1,4);


  /*
  helper_handle_key_two_button(GLOBALMENU_PSH,MENU_PAGE0_SUB0,KEY_REPEAT_INTERVAL_LONG,
			       PATTERN_SHIFT,
			       PATTERN_LENGTH,
			       1,16);
  
  helper_handle_key_two_button(GLOBALMENU_PSH,MENU_PAGE0_SUB1,KEY_REPEAT_INTERVAL_LONG,
			       PATTERN_LENGTH,
			       PATTERN_LENGTH,
			       1,4);
  */
  helper_change_sub_menu(MENU_PAGE0_SUB1);
}



void handle_key_fltr()
{
  UI->handle_key(GLOBALMENU_FLTR);  
}


void handle_key_bpm()
{
  helper_handle_key_two_button(GLOBALMENU_BPM,MENU_PAGE0_SUB0,KEY_REPEAT_INTERVAL_LONG,
			       BPM,
			       BPM,
			       1,10);

  helper_handle_key_two_button(GLOBALMENU_BPM,MENU_PAGE0_SUB1,KEY_REPEAT_INTERVAL_LONG,
			       BPM_DIVIDER,
			       SWING,
			       1,1);

  helper_handle_key_two_button(GLOBALMENU_BPM,MENU_PAGE0_SUB2,KEY_REPEAT_INTERVAL_LONG,
			       MIDI_SEND_DELTA,
			       MIDI_SEND_DELTA,
			       1,10);

  // allw to change sub menu between BPM, BPM_DIVIDER/SWING, MIDI_SEND_DELTA
  helper_change_sub_menu(MENU_PAGE0_SUB2);
}

void handle_key_load_save()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();


  // GLOBALMENU_LS
  // Load/Save 
  if (menu_cursor == GLOBALMENU_LS)
    {
      // in the load/save view 
      // move loasavecursor position 
      // Save/load Pattern


      if (menu                 == MENU_OFF        && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
	  keyState[BUTTON_B]                      &&
	  keyState[BUTTON_A]                      &&
	  keyState[BUTTON_DOWN])

	{
	  patternRemove=true;
	  return;
	}

      // Switch between load/save and song
      if (menu        == MENU_OFF    && 
	  lastEvent   == KEYRELEASED   &&
	  lastKey     == BUTTON_START
	  //keyState[BUTTON_START]
	  )
	{
	  if      (loadsave_cursor_mode==CURSOR_LOADSAVE)  loadsave_cursor_mode=CURSOR_SONG;
	  else if (loadsave_cursor_mode==CURSOR_SONG)      loadsave_cursor_mode=CURSOR_LOADSAVE;
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	  return;
	}


      // song mode
      // increment/decrement value of the current bar
      if (menu                 == MENU_OFF    && 
	  loadsave_cursor_mode == CURSOR_SONG &&
	  keyState[BUTTON_B] )
	{
	  if (keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==1)
	    {
	      pattern_song_inc=-1;
	      song_need_saving=1;
	    }
	  if (keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==1)
	    {
	      pattern_song_inc=1;
	      song_need_saving=1;
	    }
	}


      if (menu                 == MENU_OFF && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
	  keyState[BUTTON_B])
	{
	  if (keyState[BUTTON_DOWN])
	    { save=true; dirty_graphic=1; }
	  if (keyState[BUTTON_UP])	
	    { load=true;  }           // can't load and copy a a pattern to a copy paste element today ... 
	                              // a pattern can only be save horizontaly...
	}

      
      // GLOBALMENU_LS
      // in the load/save view 
      // move loasavecursor position 
      // Save/load bund of Pattern
      
      if (menu                 == MENU_OFF        && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
	  keyState[BUTTON_A] )
	{
	  if (keyState[BUTTON_DOWN])
	    { saveall=true; dirty_graphic=1; }
	  if (keyState[BUTTON_UP])	
	    loadall=true;
	}


      if (menu                 == MENU_OFF        && 
	  loadsave_cursor_mode == CURSOR_SONG     &&
	  keyState[BUTTON_A] )
	{
	  if (keyState[BUTTON_DOWN])
	    pattern_song_loop=1;
	    //saveall=true;
	  if (keyState[BUTTON_UP])	
	    pattern_song_reload=1;
	}

      
      // GLOBALMENU_LS
      // in the load/save view 
      // move load/save cursor position 
      if (menu                 == MENU_OFF        && 	  
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
	  (!(
	     keyState[BUTTON_B] ||
	     keyState[BUTTON_A])))
	{
	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1  || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { loadsave_cursor.x--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { loadsave_cursor.x++;  dirty_graphic=1;}
	  // move cursor, warning select+up is increase volume
	  if (keyState[BUTTON_UP] && keyState[BUTTON_SELECT]==0)
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { loadsave_cursor.y--;  dirty_graphic=1;}
	  // move cursor, warning select+down is decrease volume
	  if (keyState[BUTTON_DOWN] && keyState[BUTTON_SELECT]==0)
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { loadsave_cursor.y++;  dirty_graphic=1;}
	  
	  if (loadsave_cursor.x>MAX_PATTERN_BY_PROJECT-1)        { loadsave_cursor.x=0;                           }
	  if (loadsave_cursor.x<0)                               { loadsave_cursor.x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (loadsave_cursor.y>TRACK_MAX-1)                     { loadsave_cursor.y=0;                           }
	  if (loadsave_cursor.y<0)                               { loadsave_cursor.y=TRACK_MAX-1;                 }  
	  
	  SEQ.setCurrentTrackY(loadsave_cursor.y);
	}


      // Song mode
      // moving the cursor in the song mode
      if (menu                 == MENU_OFF     && 	  
	  loadsave_cursor_mode == CURSOR_SONG  &&
	  (!(
	     keyState[BUTTON_B] ||
	     keyState[BUTTON_A])))
	{
	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1  || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor.x--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { song_cursor.x++;  dirty_graphic=1;}
	  // move cursor, warning select+up is increase volume
	  if (keyState[BUTTON_UP]  && keyState[BUTTON_SELECT]==0)
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { song_cursor.y--;  dirty_graphic=1;}
	  // move cursor, warning select+down is decrease volume
	  if (keyState[BUTTON_DOWN] && keyState[BUTTON_SELECT]==0)
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor.y++;  dirty_graphic=1;}
	  
	  if (song_cursor.x>MAX_SONG_LENGHT_BY_PROJECT-1)        { song_cursor.x=0;                           }
	  if (song_cursor.x<0)                                   { song_cursor.x=MAX_SONG_LENGHT_BY_PROJECT-1;}

	  if (song_cursor.y>TRACK_MAX-1)                         { song_cursor.y=0;                           }
	  if (song_cursor.y<0)                                   { song_cursor.y=TRACK_MAX-1;                 }  
	  
	  //SEQ.setCurrentTrackY(loadsave_cursor_y);
	}


      
      // Shift the pattern of the song to the left or to the right
      // A+LEFT || A+RIGHT
      if (menu                 == MENU_OFF     && 	  
	  loadsave_cursor_mode == CURSOR_SONG  &&
	  (
	   keyState[BUTTON_A]
	   ))
	{
	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)
	      {
		SEQ.getSongSequencer().shiftTrackLeft(song_cursor.x,song_cursor.y);
		song_need_saving=1;
	      }

	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0)
	      {
		SEQ.getSongSequencer().shiftTrackRight(song_cursor.x,song_cursor.y);
		song_need_saving=1;
	      }

	}
      
      // copy the current element of the song to a next element ( right or left )
      // arm the song_need_saving variable
      // B+LEFT || B+RIGHT
      if (menu                 == MENU_OFF     && 	  
	  loadsave_cursor_mode == CURSOR_SONG  &&
	  (
	   keyState[BUTTON_B]
	   ))
	{
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      {
		song_cursor.x++;
		SEQ.getSongSequencer().setPatternNumber(song_cursor.x,song_cursor.y,SEQ.getSongSequencer().getPatternNumber(song_cursor.x-1,song_cursor.y));
		dirty_graphic=1;
		song_need_saving=1;
	      }

	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      {
		song_cursor.x--;
		SEQ.getSongSequencer().setPatternNumber(song_cursor.x,song_cursor.y,SEQ.getSongSequencer().getPatternNumber(song_cursor.x+1,song_cursor.y));
		dirty_graphic=1;
		song_need_saving=1;
	      }


	  
	  if (song_cursor.x>MAX_SONG_LENGHT_BY_PROJECT-1)        { song_cursor.x=0;                           }
	  if (song_cursor.x<0)                                   { song_cursor.x=MAX_SONG_LENGHT_BY_PROJECT-1;}

	  if (song_cursor.y>TRACK_MAX-1)                         { song_cursor.y=0;                           }
	  if (song_cursor.y<0)                                   { song_cursor.y=TRACK_MAX-1;                 }  
	  
	}


    }

}


void handle_key_bank()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();


  // GLOBALMENU_LS
  // Load/Save 
  if (menu_cursor == GLOBALMENU_BANK)
    {
      // in the load/save view 
      // move loasavecursor position 
      // Save/load Pattern



      // Switch between load/save and song
      if (menu        == MENU_OFF    && 
	  lastEvent   == KEYRELEASED   &&
	  lastKey     == BUTTON_START
	  //keyState[BUTTON_START]
	  )
	{
	  
	  bank=bank_to_load;
	  PR.init();
	  PR.setBank(bank);
	  load_pattern();
	  PR.loadSong(SEQ.getSongSequencer());
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	  return;
	}



      if (menu                 == MENU_OFF    && 
	  keyState[BUTTON_B])
	//keyRepeat[BUTTON_B]%KEY_REPEAT_INTERVAL_LONG==0)
	{
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)
	    bank_inc=-1;
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)
	      bank_inc=1;
	}
      
    }
}



void refresh_pecursor_ui(int i)
{
  int  cty=SEQ.getCurrentTrackY();
  int machine_type;
  int exit_code=8;
  PECursor=P[cty].getPatternElement(i);  
  machine_type=PECursor.get(MACHINE_TYPE);
  
  if (!MC.machineExist(machine_type))
    {
      machine_type=MC.getNext(machine_type);
      P[cty].getPatternElement(i).set(MACHINE_TYPE,machine_type);
    }
  
  DPRINTF("refresh_pecursor_ui(%d),machine_type=%d",i,machine_type);
#ifdef       ENABLE_SYNTH_PICOSYNTH
  if (machine_type==SYNTH_PICOSYNTH)    { UI=&PSUI; return ;   }
#endif

#ifdef       ENABLE_SYNTH_PICODRUM
  if (machine_type==SYNTH_PICODRUM)     { UI=&PDUI; return ;   }
#endif

#ifdef       ENABLE_SYNTH_OPL2
  if (machine_type==SYNTH_OPL2    )     { UI=&DBUI; return ;   }
#endif  

#ifdef       ENABLE_SYNTH_PBSYNTH
  if (machine_type==SYNTH_PBSYNTH)      { UI=&PBUI; return ;   }
#endif

#ifdef       ENABLE_SYNTH_LGPTSAMPLER
  if (machine_type==SYNTH_LGPTSAMPLER)  { UI=&LGUI; return ;   }
#endif

#ifdef       ENABLE_SYNTH_CURSYNTH
  if (machine_type==SYNTH_CURSYNTH)     { UI=&CSUI;   return ; }
#endif

#ifdef       ENABLE_SYNTH_OPEN303
  if (machine_type==SYNTH_OPEN303)      { UI=&O303UI; return ; }
#endif

#ifdef       ENABLE_SYNTH_TWYTCHSYNTH
  if (machine_type==SYNTH_TWYTCHSYNTH)  { UI=&TWUI;   return ; }
#endif

#ifdef       ENABLE_SYNTH_MDADRUM
  if (machine_type==SYNTH_MDADRUM)      { UI=&MDUI;   return ; }
#endif

#ifdef       ENABLE_SYNTH_SIDSYNTH
  if (machine_type==SYNTH_SIDSYNTH)     { UI=&SSUI;   return ; }
#endif

#ifdef       ENABLE_SYNTH_MIDIOUT
  if (machine_type==SYNTH_MIDIOUT)      { UI=&MIDIUI; return ; }
#endif

  
  //DPRINTF("refresh_pecursor_ui(%d)\nmachine_type=%d\nexit(%d)\nNO UI found\n",i,machine_type,exit_code);
  //fprintf(stderr,"refresh_pecursor_ui(%d)\nmachine_type=%d\nexit(%d)\nNO UI found\n",i,machine_type,exit_code);
  printf("refresh_pecursor_ui(%d) machine_type=%d exit(%d) : !!!!! NO UI found !!!!!",i,machine_type,exit_code);
  CLOSE_DPRINTF();
  exit(8);
}

/*
    refresh_pecursor is used to have a : 
     Machine User Interface updated to the current track, it is not optimal at all
  
   algo: 
     Fetch a PECursor with noteOn==1 from the current cursor.
     If there is no noteOn==1 on the current cursor, fetch it from all step and keep the lastest value
*/
void refresh_pecursor()
{
  int  cty=SEQ.getCurrentTrackY();
  int  i=0;
  int  cursor_note_on=-1;
  // Refresh the PECursor with the first NOTE_ON
  // for (i=0;i<=15;i++)
  //   {
  //     if (P[cty].getPatternElement(i).get(NOTE_ON))
  // 	cursor_note_on=i;
  //   }
  //refresh_pecursor_ui(cursor_note_on);
  refresh_pecursor_ui(0);
  
}

void handle_key_change_volume()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();

  if (keyState[BUTTON_SELECT] && keyState[BUTTON_DOWN])
    if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0)
      {
	//AudioMixer & am=AE.getAudioMixer();
	AM->setAudioVolume(AM->getAudioVolume()-1);
	IE.clearLastKeyEvent();
	return;
      }
  if (keyState[BUTTON_SELECT] && keyState[BUTTON_UP])
    if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0)
      {
	//AudioMixer & am=AE.getAudioMixer();
	AM->setAudioVolume(AM->getAudioVolume()+1);
	IE.clearLastKeyEvent();
	return;
      }  
}

// handle_key call the IE.handleKey() which will read keyboard and keypad
// it then call the appropriate function depending on the menu
void handle_key()
{
  IE.handleKey();

  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  
  if (IE.shouldExit())
    quit=1;

  // this handler are allways call
  handle_key_change_volume();        // SELECT+UP/DOWN change volume
  handle_key_patternlength();        // L|R change the view from 0-15,16-31...
  handle_key_stop_start_sequencer(); // you want to stop/start the sequencer
  handle_key_menu();                 // you want to change the menu ?
  handle_key_sixteenbox();


  if (menu_cursor==GLOBALMENU_AD)   
    { handle_key_amp_env();  }
  if (menu_cursor==GLOBALMENU_NOTE) 
    { handle_key_note();     }
  if (menu_cursor==GLOBALMENU_OSC)  
    { handle_key_osc();      }
  if (menu_cursor==GLOBALMENU_VCO)  
    { handle_key_vco();      }
  if (menu_cursor==GLOBALMENU_LFO)  
    { handle_key_lfo();      }
  if (menu_cursor==GLOBALMENU_FLTR) 
    { handle_key_fltr();     }

  if (menu_cursor==GLOBALMENU_LS)   handle_key_load_save();
  if (menu_cursor==GLOBALMENU_BANK) handle_key_bank();
  if (menu_cursor==GLOBALMENU_PSH)  handle_key_psh();
  if (menu_cursor==GLOBALMENU_MAC)  handle_key_mac();
  if (menu_cursor==GLOBALMENU_FX)   handle_key_fx();
  if (menu_cursor==GLOBALMENU_BPM)  handle_key_bpm();
}

void seq_update_tweakable_knob_one(int machineParam)
{
  int            cty=SEQ.getCurrentTrackY();
  if (TK.get(machineParam)!=0)
    {
      update_SAMM(cty,cursor+pattern_display_offset[cty]);
      P[cty].getPatternElement(cursor+pattern_display_offset[cty]).set(machineParam,
					   SAM->checkI(machineParam,P[cty].getPatternElement(cursor+pattern_display_offset[cty]).get(machineParam)+TK.get(machineParam)));


	//SAM->checkI(machineParam,P[cty].getPatternElement(cursor).get(AMP)+TK.get(machineParam)));
      TK.set(machineParam,0);
      //if (debug)
      DPRINTF("[param:%d:%d]",machineParam,P[cty].getPatternElement(cursor).get(machineParam));
    }  
}

void seq_update_tweakable_knob_all(int machineParam)
{
  int          cty=SEQ.getCurrentTrackY();
  int          i;
  int    plen=SEQ.getPatternSequencer(cty).getPatternLength();
  
  if (TK.getAll(machineParam)!=0)
    {
      //for (i=pattern_display_offset[cty];i<pattern_display_offset[cty]+16;i++)
      for (i=0;i<plen;i++)
	{
	  // if (P[cty].getPatternElement(i).get(NOTE_ON))
	  //   {
	  update_SAMM(cty,i);
	  P[cty].getPatternElement(i).set(machineParam,
					  SAM->checkI(machineParam,P[cty].getPatternElement(i).get(machineParam)+TK.getAll(machineParam)));
	      //}
	}
      TK.setAll(machineParam,0);
      //if (debug)
      DPRINTF("[paramAll:%d:%d]",machineParam,P[cty].getPatternElement(cursor).get(machineParam));
    }  
}


// this function is called multiple time in one step
// it's goal is to put at 0 every "state" variable :
// - TK which contain the MACHINE_TYPE, BPM_DIVIDER, CUTOFF, etc... of the current track
// - also some more "hackish" variable which may be moved to another function
//   - bank_inc to change the current bank
//   - mmc_stop to put the four sequencer of the track at step 0

void seq_update_multiple_time_by_step()
{
  //AudioMixer & am=AE.getAudioMixer();
  // printf("seq_update_multiple_time_by_step()\n");
  int          cty=SEQ.getCurrentTrackY();
  int          ctx=SEQ.getCurrentTrackX();
  int          step=SEQ.getPatternSequencer(cty).getStep();
  int          oldstep=0;
  int          i=0;
  int          t=0;
  int          currentMachine;
  int          nextMachine;
  int          knobNonZero=0;
  int          knobAllNonZero=0;

  //DPRINTF("*********************pattern_cursor_max_pos:%d",pattern_cursor_max_pos[cty]);
  
  // Read all valued contained in the Tweakable Knob
  // Apply the Value to the sequencer, then to the Machine


  if (  TK.get(   MACHINE_TYPE)!=0 ||
	TK.getAll(MACHINE_TYPE)!=0)
    {
      currentMachine=P[cty].getPatternElement(cursor+pattern_display_offset[cty]).get(MACHINE_TYPE);

      if ((TK.get(  MACHINE_TYPE)>0)||
	  TK.getAll(MACHINE_TYPE)>0)
	{
	  nextMachine=MC.getNext(currentMachine);
	  for (i=0;i<MAX_STEP_PER_TRACK;i++)
	    P[cty].getPatternElement(i).set(MACHINE_TYPE,nextMachine);
	}

      if ((TK.get(  MACHINE_TYPE)<0)||
	  TK.getAll(MACHINE_TYPE)<0)

	{
	  nextMachine=MC.getPrevious(currentMachine);
	  for (i=0;i<MAX_STEP_PER_TRACK;i++)
	    P[cty].getPatternElement(i).set(MACHINE_TYPE,nextMachine);
	}
      TK.set(   MACHINE_TYPE,0);
      TK.setAll(MACHINE_TYPE,0);
      DPRINTF("currentMachine:%d,nextMachine:%d",currentMachine,nextMachine);
    }


  if (TK.get(BPM_DIVIDER)!=0)
    {	  
      if (TK.get(BPM_DIVIDER)>0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()*2);
      if (TK.get(BPM_DIVIDER)<0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()/2);
      TK.set(BPM_DIVIDER,0);
      P[cty].setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider());
    }


  // Change PatternLength of a track
  // update the : Pattern in memory, the Sequencer and the cursor_max_position
  if (TK.getAll(PATTERN_LENGTH)!=0 ||
      TK.get(PATTERN_LENGTH)!=0)
    { 
      int cur_plen=SEQ.getPatternSequencer(cty).getPatternLength();
      int dest_plen=SEQ.getPatternSequencer(cty).getPatternLength()+TK.getAll(PATTERN_LENGTH)+TK.get(PATTERN_LENGTH);
      int i;
      int j;

      // prevent negative size for example
      if (dest_plen<2)
	dest_plen=cur_plen;

      // duplicate the 16 first step into : 
      // 16-31
      // 32-47
      // 48-63 etc
      if (cur_plen<=16 &&
	  dest_plen==cur_plen+16)
	{
	  for (i=0;i<16;i++)
	    {
	      for (j=1;j<MAX_STEP_PER_TRACK/16;j++)
		P[cty].getPatternElement(j*16+i)=P[cty].getPatternElement(i);
	    }
	}

      SEQ.getPatternSequencer(cty).setPatternLength(SEQ.getPatternSequencer(cty).getPatternLength()+TK.getAll(PATTERN_LENGTH));
      TK.setAll(PATTERN_LENGTH,0);
      TK.set(PATTERN_LENGTH,0);
      P[cty].setSize(SEQ.getPatternSequencer(cty).getPatternLength());

      
      pattern_cursor_max_pos[cty]=dest_plen-1-pattern_display_offset[cty];
      if (pattern_cursor_max_pos[cty]>15)
	pattern_cursor_max_pos[cty]=15;
      if (pattern_cursor_max_pos[cty]<cursor)
	cursor=pattern_cursor_max_pos[cty];
      if (pattern_display_offset[cty] > SEQ.getPatternSequencer(cty).getPatternLength())
	pattern_display_offset[cty]=0;
      DPRINTF("*********************pattern_cursor_max_pos:%d",pattern_cursor_max_pos[cty]);
    }

  /*
  if (TK.getAll(PATTERN_LENGTH)!=0 ||
      TK.get(PATTERN_LENGTH)!=0
      )
    { 
      SEQ.getPatternSequencer(cty).setPatternLength(SEQ.getPatternSequencer(cty).getPatternLength()+TK.getAll(PATTERN_LENGTH));
      TK.setAll(PATTERN_LENGTH,0);
      TK.set(PATTERN_LENGTH,0);
      P[cty].setSize(SEQ.getPatternSequencer(cty).getPatternLength());
    }
  */


  // shift PatternEntry
  if (TK.getAll(PATTERN_SHIFT)!=0 ||
      TK.get(PATTERN_SHIFT)!=0)
    {       
      PatternElement Pe;
      int time=TK.getAll(PATTERN_SHIFT)+TK.get(PATTERN_SHIFT);
      //printf("time:%d\n",time);
      DPRINTF("time:%d",time);
      if (TK.getAll(PATTERN_SHIFT)>0 ||
	  TK.get(PATTERN_SHIFT)>0)
	{
	  Pe=P[cty].getPatternElement(pattern_cursor_max_pos[cty]+pattern_display_offset[cty]);
	  for (i=pattern_cursor_max_pos[cty];i>0;i--)
	    {	    
	      P[cty].getPatternElement(i+pattern_display_offset[cty])=P[cty].getPatternElement(i-1+pattern_display_offset[cty]);	    
	    }
	  i--;
	  P[cty].getPatternElement(0+pattern_display_offset[cty])=Pe;	    
	}
      
      if (TK.getAll(PATTERN_SHIFT)<0 ||
	  TK.get(PATTERN_SHIFT)<0)
	{
	  Pe=P[cty].getPatternElement(0+pattern_display_offset[cty]);
	  for (i=0;i<pattern_cursor_max_pos[cty];i++)
	    {
	      P[cty].getPatternElement(i+pattern_display_offset[cty])=P[cty].getPatternElement(i+1+pattern_display_offset[cty]);	    
	    }
	  i++;
	  P[cty].getPatternElement(pattern_cursor_max_pos[cty]+pattern_display_offset[cty])=Pe;
	}
      TK.setAll(PATTERN_SHIFT,0);
      TK.set(PATTERN_SHIFT,0);
      dirty_graphic=1;
     DPRINTF("[pshift_all:left]");	  
    }


  if (bank_inc!=0)
    {
      bank_to_load=bank_to_load+bank_inc;
      bank_inc=0;
      if (bank_to_load>MAX_BANK-1)
	bank_to_load=0;
      if (bank_to_load<0)
	bank_to_load=MAX_BANK-1;
    }


  if (TK.get(BPM)!=0)
    {
      float bpm_diff=TK.get(BPM);
      // bpm can be change by -0.1,0.1 or -1.0,1.0
      bpm_diff=bpm_diff/10; 

     DPRINTF("Recalculate BPM");
     //bpm_current=P[cty].getBPM();
      for(t=0;t<TRACK_MAX;t++)	    
	P[t].setBPM(bpm_current+bpm_diff);
      
      //bpm_current=bpm_current+TK.get(BPM);
      bpm_current=bpm_current+bpm_diff;
      TK.set(BPM,0);
      refresh_bpm();      
    }


  if (TK.get(MIDI_SEND_DELTA)!=0)
    {
      counter_delta_midi_clock += TK.get(MIDI_SEND_DELTA);
      delta_midi_clock         += TK.get(MIDI_SEND_DELTA);
      TK.set(MIDI_SEND_DELTA,0);
    }

  
  if (bpm_sync_need_refresh)
    {
      refresh_bpm();
    }
  
  if (TK.get(SWING)!=0)
    {
      //save the swing in the Pattern
      //change the number of time AudioEngine need to be trigged
      //to effectively change pattern step
     DPRINTF("Recalculate Swing");
      current_swing=current_swing+TK.get(SWING);
      TK.set(SWING,0);
      
      if (current_swing>75)
	current_swing=75;
      if (current_swing<25)
	current_swing=25;
      
      for(t=0;t<TRACK_MAX;t++)	    
	P[t].setSwing(current_swing);
      
      refresh_bpm();
    }
  
  

      // invert trig => insert/remove/copy note 
  if (TK.get(INVERT_TRIG)!=0)
    {
      // if there is a "NOTE_ON" element here I copy it in the PE global object
      // PE is a PatternElement object
      if (P[cty].getPatternElement(cursor+pattern_display_offset[cty]).get(NOTE_ON))
	{
	  P[cty].getPatternElement(cursor+pattern_display_offset[cty]).set(NOTE_ON,(P[cty].getPatternElement(cursor+pattern_display_offset[cty]).get(NOTE_ON)+1)%2);
	  PE=P[cty].getPatternElement(cursor+pattern_display_offset[cty]);
	}
      else // I paste the PE Object here
	{
	  int mt;
	  P[cty].setPatternElement(cursor+pattern_display_offset[cty],PE);
	  P[cty].getPatternElement(cursor+pattern_display_offset[cty]).set(NOTE_ON,1);
	  if (P[cty].getPatternElement(cursor+pattern_display_offset[cty]).get(NOTE1)==0)
	    {
	      P[cty].getPatternElement(cursor+pattern_display_offset[cty]).set(NOTE1,25); // NOTE1=25 mean => C2
	    }
	  // Now I need to set the MachineType on all step to ensure all the step use the same machine!
	  mt=P[cty].getPatternElement(cursor+pattern_display_offset[cty]).get(MACHINE_TYPE);
	  for (i=0;i<MAX_STEP_PER_TRACK;i++)
	    P[cty].getPatternElement(i).set(MACHINE_TYPE,mt);
	}
      TK.set(INVERT_TRIG,0);
    }


  // Process all button and knob change ADSR, OSC... see Machine.h for the list
  // Must be done at the end because
  //     - they are not related to the Sequencer BPM, SWING, Number of Step ( done before on this function )
  //     - they are     related to     Synth parameter Cutoff, ADSR, etc...
  // Each call seq_update_tweakable_knob_XXX reset the knob to 0
  
  while (TK.getNonZero()!=-1)
    {
      knobNonZero=TK.getNonZero();
      seq_update_tweakable_knob_one(knobNonZero);
      TK.set(knobNonZero,0);
    }

  while (TK.getAllNonZero()!=-1)
    {
      knobAllNonZero=TK.getAllNonZero();
      seq_update_tweakable_knob_all(knobAllNonZero);
      TK.setAll(knobAllNonZero,0);      
      //seq_update_tweakable_knob_all(TK.getAllNonZero());
    }
  // midi machine command stop related
  // midi related but not only, A+B on the [BPM] menu "should"
  //  - on first  A+B : stop  the sequencer
  //  - on second A+B : start the sequencer
  if (mmc_stop>0)
    {
      mmc_stop=0;
      sequencer_playing=0;     // we stop the sequencer
      //      for(i=0;i<TRACK_MAX;i++)
      //{
	  //SEQ.getPatternSequencer(i).setStep(0);
      //}
    }
  if (mmc_start>0)
    {
      lock_audiocallback.lock();
      mmc_start=0;
      counter_recv_midi_clock=0;    
      counter_recv_midi_clock_six=0;
      sequencer_playing=1;     // we start the sequencer from the beginning
      //printf("mmc start >0\n");
      for(i=0;i<TRACK_MAX;i++)
	{
	  SEQ.getPatternSequencer(i).setStep(0);
          //printf("seq update track\n");
	  seq_update_track(i);
	}
      lock_audiocallback.unlock();
    }
    if (mmc_continue>0)
    {
      mmc_continue=0;
      sequencer_playing=1;     // we stop the sequencer
    }

}



int seq_update_by_step()
{
  int  cty=SEQ.getCurrentTrackY();
  int  ctx=SEQ.getCurrentTrackX();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  int  t;

  //SEQ.getPatternSequencer(0).setStepDivider(4);
  //printf("seq update by step(func) TRACK_MAX%d %d %d %d\n", TRACK_MAX, cty, ctx, step);
  if (pattern_song_inc!=0)
    {
      printf("pattern song inc %d %d %d\n", song_cursor.x, song_cursor.y, pattern_song_inc);
      SEQ.getSongSequencer().setPatternNumber(song_cursor.x,
					      song_cursor.y,
					      SEQ.getSongSequencer().getPatternNumber(song_cursor.x,
										      song_cursor.y)+
					      pattern_song_inc);
      pattern_song_inc=0;
    }

  if (pattern_song_reload!=0)
    {

      SEQ.getSongSequencer().setStep(song_cursor.x);
      pattern_song_reload=0;      
      SEQ.setSongMode(1);

      for (t=0;t<TRACK_MAX;t++)
	{
	  if (PR.PatternDataExist(SEQ.getSongSequencer().getPatternNumber(song_cursor.x,t),t))
	    {
              printf("reading pattern data %d %d \n", song_cursor.x, t);
	      PR.readPatternData(SEQ.getSongSequencer().getPatternNumber(song_cursor.x,t),t,P[t]);
	      //SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
	    }
	  else
	    {
	      P[t].init();
	      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
	      init_cursor_display_offset_cursor_max_pos(t);
	      refresh_cursor_display_offset_cursor_max_pos(t);
	      //init_cursor_display_offset_cursor_max_pos(t);
	      
	      //refresh_cursor_display_offset_cursor_max_pos();
	    }
	}
	    

    }

  if (pattern_song_loop!=0)
    {
      //printf("song mode active\n");
      SEQ.getSongSequencer().setLoopPoint(song_cursor.x);
      pattern_song_loop=0;
      SEQ.setSongMode(1);
    }


  // Stop the song mode if load/save activated ( in load/save )
  if (load    |
      save    | 
      loadall |
      saveall )
    SEQ.setSongMode(0);


  // Load save only on pattern change
  if (save)
    {
     DPRINTF("<==[SAVE]==>");
      //PR.writePattern(1,ct+1,P[ct]);
      PR.writePattern(loadsave_cursor.x,loadsave_cursor.y,P[cty]);
      load_save_highligth_current[loadsave_cursor.y]=loadsave_cursor.x;
      dirty_graphic=1;
      save=false;
    }

  // Load save only on pattern change
  if (load)
    {
     DPRINTF("<==[LOAD]==>");
      if (PR.PatternDataExist(loadsave_cursor.x,loadsave_cursor.y)==true)
	{
	  PR.readPatternData(loadsave_cursor.x,loadsave_cursor.y,P[cty]);
	  load_save_highligth_current[loadsave_cursor.y]=loadsave_cursor.x;


	  SEQ.getPatternSequencer(cty).setPatternLength(P[cty].getSize());
	  SEQ.getPatternSequencer(cty).setBPMDivider(P[cty].getBPMDivider());
	  P[cty].setSize(SEQ.getPatternSequencer(cty).getPatternLength());

	  // update BPM and Swing only if bpm_lock==0
	  // and for example (int)120.1 != 120

	  if (bpm_lock==0 && (int)bpm_current != (int)P[cty].getBPM())
	    {
	      bpm_current=P[cty].getBPM();
	      current_swing=P[cty].getSwing();
	    }
	  else
	    {
	      P[cty].setBPM(bpm_current);
	    }
	  refresh_bpm();
	}
      else
	{
	  P[cty].init();
	  SEQ.getPatternSequencer(cty).setBPMDivider(P[cty].getBPMDivider());
	  init_cursor_display_offset_cursor_max_pos(cty);
	  load_save_highligth_current[loadsave_cursor.y]=-1;
	  //init_cursor_display_offset_cursor_max_pos(cty);
	  refresh_cursor_display_offset_cursor_max_pos(cty);
	}
      init_cursor_display_offset_cursor_max_pos(cty);
      refresh_cursor_display_offset_cursor_max_pos(cty);
      load=false;
    }


  // Load save only on pattern change
  if (loadall)
    {
     DPRINTF("<==[LOAD_ALL]==>");
      for (t=0;t<TRACK_MAX;t++)
	{
	  if (PR.PatternDataExist(loadsave_cursor.x,t)==true)
	    {
	      PR.readPatternData(loadsave_cursor.x,t,P[t]);
	      load_save_highligth_current[t]=loadsave_cursor.x;


	      SEQ.getPatternSequencer(t).setPatternLength(P[t].getSize());
	      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
	      P[t].setSize(SEQ.getPatternSequencer(t).getPatternLength());

	      // update BPM and Swing only if bpm_lock==0
	      // and for example (int)120.1 != 120
	      // AND if we are on the loadsave_cursor.y

	      if (bpm_lock==0 && (int)bpm_current != (int)P[t].getBPM() && t==loadsave_cursor.y)
		{
		  bpm_current=P[t].getBPM();
		  current_swing=P[t].getSwing();
		}
	      else // bpm_lock==1 for example
		{
		  P[t].setBPM(bpm_current);
		  P[t].setSwing(current_swing);
		}
	      refresh_bpm();
	    }
	  else
	    {
	      P[t].init();
	      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
	      init_cursor_display_offset_cursor_max_pos(t);
	      load_save_highligth_current[t]=-10; // don't highlight
	      //init_cursor_display_offset_cursor_max_pos(t);
	      refresh_cursor_display_offset_cursor_max_pos(t);
	    }
	  init_cursor_display_offset_cursor_max_pos(t);
	  refresh_cursor_display_offset_cursor_max_pos(t);		
	}

      loadall=false;
    }


  // Load save only on pattern change
  if (saveall)
    {
     DPRINTF("<==[SAVE_ALL]==>");
      //PR.writePattern(1,ct+1,P[ct]);
      for (t=0;t<TRACK_MAX;t++)
	{
	  PR.writePattern(loadsave_cursor.x,t,P[t]);
	  load_save_highligth_current[t]=loadsave_cursor.x;
	}
      dirty_graphic=1;
      saveall=false;      
    }



  // Load save only on pattern change
  if (patternRemove)
    {
     DPRINTF("<==[REMOVE]==>");
      if (PR.PatternDataExist(loadsave_cursor.x,loadsave_cursor.y)==true)
	{
	  if (PR.PatternRemove(loadsave_cursor.x,loadsave_cursor.y))
	    {
	     DPRINTF("<==[REMOVE==Success]==>");
	      dirty_graphic=1;
	    }
	}
      else
	{
	  P[cty].init();
	  SEQ.getPatternSequencer(cty).setBPMDivider(P[cty].getBPMDivider());
	  init_cursor_display_offset_cursor_max_pos(cty);
	  refresh_cursor_display_offset_cursor_max_pos(cty);
	  //init_cursor_display_offset_cursor_max_pos(cty);    
	 DPRINTF("<==[Remove==Failed]==>");
	}
      patternRemove=false;
    }

    
  //if (step==16) 
  //    step=0; 

  if (debug)
   DPRINTF("STEP:%d",step);	  
  return 0;
}


//static std::vector<int> OSC_ARRAY(16) = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int OSC_ARRAY[] = {0,0,0,0,0,25,0,0,0,0,0,0,0,0,0,0};
static int loop=0;
static int seedsong=0;
std::vector<std::vector<int> > master_array; // bars/thread -> pattern index
std::vector<std::vector<int> > note_array; // pattern/step -> note

//static int master_loop=0;
static int patternt[4][16]
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,31,0,31,0,0,0,31,0,35,0},
      {0,0,0,0,25,24,25,0,0,25,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
void seq_update_track(int t)
{
  int master_loop = SEQ.getSongSequencer().m_master_loop;  
  if (master_loop == 0){
    //master_array.resize(bars); // number of bars
    //master_array[0].resize(4); // threads
    
    osc_array.resize(11);
    for(int i=0; i<11; ++i){
      osc_array[i].resize(10);
      for(int j = 0; j < 10; ++j){
        osc_array[i][j].resize(16);
        for(int k = 0; k < 16; ++k){
          osc_array[i][j][k].resize(4);
          for(int l=0; l < 4; ++l){
            osc_array[i][j][k][l] = 0;
          }
        }
      }
    }
  }
  //int  cty=SEQ.getCurrentTrackY();
  //int  ctx=SEQ.getCurrentTrackX();
  //int  step=SEQ.getPatternSequencer(cty).getStep();
  int machine_type;
  int  step=SEQ.getPatternSequencer(t).getStep();
  int bar = SEQ.getSongSequencer().getStep(); //jke song position 0 = start of song, loops may be from e.g. 2-4

  int pn = SEQ.getSongSequencer().getPatternNumberAtCursorPosition(t);
  int  xx = SEQ.getCurrentTrackX();
  int  cty=SEQ.getCurrentTrackY();
   
  // here t is in 0,1,2,3, it's the bank within the bank
  // song_step is the position in the main grid
  // pn is the bank number
  // step is the index (0-15)
  // t is the sub bank (0-3)
  // nn is the note flag
  // note1 is the actual note
  //int  step=SEQ.getPatternSequencer(cty).getStep();
  //printf("xx: %d, cty: %d t:%d \n", xx, cty, t);
  // Handle only modification of on next step value 
  // for each Machine 
  // of each track 
  //for (t=0;t<TRACK_MAX;t++)
  //{
  int nn = P[t].getPatternElement(step).get(NOTE_ON);
  //nn = osc_array[song_step][pn][step][(int)t]; // read in from array
  //nn = patternt[t][master_loop * 6 * 16 + step];
  FILE* pf2 = fopen("tstep.txt", "a");
  //P[t].print();
  int f_pn = master_array[t][bar]; // bars/thread -> pattern index
  nn = note_array[f_pn][step]; // // pattern/step -> note
  int note1 = nn;
  //printf("n:%d,%d,%d,%d\n", f_pn, step, t, nn);
  if (nn==0){
    //fprintf(pf2, "%d,%d,%d,%d,%d,%d\n", pn, step, t, nn, 0);
    osc_array[bar][pn][step][(int)t] = nn;
    fclose(pf2);
  }
  if (nn!=0) //(t==0) and (step == 0))// nn!=0) // 1 jke    
    {
      //printf("seq update track for %d %d %d %d\n", pn, step, t, NOTE_ON);
      
      
      
      machine_type=P[t].getPatternElement(step).get(MACHINE_TYPE);
          // printf
	  if (!MC.machineExist(machine_type))
	    {
	      machine_type=MC.getNext(machine_type);
	      P[t].getPatternElement(step).set(MACHINE_TYPE,machine_type);
	    }
          int amp = P[t].getPatternElement(step).get(AMP);;
	  MM[t]->setAmplitude(amp);
	  MM[t]->setMachineType(P[t].getPatternElement(step).get(MACHINE_TYPE));
	  M[t]  = MM[t]->getInput();
	  FX[t] = MM[t]->getEffect();

          int depth = P[t].getPatternElement(step).get(FX1_DEPTH);
          int speed = P[t].getPatternElement(step).get(FX1_SPEED);
	  FX[t]->setDepth(depth);
	  FX[t]->setSpeed(speed);

	  //printf("[Freq:%d]\n",i);
	  //M[t]->set(OSC1_FREQ,i);
	  M[t]->reset();
	  //M[t]->setF(OSC1_FREQ,f);

          int osc1 = P[t].getPatternElement(step).get(OSC1_SCALE);
          int osc2 = P[t].getPatternElement(step).get(OSC2_SCALE);
          int osc3 = P[t].getPatternElement(step).get(OSC3_SCALE);
          int osc4 = P[t].getPatternElement(step).get(OSC4_SCALE);
          //osc1 = OSC_ARRAY[step];
          //note1 = P[t].getPatternElement(step).get(NOTE1);
          
          //note1 = nn;
          ////if(step == 5)
          //            note1=25;
          fprintf(pf2, "%d,%d,%d,%d,%d,%d\n", bar, pn, step, t, nn, note1);
          char buff[100];
          //int master_loop = SEQ.getSongSequencer().m_master_loop;
          sprintf(buff,"pfileout_notes_%d.txt", master_loop);
          FILE* pf1 = fopen(buff, "a");  
          fprintf(pf1,  "%d,%d,%d,%d,%d,%d\n", master_loop,bar, pn, step, t, note1);
          fclose(pf1);
          
          osc_array[bar][pn][step][(int)t] = note1;
          //write out osc_array to file
          //fprintf(pf2, "%d,%d,%d,%d,%d,%d\n", loop,song_step,t,step,nn,note1);
          fclose(pf2);
          int trig_time_dur = P[t].getPatternElement(step).get(TRIG_TIME_DURATION);
          int fm_type = P[t].getPatternElement(step).get(FM_TYPE);
          //printf("amp %d %d %d\n", amp, depth, speed);
          FILE* pf = fopen("pfileout.txt", "a");
          fprintf(pf, "step: %d,%d,amp:%d,depth:%d,speed:%d\n", step, NOTE_ON, amp, depth, speed);
          fclose(pf);

          //printf("osc1 note1 %d %d %d %d %d %d bpm:%d fm_type:%d\n", osc1, osc2, osc3, osc4, note1, trig_time_dur, (int)bpm_current, fm_type);
          // osc1 note1 0 0 0 0 25 1 bpm:120 fm_type:0
	  M[t]->setI(OSC1_SCALE,osc1);
	  M[t]->setI(OSC2_SCALE,osc2);
	  M[t]->setI(OSC3_SCALE,osc3);
	  M[t]->setI(OSC4_SCALE,osc4);
	  M[t]->setI(NOTE1,note1);

	  /*
	  noteOffTrigger[t]=
	    P[t].getPatternElement(step).getAttack_amp()+
	    P[t].getPatternElement(step).getAttack_fltr()
	    ;
	  */
	  //noteOffTrigger[t]=P[t].getPatternElement(step).getTrigTime()/8;

	  // Let the Machine handle NoteOn/NoteOff with a trig time duration
	  M[t]->setI(TRIG_TIME_MODE,1);
	  M[t]->setI(TRIG_TIME_DURATION,trig_time_dur);
	  M[t]->setI(BPM,bpm_current);

	  M[t]->setI(FM_TYPE,fm_type);
          int adsr1 = P[t].getPatternElement(step).get(ADSR_AMP_ATTACK);

          int adsrdecay = P[t].getPatternElement(step).get(ADSR_AMP_DECAY);
          adsrdecay = 110;
          //printf("adsr1 %d %d\n", adsr1, adsrdecay);
	  M[t]->setI(ADSR_ENV0_ATTACK,  adsr1);
	  M[t]->setI(ADSR_ENV0_DECAY,   adsrdecay);
	  M[t]->setI(ADSR_ENV0_SUSTAIN, P[t].getPatternElement(step).get(ADSR_AMP_SUSTAIN));
	  M[t]->setI(ADSR_ENV0_RELEASE, P[t].getPatternElement(step).get(ADSR_AMP_RELEASE));

	  M[t]->setI(ADSR_ENV1_ATTACK,  P[t].getPatternElement(step).get(ADSR_FLTR_RELEASE));
	  M[t]->setI(ADSR_ENV1_DECAY ,  P[t].getPatternElement(step).get(ADSR_FLTR_DECAY));
	  M[t]->setI(ADSR_ENV1_SUSTAIN, P[t].getPatternElement(step).get(ADSR_FLTR_SUSTAIN));
	  M[t]->setI(ADSR_ENV1_RELEASE, P[t].getPatternElement(step).get(ADSR_FLTR_RELEASE));



	  M[t]->setI(OSC12_MIX,P[t].getPatternElement(step).get(VCO_MIX));
	  M[t]->setI(OSC1_PHASE,P[t].getPatternElement(step).get(OSC1_PHASE));

	  M[t]->setI(LFO1_DEPTH,     P[t].getPatternElement(step).get(LFO1_DEPTH));
	  M[t]->setI(LFO1_ENV_AMOUNT,P[t].getPatternElement(step).get(LFO1_ENV_AMOUNT));
	  M[t]->setF(LFO1_FREQ,      P[t].getPatternElement(step).get(LFO1_FREQ));

	  M[t]->setI(LFO2_DEPTH,     P[t].getPatternElement(step).get(LFO2_DEPTH));
	  M[t]->setI(LFO2_ENV_AMOUNT,P[t].getPatternElement(step).get(LFO2_ENV_AMOUNT));
	  M[t]->setF(LFO2_FREQ,      P[t].getPatternElement(step).get(LFO2_FREQ));

	  M[t]->setI(PITCHBEND_DEPTH,P[t].getPatternElement(step).get(PITCHBEND_DEPTH));
	  M[t]->setI(PITCHBEND_SPEED,P[t].getPatternElement(step).get(PITCHBEND_SPEED));
	  M[t]->setI(LFO_TYPE,       P[t].getPatternElement(step).get(LFO_TYPE));

	  //M[t]->set(OSC12_MIX,phase);

	  M[t]->setI(OSC1_TYPE,P[t].getPatternElement(step).get(OSC1_TYPE));
	  M[t]->setI(OSC2_TYPE,P[t].getPatternElement(step).get(OSC2_TYPE));

	  M[t]->setI(OSC3_TYPE,P[t].getPatternElement(step).get(OSC3_TYPE));

	  M[t]->setI(LFO1_WAVEFORM,P[t].getPatternElement(step).get(LFO1_WAVEFORM));
	  M[t]->setI(LFO2_WAVEFORM,P[t].getPatternElement(step).get(LFO2_WAVEFORM));


	  M[t]->setI(OSC1_AMP,P[t].getPatternElement(step).get(OSC1_AMP));
	  M[t]->setI(OSC2_AMP,P[t].getPatternElement(step).get(OSC2_AMP));

	  M[t]->setI(OSC3_AMP,P[t].getPatternElement(step).get(OSC3_AMP));
	  M[t]->setI(OSC4_AMP,P[t].getPatternElement(step).get(OSC4_AMP));

	  M[t]->setI(OSC1_UNISON,P[t].getPatternElement(step).get(OSC1_UNISON));
	  M[t]->setI(OSC2_UNISON,P[t].getPatternElement(step).get(OSC2_UNISON));

	  M[t]->setI(OSC1_UNISONDT,P[t].getPatternElement(step).get(OSC1_UNISONDT));
	  M[t]->setI(OSC2_UNISONDT,P[t].getPatternElement(step).get(OSC2_UNISONDT));


	  M[t]->setI(FILTER1_TYPE,P[t].getPatternElement(step).get(FILTER1_TYPE));
	  M[t]->setI(FILTER1_ALGO,P[t].getPatternElement(step).get(FILTER1_ALGO));;


	  M[t]->setI(FILTER1_CUTOFF,   P[t].getPatternElement(step).get(FILTER1_CUTOFF));
	  M[t]->setI(FILTER1_RESONANCE,P[t].getPatternElement(step).get(FILTER1_RESONANCE));

	  M[t]->setI(FILTER1_SATURATION,   P[t].getPatternElement(step).get(FILTER1_SATURATION));
	  M[t]->setI(FILTER1_FEEDBACK,     P[t].getPatternElement(step).get(FILTER1_FEEDBACK));

	  M[t]->setI(ENV1_DEPTH,      P[t].getPatternElement(step).get(ENV1_DEPTH));
	  M[t]->setI(ENV2_DEPTH,      P[t].getPatternElement(step).get(ENV2_DEPTH));
	  M[t]->setI(ENV3_DEPTH,      P[t].getPatternElement(step).get(ENV3_DEPTH));
	  M[t]->setI(ENV4_DEPTH,      P[t].getPatternElement(step).get(ENV4_DEPTH));

	  M[t]->setI(OSC1_DETUNE,      P[t].getPatternElement(step).get(OSC1_DETUNE));
	  M[t]->setI(OSC2_DETUNE,      P[t].getPatternElement(step).get(OSC2_DETUNE));

	  M[t]->setI(VELOCITY,         P[t].getPatternElement(step).get(VELOCITY));
	  M[t]->setI(OSC1_MOD,         P[t].getPatternElement(step).get(OSC1_MOD));




	  M[t]->setI(NOTE_ON,1);
#ifdef __RTMIDI__
	  //SDL_LockAudio();
	  MidiOutSystem & MOS=MidiOutSystem::getInstance();
	  //MOS.flushMsg();
	  //SDL_UnlockAudio();
#endif 

	}
      /*
      else
	{
	  if (noteOffTrigger[t]<0)
	    {
	     DPRINTF("$$$NOTEOFF\n");
	      //M[t]->getADSRAmp().setNoteOff();
	      //M[t]->getADSRFltr().setNoteOff();
	      //M[t]->reset();

	      //M[t]->setI(NOTE_ON,0);
	    }
	  else
	    {
	      noteOffTrigger[t]=noteOffTrigger[t]-1;
	     DPRINTF("noteOffTrigger[%d]=%d\n",noteOffTrigger[t],t);
	    }
	}
      */
}

void write_out_osc_array(){
  char buff[100];
  sprintf(buff,"pfileout_master_%d.txt", SEQ.getSongSequencer().m_master_loop);
  printf("buff %s\n", buff);
  FILE* pf = fopen(buff, "w");
  fprintf(pf,"bar,beat,pattern_index,ch1,ch2,ch3,ch4\n"); // to play a song only requires bar, beat. 
  for(int i = 0; i < 11; ++i){     // song step in the musical bars
    for(int k = 0; k < 16; ++k){   // step in the 16th song seq
      for(int j = 0; j < 10; ++j){ // pattern number (each can be its own generative cell)
        int val1 = osc_array[i][j][k][0];
        int val2 = osc_array[i][j][k][1];
        int val3 = osc_array[i][j][k][2];
        int val4 = osc_array[i][j][k][3];        
        if ((val1 > 0) or (val2 > 0) or (val3 > 0) or (val4 > 0))
          {
            fprintf(pf, "%d,%d,%d,%d,%d,%d,%d\n", i, k, j, osc_array[i][j][k][0], osc_array[i][j][k][1], osc_array[i][j][k][2], osc_array[i][j][k][3]);
          }
      }
    }
  }
  fclose(pf);
}

// This callback run in a really high priority thread
// No IO or syscall is allowed to prevent audio drifting/garbage
// EX : printf      SHOULD BE forbiden ( not the case today ) 
//      read(fd...) MUST BE forbiden ( unfortunately not the case too )
void seq_callback_update_step()
{
  int i;
  int oldstep;
  int songSequencerHasInc=0;
  
  if (sequencer_playing==0) // we don't have something to do the sequencer is not playing
    return;
  //for ( i=0;i<10;i++)
  //  {
  //SEQ.getSongSequencer().setPatternNumberAtCursorPosition(i,0,i);
  //}
  // write out osc
  //
  //master_loop += 1;
  int pn = SEQ.getPatternSequencer(0).m_step;
  int bar = SEQ.getSongSequencer().m_song_step;
  printf("master loop song:%d bar:%d quarter note:%d seed:%d\n", SEQ.getSongSequencer().m_master_loop, bar, pn, seedsong);
  if(SEQ.getSongSequencer().m_inc == 1 && pn==15 && bar==6){
    printf("write out osc array\n");
    write_out_osc_array();
  }
  read_from_file(master_array, "bars_threads.csv"); //[thread][bars] -> pattern
  read_from_file(note_array, "note_array.csv"); //[pattern][step] -> note 
  for(i=0;i<TRACK_MAX;i++)
    {
      oldstep=0;

      oldstep=SEQ.getPatternSequencer(i).getStep();
      SEQ.getPatternSequencer(i).incStep();
      if (oldstep!=SEQ.getPatternSequencer(i).getStep())
	{
	  if (SEQ.getPatternSequencer(i).getStep()==0)
	    {

	      if (songSequencerHasInc == 0 &&
		  SEQ.getSongMode()   == 1		  
		  )
		{
		  SEQ.getSongSequencer().incStep();
		  songSequencerHasInc++;
		}

	      if (SEQ.getSongMode()   == 1)		
		if (PR.PatternDataExist(SEQ.getSongSequencer().getPatternNumberAtCursorPosition(i),i))
		  {
                    //JKE this is where the new P[i] is set.
                    //int pattern = master_array[i][bar];
                    //int* notes = pattern_array[pattern];
                    int pn= SEQ.getSongSequencer().getPatternNumberAtCursorPosition(i); //pattern number 
                    //printf("setting P[i] %d\n", pn);
		    PR.readPatternData(pn,i,P[i]);
		    SEQ.getPatternSequencer(i).setBPMDivider(P[i].getBPMDivider());
		  }
		else
		  {
		    P[i].init();
		    SEQ.getPatternSequencer(i).setBPMDivider(P[i].getBPMDivider());
		    init_cursor_display_offset_cursor_max_pos(i);
		    refresh_cursor_display_offset_cursor_max_pos(i);
		    //init_cursor_display_offset_cursor_max_pos(i);
		  }
	     
	      //if (i==TRACK_MAX-1)
	      //{
	      //printf("SongSequencer.cursorPosition:%d\n",SEQ.getSongSequencer().getStep());
	      //}
	    }
	  
	  // Update the Machine for the new step
          //printf("seq update trackloop\n");
	  seq_update_track(i);	  
	}
    }
  dirty_graphic=1;
  seq_update_by_step_next=1;
  refresh_bpm();
  //counter_recv_midi_clock=0;
  //counter_recv_midi_clock_six=0;


}

void init_monomixer_and_machine()
{
  int          t=0;
  int          i=0;
  for (t=0;t<TRACK_MAX;t++)
    {
      //MM[t]=AE.getAudioMixer().getMonoMixer(t);
      MM[t]=AM->getMonoMixer(t);
      MM[t]->init();
      MM[t]->setMonoMixerChannelNumber(t);
      MM[t]->setAmplitude(0);  // set the amplitude to 0 to avoid trashing speakers
      
      M[t]=MM[t]->getInput();                             
      M[t]->init();

      P[t].init();
      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
      init_cursor_display_offset_cursor_max_pos(t);
    }
  SAMM.init(); // init this one too
  // startup all track 
  // dump 64 sample without using each 64 sample
  // to avoir some trash

  for (t=0;t<TRACK_MAX;t++)
    {
      DPRINTF("DUMPING 64 sample from track:%d",t);
      for (i=0;i<64;i++)
	{
	  //printf("MM[t]=%d\n",MM[t]);
	  MM[t]->tick();
	}
    }
  
}

void init_and_setup_midi()
{

#ifdef __RTMIDI__

  // Open output Port
  MidiOutSystem & MOS=MidiOutSystem::getInstance();
  MOS.chooseMidiPortDeviceNumber(menu_config_midiOutput);
  
  // Open Input Port
  MidiInSystem & MIS=MidiInSystem::getInstance();
  MIS.chooseMidiPortDeviceNumber(menu_config_midiInput);

  // init the thread midi used only with RTMIDI and if sync out is enabled
  if (menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCOUT)
    {
#ifdef __SDL12__
      thread_midiclock_send = SDL_CreateThread( thread_seq_send_midiclock, NULL );
#endif
#ifdef __SDL20__
      thread_midiclock_send = SDL_CreateThread( thread_seq_send_midiclock, "midiclockthread", NULL );
#endif
    }
  if (menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCIN ||
      menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCINTERNAL)
    {
      MidiInSystem  & MIS=MidiInSystem::getInstance();
      MIS.setupcallback();
    }
  // AE need to trig the seqCallback when nb_tick>=nb_tick_before_step_change
  AE.setMidiClockMode(menu_config_midiClockMode);
#endif
}


void init_audiomixer_and_audioengine()
{
  int i;
  AM=new AudioMixer();
  AM->setAudioVolume(DEFAULT_VOLUME);
  AE.setAudioMixer(AM);
  init_monomixer_and_machine();
  // Init all track to the current step, step0 in this particular case 
  for (i=0;i<TRACK_MAX;i++)
    seq_update_track(i);
}


// Here is the while (1) { loop } of the application
int seq()
{
  //AudioMixer * AM=AE.getAudioMixer();
  int          cty=SEQ.getCurrentTrackY();
  int          ctx=SEQ.getCurrentTrackX();
  int          step=SEQ.getPatternSequencer(cty).getStep();
  int          oldstep=0;
  int          i=0;
  int          delay=1;
  int          t=0;
  int          running=1;
  int          nbcb;        // current nb audio callback 
  int          last_nbcb;   // number of occurence of AudioEngine callback before changing step

  printf("currentTrackY %d currentTrackX %d\n", cty, ctx);
  dirty_graphic=1;
  refresh_pecursor();

  DPRINTF("PatternPlayer::seq()");  
  DPRINTF("openAudio start streaming");
  AE.startAudio();

  // JKE this is where the loop point is set
  SEQ.getSongSequencer().setLoopPoint(5);
  pattern_song_loop = 0;
  SEQ.setSongMode(1);
  // end jke, this starts up a loop between position 0 and 6 automatically
  // so that we can dump audio directly

  // this is where we could load in from a file
  // set pattern
  seq_update_by_step_next=1;
  seq_update_by_step();
  
  while (quit!=1 || running==1)
    {

      //read_from_file(note_array, "note_array.csv"); //[pattern][step] -> note
      
      cty=SEQ.getCurrentTrackY();
      ctx=SEQ.getCurrentTrackX();
#ifdef __SDL_AUDIO__
      SDL_LockAudio();
      handle_key();
      SDL_UnlockAudio();
#else
      handle_key();
#endif
      //printf("main while loop \n");
      seq_update_multiple_time_by_step();

      // We give the processor to other thread to avoid spinning
      //SDL_Delay(delay);  
#ifdef __LINUX__
      //sched_yield();
      SDL_Delay(1);
#else
      SDL_Delay(1); 
#endif
#ifdef PSP
      running = isRunning();
#endif

#if defined(__RTMIDI__)
      // printf("midi out\n");
      MidiOutSystem & MOS=MidiOutSystem::getInstance();
      if (MOS.msgSize())
       	MOS.flushMsg();
#endif

      if (SEQ.getSongSequencer().m_master_loop == 1){
        printf("completed a loop\n");
        return(0);
      }
      // if user want to quit via handle_key
      if (quit ||
	  running<=0)
	{
	  DPRINTF("user want to quit");
	  return(0);
	}

      // Number of audio callback, it is related to performance
      nbcb=AE.getNbCallback();
      if (nbcb>last_nbcb){
	last_nbcb=nbcb;
      }

      if (dirty_graphic)
	{
	  refresh_pecursor();
	  display_board();
#ifdef __SDL_AUDIO__	  
	  SDL_LockAudio();
	  SG.refresh();
	  SDL_UnlockAudio();
#else
          //printf("SG.refresh();\n");
	  SG.refresh();
#endif
	  dirty_graphic=0;
	}

      // We are not in the L/S menu and song was modified
      // we flush it
      if (song_need_saving &&
	  menu             != MENU_OFF)
	{
	  PR.saveSong(SEQ.getSongSequencer());
	  song_need_saving=0;	      
	}

      // change step in the pattern
      if (seq_update_by_step_next)
	{	 
	  seq_update_by_step();
	  seq_update_by_step_next=0;
	}
      //printf("while loop\n");
    }
  return 0;
}

void load_pattern()
{
  int t;
  int p;

  string fileName="data.pic";

  // Disable highlight
  disable_load_save_highlight();
  vector <Pattern>            TMPP(TRACK_MAX);  

  //PR.setFileName("data.pic");
  //PR.setFileName(fileName);

  PR.loadSong(SEQ.getSongSequencer());

  // Warmup the patternReader cache
  for (t=0;t<TRACK_MAX;t++)
    for (p=0;p<MAX_PATTERN_BY_PROJECT;p++)
      {
	PR.readPatternData(p,t,TMPP[t]);
      }
}


void wtg()
{
  Generator G;
  WaveTableManager & WTM = WaveTableManager::getInstance();
  WaveTable* WT;


  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.sine();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SINE);


  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.saw();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SAW);

  
  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.pulse();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_PULSE);

  
  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.triangle();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_TRGL);

  
  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothSine();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMSINE);

  
  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothSaw();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMSAW);

  
  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothPulse();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMPULSE);

  
  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothTriangle();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMTRGL);



  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.noise();
  //memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_NOISE);

  
  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.LFSRNoise();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_LFSRNOISE);

    
  DPRINTF("wavetablemanager.getSize : %d",WTM.getSize());
}

void init_vita_debug()
{
#if defined(PSVITA) && defined(DEBUG_PRINTF)
  //psp2shell_init(3333,0);
  //psp2shell_init(3333);
  SDL_Delay(5000);
  //psp2shell_print("Start\n");
  sceClibPrintf("Start\n");
#endif
}

void init_psp()
{
  int cpu_speed;
  int running=1;
#ifdef PSP
  running = isRunning();
  setupExitCallback();
  cpu_speed=scePowerGetCpuClockFrequencyInt();
 DPRINTF("CURRENT PSP CPU SPEED:%d",cpu_speed);
  //cpu_speed=333;
  //scePowerSetCpuClockFrequency(cpu_speed);
  //scePowerSetClockFrequency(333, 333, 166);
  scePowerSetClockFrequency(266, 266, 133);
  cpu_speed=scePowerGetCpuClockFrequencyInt();
 DPRINTF("NEW PSP CPU SPEED:%d",cpu_speed);
#else
 running=1; // if we are not on psp, running should be 1  
#endif
}

void finish_psp()
{
#ifdef PSP
  sceKernelExitGame();	
#endif
}

void finish_vita()
{
#if defined(PSVITA)
  sceKernelExitProcess(0);
#endif 
}

void init_midi()
{
#ifdef __RTMIDI__
  MidiOutSystem & MOS=MidiOutSystem::getInstance();
  MidiInSystem  & MIS=MidiInSystem::getInstance();
  MOS.init();
  MIS.init();  
#endif
}

void init_and_load_config()
{
  config_loaded=0;
  config_key_pressed=0;
  config_first_time=1;
  while (config_loaded!=1)
	{
	  display_config();
	  handle_key_config();
	  handle_config();
	  SDL_Delay(1);  
	}
}

void init_lgpt_samplepool()
{
#if defined(__RAM512MIB__)
  SamplePool * SP=SamplePool::GetInstance();
  SyncMaster * SM=SyncMaster::GetInstance();
  char pathtosample[128];
  int i;

  DEB.Boot(argc,argv);
  for (i=0;i<127;i++)
    {
#if defined(PSVITA)
      sprintf(pathtosample,"ux0:/app/PICOLOOP1/samples/%d.wav",i);
#else
      sprintf(pathtosample,"samples/%d.wav",i);
#endif
      SP->loadSample(i,pathtosample);
      //SP->loadSample(i,"samples/0.wav");
      //SP->loadSample(1,"samples/1.wav");
    }
#endif
}

int main(int argc,char **argv)
{
  char* p;
  if(argc == 2){

    seedsong = strtol(argv[1], &p,10);
    printf("loading %d\n", seedsong);
  }
  printf("load pattern1");
  int i;
  loadsave_cursor.x=0;
  loadsave_cursor.y=0;
  song_cursor.x=0;
  song_cursor.y=0;
#if defined(__LINUX__) && !defined(OPENDINGUX)
  signal(SIGSEGV, handler);   // install our handler
  signal(SIGABRT, handler);
#endif
  
#if !defined(__PBSYNTHONLY__)
  UI=&PSUI;          // The UI is set as default on PicoSynthUI
#endif
#if defined(__PBSYNTHONLY__)
  UI=&PBUI;
#endif
    
  init_vita_debug(); // used only on psvita
  init_psp();        // used only on psp
  init_midi();       // used when we have midi in/out

  wtg();             // Generate the Waveform use by picoSynth/PicoDrum

  init_lgpt_samplepool();

  NoteFreq & NF = NoteFreq::getInstance();
  NF.init();      // init the NoteFrequency
  TK.init();      // init the Tweakable Knob
  IE.init();      // init the Input Manager
  IE.printState();

  DPRINTF("openVideo output");
  SG.initVideo();  // Init the video outpout
  //SG.openBMPFont();
  if (SG.openTTFFont()==false) {DPRINTF("ttf font error"); exit(1); }
  SG.loadingScreen();
  SDL_Delay(1000);

  //init_and_load_config(); jke uncomment to restore the check in screen
      
  PR.init();         // Init the     storage bank
  PR.setBank(bank);  // The current  storage bank will be the value of bank the directory/file are here PWD/bank/bank%d/
                     // menu_config_bank allow to choose it at startup

  printf("load pattern");
  load_pattern();    // load the pattern of the bank 
  
  read_from_file(master_array, "bars_threads.csv"); //[thread][bars] -> pattern
  read_from_file(note_array, "note_array.csv"); //[pattern][step] -> note

  DPRINTF("after load pattern");
  printf("refresh bbm");
  refresh_bpm();
  AE.setupSequencerCallback(seq_callback_update_step);
  printf("refresh openAudio");
  DPRINTF("openAudio output");
  AE.openAudio();
  printf("init_audiomixer");
  init_audiomixer_and_audioengine();
  printf("init_midi");
  init_and_setup_midi();
  
  DPRINTF("before seq");
  printf("before seq-2");
  seq();        // Launch the sequencer which will stream audio
  printf("after seq-2");
  DPRINTF("after seq");

  DPRINTF("closeAudio output");
  AE.stopAudio();
  AE.closeAudio();
  
  DPRINTF("closeVideo output");
  SG.closeVideo();
  DPRINTF("Exiting PatternPlayer");
  finish_psp();
  finish_vita();
  CLOSE_DPRINTF();
}

