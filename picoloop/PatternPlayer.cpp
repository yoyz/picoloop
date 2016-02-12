using namespace std;

#ifdef PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <psppower.h>
#include "psp_callback.h"
#define VERS    1 //Talk about this
#define REVS    0

PSP_MODULE_INFO("PatternPlayer", PSP_MODULE_USER, VERS, REVS);
//PSP_MODULE_INFO("PatternPlayer", PSP_MODULE_USER, VERS, REVS);
//PSP_MODULE_INFO("TESTPRX", 0x1000, 1, 1);
//PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|THREAD_ATTR_VFPU);
//PSP_HEAP_SIZE_MAX();
//PSP_HEAP_SIZE_KB(16384);
//PSP_HEAP_SIZE_KB(-1024);
//PSP_HEAP_SIZE_KB(20480) ;
//PSP_HEAP_SIZE_KB(-1024) ;
//PSP_HEAP_SIZE_KB(-4096) ;
//PSP_HEAP_SIZE_KB(-6144) ;
//PSP_HEAP_SIZE_KB(-8192) ;
//PSP_HEAP_SIZE_KB(-12288) ;

//PSP_HEAP_SIZE_KB(-6144) ;
//PSP_HEAP_SIZE_KB(-8192) ;
//PSP_HEAP_SIZE_KB(-1024) ;
//PSP_HEAP_SIZE_KB(-12288) ;
//PSP_HEAP_SIZE_KB(-8192) ;
//PSP_HEAP_SIZE_KB(-4096) ;
//PSP_HEAP_SIZE_KB(-8192) ;
PSP_HEAP_SIZE_KB(-2048) ;
#endif

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

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
#include "Sequencer.h"

#include "Generator.h"
#include "WaveTableManager.h"
#include "WaveTable.h"

#include "NoteFreq.h"
#include "TweakableKnob.h"

#include "UserInterface.h"
#include "Machine/Picosynth/PicosynthUserInterface.h"
#include "Machine/Picodrum/PicodrumUserInterface.h"
#include "Machine/Dbopl/DboplUserInterface.h"
#include "Machine/PBSynth/PBSynthUserInterface.h"

#ifdef __FPU__
#include "Machine/Cursynth/CursynthUserInterface.h"
#include "Machine/Open303/Open303UserInterface.h"
#include "Machine/Twytch/TwytchsynthUserInterface.h"
#include "Machine/MDADrum/MDADrumUserInterface.h"
#endif


#ifdef __RTMIDI__
#include "MidiOutSystem.h"
#include "Machine/MidiOutSystem/MidiOutUserInterface.h"
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
PicosynthUserInterface   PSUI;
PicodrumUserInterface    PDUI;
DboplUserInterface       DBUI;
PBSynthUserInterface     PBUI;

#ifdef __FPU__
CursynthUserInterface    CSUI;
Open303UserInterface     O303UI;
TwytchsynthUserInterface TWUI;
MDADrumUserInterface     MDUI;
#endif

#ifdef __RTMIDI__
MidiOutUserInterface MIDIUI;
#endif

//UI.handle_key(1,1);
//UI.a=1;
//UI=PUI;
//PUI.a=1;

// highlight the loaded and saved active track 
// load_save_highligth_current[0..t-1]=-1 by default
// load_save_highligth_current[0..t-1]>=0 active played track 
int load_save_highligth_current[TRACK_MAX];


vector <Pattern>            P(TRACK_MAX);  
vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);
vector <Effect    *>        FX(TRACK_MAX);


MonoMixer                   SAMM; // Standalone MonoMixer 
                                  //  used to get   a standalone Machine SAM
                                  //  used to check the boundary of value of different element
                                  //  it is used in seq_update_multiple_time_by_step()

Machine  *                   SAM; // Standalone Machine
                                  //  it is used in seq_update_multiple_time_by_step()


Sequencer      SEQ;         // used to  store/get information about sequencer 
AudioEngine    AE;          // used to  init alsa/rtaudio
PatternReader  PR;          // used to  read data.pic file
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

//int bpm_current=120;    // current value for the four ( TRACK_MAX ) tracks
float bpm_current=120.0;    // current value for the four ( TRACK_MAX ) tracks


//int nbcb=0;             // current nb audio callback 
//int last_nbcb=0;        // number of occurence of AudioEngine callback before changing step
//int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm); // Weird ?
//int nb_cb_ch_step;              //=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm_current);

int nb_tick_before_step_change; //=(60*DEFAULT_FREQ)/(bpm_current*4);

//int last_nbcb_ch_step=0;// nb audio callback from last step
int debug=1;
//int t=0;                // index of track



int repeat=0;
int quit=0;             // do we need to quit ?
int cursor=0;           // cursor position in a sequencer track



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

int bank=0;
int bank_inc=0;
int bank_to_load=0;

//int start_key=0;        // start key pressed ?
//int step=0;             // current step in the sequencer

int menu_cursor=GLOBALMENU_AD;      // index int the menu
int menu=MENU_ON_PAGE1;             // menu mode
int menu_note=ENABLE;
int menu_ad=MENU_AD_AMP_ATTACK_RELEASE;
int menu_osc=MENU_OSC_OSC1OSC2;
int menu_vco=MENU_VCO_OSCMIX_PHASE;
int menu_lfo=MENU_LFO_LFOPITCH;
int menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;
int menu_fx=MENU_FX_DEPTH_SPEED;
int menu_ls=MENU_LS_PATTERN;

int menu_ad_dirty_keyboard=0;

//int ct=0;               // current_track
//int ct_x=0;             
//int ct_y=0;


int dirty_graphic=1;           // 1 : the UI need to be updated

int seq_update_by_step_next=0; // 0 : the UI audio and seq are sync
                               // 1 : the audio callback increment the variable
                               //   : the sequencer is notified by this shared variable,
                               //   : other stuff need to be updated 
                               //   : seq_update_by_step(), load,save...

int current_swing=50;  
//int current_swing=64;

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
  SAMM.setMachineType(P[trackNumber].getPatternElement(stepNumber).get(MACHINE_TYPE));	  
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

  int  swing_sevenbit=(current_swing*127)/100;
  int  swing_inv_sevenbit=127-swing_sevenbit;
  int  step=SEQ.getPatternSequencer(0).getStepWithoutDivider();
  if (step%2)
    {
      //printf("odd\n");
      AE.setNbTickBeforeStepChange((nb_tick_before_step_change*swing_inv_sevenbit)/64);
    }
  else
    {
      //printf("even\n");
     AE.setNbTickBeforeStepChange((nb_tick_before_step_change*swing_sevenbit)/64);
    }
}

void refresh_bpm()
{
  float nb_tick_before_step_change_f;
  //nb_cb_ch_step=(60*DEFAULTFREQ)/(BUFFER_FRAME*4*bpm_current);
  nb_tick_before_step_change_f=(60*DEFAULTFREQ)/(bpm_current*4);
  //nb_tick_before_step_change=(60*DEFAULTFREQ)/(bpm_current*4);
  nb_tick_before_step_change=nb_tick_before_step_change_f;
  refresh_swing();
}

//char * tmp_str;

void display_board_trig()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // Cursor & step postion      
  SG.drawBoxNumber(cursor,CURSOR_COLOR);
  SG.drawBoxNumber(step,STEP_COLOR);  
  for (i=0;i<16;i++)
    {	  // Draw trigged box trig color   
      if (P[cty].getPatternElement(i).get(NOTE_ON))
	{
	  SG.drawBoxNumber(i,TRIG_COLOR);
	  if (i==cursor)
	    SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  if (i==step)
	    SG.drawBoxNumber(step,STEP_COLOR);  
	}
    }  
}



void display_board_two_param(int machineParam1,int machineParam2)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // Cursor & step postion      
  SG.drawBoxNumber(cursor,CURSOR_COLOR);
  SG.drawBoxNumber(step,STEP_COLOR);  
  for (i=0;i<16;i++)
    {	  // Draw trigged box trig color   
      if (P[cty].getPatternElement(i).get(NOTE_ON))
	{
	  SG.drawBoxNumber(i,TRIG_COLOR);
	  if (i==cursor)
	    SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  if (i==step)
	    SG.drawBoxNumber(step,STEP_COLOR);  
	  SG.smallBoxNumber(i,P[cty].getPatternElement(i).get(machineParam1),128,SMALLBOX_COLOR);
	  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).get(machineParam2),SMALLBOX_COLOR);
	}
    }  
}


void display_board_one_param_text(int machineParam1)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  const char * space="    ";

  int  size_of_zero=5;
  char line1[size_of_zero+1];

  char * line1_to_process;
  int    line1_size;


  // Cursor & step postion      
  SG.drawBoxNumber(cursor,CURSOR_COLOR);
  SG.drawBoxNumber(step,STEP_COLOR);  
  for (i=0;i<16;i++)
    {	  // Draw trigged box trig color   
      if (P[cty].getPatternElement(i).get(NOTE_ON))
	{
	  SG.drawBoxNumber(i,TRIG_COLOR);
	  if (i==cursor)
	    SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  if (i==step)
	    SG.drawBoxNumber(step,STEP_COLOR);  

	  update_SAMM(cty,i);
	  strcpy(line1,space);
	  line1_to_process=SAM->getMachineParamCharStar(machineParam1,P[cty].getPatternElement(i).get(machineParam1));

	  line1_size=strlen(line1_to_process);

	  if (line1_size>size_of_zero)
	    strncpy(line1,line1_to_process,size_of_zero);
	  else
	    strncpy(line1,line1_to_process,line1_size);
	  line1[size_of_zero]='\0';

	  SG.drawTTFTextNumberFirstLine(i,line1);	  

	  //	  SG.drawTTFTextNumberFirstLine(i,SAM->getMachineParamCharStar(machineParam1,P[cty].getPatternElement(i).get(machineParam1)));	  

	}
    }  
}


void display_board_two_param_text(int machineParam1,int machineParam2)
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

  // Cursor & step postion      
  SG.drawBoxNumber(cursor,CURSOR_COLOR);
  SG.drawBoxNumber(step,STEP_COLOR);  
  for (i=0;i<16;i++)
    {	  // Draw trigged box trig color   
      if (P[cty].getPatternElement(i).get(NOTE_ON))
	{
	  SG.drawBoxNumber(i,TRIG_COLOR);
	  if (i==cursor)
	    SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  if (i==step)
	    SG.drawBoxNumber(step,STEP_COLOR);  

	  update_SAMM(cty,i);
	  strcpy(line1,space);
	  line1_to_process=SAM->getMachineParamCharStar(machineParam1,P[cty].getPatternElement(i).get(machineParam1));
	  line2_to_process=SAM->getMachineParamCharStar(machineParam2,P[cty].getPatternElement(i).get(machineParam2));

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

	  // line1[size_of_zero]='\0';
	  // line2[size_of_zero]='\0';

	  SG.drawTTFTextNumberFirstLine( i,line1);	  
	  SG.drawTTFTextNumberSecondLine(i,line2);

	}
    }  
}

void display_board_one_and_two_param_text(int machineParam1,int machineParam2)
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  int    size_of_zero=5;
  char   line1[size_of_zero];
  char   line2[size_of_zero];

  char * line1_to_process;
  char * line2_to_process;
  int    line1_size;
  int    line2_size;

  // Cursor & step postion      
  SG.drawBoxNumber(cursor,CURSOR_COLOR);
  SG.drawBoxNumber(step,STEP_COLOR);  
  for (i=0;i<16;i++)
    {	  // Draw trigged box trig color   
      if (P[cty].getPatternElement(i).get(NOTE_ON))
	{
	  SG.drawBoxNumber(i,TRIG_COLOR);
	  if (i==cursor)
	    SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  if (i==step)
	    SG.drawBoxNumber(step,STEP_COLOR);  

	  update_SAMM(cty,i);

	  line1_to_process=SAM->getMachineParamCharStar(machineParam1,
							P[cty].getPatternElement(i).get(machineParam1));

	  line2_to_process=SAM->getMachineTwoParamCharStar(machineParam2,
							   P[cty].getPatternElement(i).get(machineParam1),
							   P[cty].getPatternElement(i).get(machineParam2));

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

	}
    }  
}



void display_board_amp_env()
{
  UI->display_board(GLOBALMENU_AD);
}

void display_board_note()
{
  UI->display_board(GLOBALMENU_NOTE);
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
      display_board_trig();
    }
  
}

void display_board_bank()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  //int  bank=PR.getBank();




  char str_bank[16];
  char str_song[16];


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
     DPRINTF("HIT\n");
      SG.clearScreen();
      
      sprintf(str_bank,"Current Bank %d ",bank);

      SG.guiTTFText(30,10, str_bank);

      sprintf(str_bank,"Bank to load %d ",bank_to_load);
      SG.guiTTFText(30,30, str_bank);
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


  char str_bank[16];
  char str_song[16];

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
    "240-255" 
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


  if (menu         ==  MENU_OFF        && 
      menu_cursor  ==  GLOBALMENU_LS   && 
      menu_ls      ==  MENU_LS_PATTERN
      )
    {
     DPRINTF("HIT\n");
      SG.clearScreen();
      
      sprintf(str_bank,"Bank %d ",bank);

      SG.guiTTFText(30,10, str_bank);
      SG.guiTTFText(30,30,txt_pattern_modulo_sixteen_slot[loadsave_cursor_x_div_sixteen]);


      sprintf(str_song,"SongPosition %d ",SEQ.getSongSequencer().getStep());

      SG.guiTTFText(30,(SCREEN_HEIGHT/2)*SCREEN_MULT+0, str_song);
      SG.guiTTFText(30,(SCREEN_HEIGHT/2)*SCREEN_MULT+20,txt_pattern_modulo_sixteen_slot[song_cursor_x_div_sixteen]);

      sprintf(str_song,"LoopA %d ",SEQ.getSongSequencer().getLoopA());
      SG.guiTTFText((SCREEN_WIDTH/2)*SCREEN_MULT,(SCREEN_HEIGHT/2)*SCREEN_MULT+0, str_song);

      sprintf(str_song,"LoopB %d ",SEQ.getSongSequencer().getLoopB());
      SG.guiTTFText((SCREEN_WIDTH/2)*SCREEN_MULT,(SCREEN_HEIGHT/2)*SCREEN_MULT+20,str_song);



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
		  SG.middleBoxNumberUp(x%16,y,BOX_COLOR);
		else
		  SG.middleBoxNumberUp(x%16,y,NOTE_COLOR);
	      }
	    else
	      SG.middleBoxNumberUp(x%16,y,STEP_COLOR);
	  }

      // Display box song
      for (x=0;x<16;x++)
	for (y=0;y<TRACK_MAX;y++)
	  {
	    //SG.middleBoxNumberDown(x%16,y,NOTE_COLOR);
	    //if (SEQ.getSongSequencer().getPatternNumberAtCursorPosition(i))
	    //SG.middleBoxNumberDown(x%16,y,NOTE_COLOR);
	    //else
	    SG.middleBoxNumberDown(x%16,y,STEP_COLOR);
	  }



      // we are in song mode
      // Display the current song position by a vertical bar
      for (y=0;y<TRACK_MAX;y++)
	{
	  if (SEQ.getSongSequencer().getStep()/16 == song_cursor_x_div_sixteen)
	    //if (loadsave_cursor_mode==CURSOR_SONG)
	    SG.middleBoxNumberDown(SEQ.getSongSequencer().getStep()%16,
				   y,
				   NOTE_COLOR);
	}




      // we are in loadsave mode
      // Display your current position
      if (loadsave_cursor_mode==CURSOR_LOADSAVE)
	SG.middleBoxNumberUp(loadsave_cursor.x%16,
			     loadsave_cursor.y,
			     TRIG_COLOR);

      // we are in song mode
      // Display your current position
      if (loadsave_cursor_mode==CURSOR_SONG)
	SG.middleBoxNumberDown(song_cursor.x%16,
			       song_cursor.y,
			       TRIG_COLOR);

      
      
      // txt in loadsave box 
      // Display text 00..09..FF
      for (x=loadsave_cursor_x_divmul_sixteen;
	   x<(loadsave_cursor_x_divmul_sixteen)+16;
	   x++)
	for (y=0;y<TRACK_MAX;y++)
	  SG.drawTTFTextLoadSaveBoxNumberUp(x%16,y,txt_tab[x]);
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
      display_board_trig();
    }                  
}



void display_board_psh()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();


  // need to display something, so why not ?
  if (menu!=MENU_OFF && 
      menu_cursor==GLOBALMENU_PSH
      )
    {
      display_board_trig();
    }                  
}





void display_board_mac()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

    if (menu_cursor==GLOBALMENU_MAC)
    {
      for (i=0;i<16;i++)
	{
	  if (P[cty].getPatternElement(i).get(NOTE_ON))
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)       SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)         SG.drawBoxNumber(step,STEP_COLOR);  
	      SG.drawTTFTextNumberFirstLine(i, P[cty].getPatternElement(i).getMachineTypeCharStar());
	    }
	  else
	    {
	      if (i==cursor)SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)  SG.drawBoxNumber(step,STEP_COLOR);  
	    }

	}
    }
}



void display_board_text_global()
{
  int  i=0;
  char str_line1[64]="";
  char str_line2[64]="";
  char str_line3[64]="";
  char str_line4[64]="";
  char str_line5[64]="";
  char str_menu[64]="";

  int  right_x_display_offset=      200*SCREEN_MULT;
  int  right_y_display_offset_line1= 20*SCREEN_MULT;
  int  right_y_display_offset_line2= 40*SCREEN_MULT;
  int  right_y_display_offset_line3= 60*SCREEN_MULT;
  int  right_y_display_offset_line4= 80*SCREEN_MULT;
  int  right_y_display_offset_line5=100*SCREEN_MULT;

  int  menu_x_display_offset=       10*SCREEN_MULT;
  int  menu_y_display_offset=      200*SCREEN_MULT;

  int  cty=SEQ.getCurrentTrackY();
  int  stepdiv=SEQ.getPatternSequencer(cty).getBPMDivider();


  sprintf(str_line1,    "Track/%d ",cty);

  sprintf(str_line3,    "Div  /%d",stepdiv);

  if (menu_note==ENABLE &&
      menu_cursor==GLOBALMENU_NOTE)
      sprintf(str_line2,"NOTE");

  if (menu_note==DISABLE &&
      menu_cursor==GLOBALMENU_NOTE)
      sprintf(str_line2,"DOT");

  if (menu_cursor==GLOBALMENU_BPM)
    sprintf(str_line2,  "BPM %.1f SWING %d",bpm_current,current_swing);



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


void display_board()
{
  int  i;
  char str_up[64];
  char str_down[64];
  char str_divider[64];
  char str_submenu[64];

  int  right_x_display_offset=      200*SCREEN_MULT;
  int  right_y_display_offset_line1=20*SCREEN_MULT;
  int  right_y_display_offset_line2=40*SCREEN_MULT;
  int  right_y_display_offset_line3=60*SCREEN_MULT;
  int  right_y_display_offset_line4=80*SCREEN_MULT;

  int  menu_x_display_offset=       10*SCREEN_MULT;
  int  menu_y_display_offset=      200*SCREEN_MULT;
  //int  

  int  cty=SEQ.getCurrentTrackY();
  int  stepdiv=SEQ.getPatternSequencer(cty).getBPMDivider();
  dirty_graphic=0;

  SG.clearScreen();

  display_board_text_global();
  UI->display_board_text();

  for (i=0;i<SEQ.getPatternSequencer(cty).getPatternLenght();i++)
    { SG.drawBoxNumber(i,BOX_COLOR); }
  for (i=i;i<16;i++)
    { SG.drawBoxNumber(i,DISABLEDBOX_COLOR); }
     

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

void display_refresh()
{
  SG.refresh();
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
     DPRINTF("key lalt\n");      
      dirty_graphic=1;
      IE.clearLastKeyEvent();
    }  
}

void handle_key_menu()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  int last_menu=0xffff;
  int last_menu_cursor=0xffff;
  int menu_switching=0;         // when 1 we will switch from one menu to one another

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


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
      
      // We exit the LS Menu so we had to 
      // sync SEQ.Y with the cursor in the load save screen
      // save the song to a file
      if (last_menu        == MENU_OFF &&
	  menu             != MENU_OFF &&
	  menu_cursor      == GLOBALMENU_LS)
	{
	  SEQ.setCurrentTrackY(loadsave_cursor.y);
	  PR.saveSong(SEQ.getSongSequencer());
	 DPRINTF("SAVING SONG TO FILE\n");
	}
      //printf("HERE IAMn");
      dirty_graphic=1;
      IE.clearLastKeyEvent();
     DPRINTF("[gmenu : %d cmenu : %d]\n",menu,menu_cursor);
    }

  //We enter the LS Menu so we had to sync the cursor with SEQ 
  if (lastKey      ==  BUTTON_B       && 
      lastEvent    ==  KEYRELEASED    &&
      (menu        ==  MENU_ON_PAGE1  ||
       menu        ==  MENU_ON_PAGE2))
    {
      last_menu        = menu;
      menu=MENU_OFF;
      menu_switching=1;
      loadsave_cursor.y=SEQ.getCurrentTrackY();
      dirty_graphic=1;
      IE.clearLastKeyEvent();
     DPRINTF("[gmenu : %d cmenu : %d]\n",menu,menu_cursor);
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
      if(keyState[BUTTON_LEFT]) 
	{
	  if (keyRepeat[BUTTON_LEFT]    == 1 || 
	      keyRepeat[BUTTON_LEFT]%64 == 0)
	    {
	      menu_cursor--;
	      menu_switching=1;
	    }
	  //	  if (menu_cursor<GLOBALMENU_AD  && menu==MENU_ON_PAGE1) menu_cursor=GLOBALMENU_VCO;
	  //	  if (menu_cursor<GLOBALMENU_OSC && menu==MENU_ON_PAGE2) menu_cursor=GLOBALMENU_BPM;
	  if (menu_cursor<0     && menu==MENU_ON_PAGE1) menu_cursor=5;
	  if (menu_cursor<6     && menu==MENU_ON_PAGE2) menu_cursor=11;

	  dirty_graphic=1;
	 DPRINTF("\t\t[menu_cursor:%d]\n",menu_cursor);
	 DPRINTF("key left\n");            
	}      
      
      if(keyState[BUTTON_RIGHT])
	{
	  if (keyRepeat[BUTTON_RIGHT]    == 1 || 
	      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG == 0)
	    {
	      menu_cursor++;
	      menu_switching=1;
	    }
	  //if (menu_cursor>GLOBALMENU_VCO  && menu==MENU_ON_PAGE1) menu_cursor=GLOBALMENU_AD;
	  //if (menu_cursor>GLOBALMENU_BPM  && menu==MENU_ON_PAGE2) menu_cursor=GLOBALMENU_OSC;
	  if (menu_cursor>5      && menu==MENU_ON_PAGE1) { menu_cursor=0; 	      menu_switching=1; }
	  if (menu_cursor>11     && menu==MENU_ON_PAGE2) { menu_cursor=6;             menu_switching=1; }

	  dirty_graphic=1;
	 DPRINTF("\t\t[menu_cursor:%d]\n",menu_cursor);
	 DPRINTF("key right\n");            
	}
      
      if(keyState[BUTTON_UP])
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
	 DPRINTF("[key up : change track : %d] \n",SEQ.getCurrentTrackY());
	  dirty_graphic=1;
	}

      if(keyState[BUTTON_DOWN])
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
	 DPRINTF("[key down : change track : %d] \n",SEQ.getCurrentTrackY());
	  dirty_graphic=1;
	}
    }

  if (menu_switching>0)
    {
     DPRINTF("Switching back to all default menu\n");
      menu_note=ENABLE;
      menu_ad=0;
      menu_osc=0;
      menu_vco=0;
      menu_lfo=0;
      menu_fltr=0;
      menu_fx=0;
    }


}


void handle_key_sixteenbox()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  int          cty=SEQ.getCurrentTrackY();

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

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
      !keyState[BUTTON_START]               
      )
    {                 

      if(keyState[BUTTON_UP] && 
	 !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_UP]   ==1 || 
	      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)	    
	    cursor=cursor-4;
	  if (cursor < 0) cursor=cursor +16;
	 DPRINTF("key down : up \n");
	  dirty_graphic=1;
	}
      
      if(keyState[BUTTON_DOWN] && 
	 !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor=( cursor+4 ) %16;
	 DPRINTF("key down : down\n");
	  dirty_graphic=1;
	}
      
      if(keyState[BUTTON_LEFT] && 
	 !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_LEFT]==1 || 
	      keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor--;
	  
	  if (cursor<0) cursor=15;
	 DPRINTF("key left\n");            
	  dirty_graphic=1;
	}
      
      if (keyState[BUTTON_RIGHT] && 
	  !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_RIGHT]   ==1 || 
	      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor++;
	  if (cursor>15) cursor=0;
	 DPRINTF("key right\n");      
	  dirty_graphic=1;
	}
    }
}


int handle_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all)
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  if (keyState[buttonPressed]  && keyState[buttonKeyRepeat])
    if (keyRepeat[buttonKeyRepeat]==1 ||  keyRepeat[buttonKeyRepeat]%repeatInterval==0)
      { 
	if (all==0)
	  { TK.set(machineParam,paramValue);    dirty_graphic=1; }
	if (all==1)
	  { TK.setAll(machineParam,paramValue); dirty_graphic=1; }
      }
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
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_FX
  // change fx_depth and fx_speed
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_FX)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FX1_DEPTH      ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FX1_DEPTH      ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FX1_SPEED         ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FX1_SPEED         ,  -1, 0);
    }

  // GLOBALMENU_FX
  // change fx_depth and fx_speed
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_FX)
    {
      // Insert/Remove Trig
      //sub_handle_invert_trig();

      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FX1_DEPTH      ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FX1_DEPTH      ,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FX1_SPEED         ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FX1_SPEED         ,  -1, 1);

    }

}

void handle_key_vco()
{
    UI->handle_key(GLOBALMENU_VCO);
}


void handle_key_mac()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_MAC
  // change machine type
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_MAC )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     MACHINE_TYPE        ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     MACHINE_TYPE        ,     -1, 0);      
    }

  // GLOBALMENU_MAC
  // change machine type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_MAC )
    {

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     MACHINE_TYPE        ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     MACHINE_TYPE        ,     -1, 1);
    }

}

void handle_key_lfo()
{
  UI->handle_key(GLOBALMENU_LFO);  
}



void handle_key_psh()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();



  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_PSH
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , PATTERN_SHIFT,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , PATTERN_SHIFT,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , PATTERN_LENGTH ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , PATTERN_LENGTH ,  -1, 1);
    }

}



void handle_key_fltr()
{
  UI->handle_key(GLOBALMENU_FLTR);  
}


void handle_key_bpm()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_BPM
  // change bpm speed
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {
      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , BPM ,             -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , BPM ,              1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , BPM            ,  10, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , BPM            , -10, 0);
    }  

  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {
      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , BPM_DIVIDER    ,   1, 0);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , BPM_DIVIDER    ,  -1, 0);
    }  


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , SWING    ,  -1, 0);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , SWING    ,   1, 0);
    }  


}

void handle_key_load_save()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


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



      if (menu                 == MENU_OFF    && 
	  loadsave_cursor_mode == CURSOR_SONG &&
	  keyState[BUTTON_B] )
	{
	  if (keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==1)
	    pattern_song_inc=-1;
	  if (keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==1)	
	    pattern_song_inc=1;
	}


      if (menu                 == MENU_OFF && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
	  keyState[BUTTON_B])
	{
	  if (keyState[BUTTON_DOWN])
	    { save=true; dirty_graphic=1; }
	  if (keyState[BUTTON_UP])	
	    load=true;
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
	  
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { loadsave_cursor.y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { loadsave_cursor.y++;  dirty_graphic=1;}
	  
	  if (loadsave_cursor.x>MAX_PATTERN_BY_PROJECT-1)        { loadsave_cursor.x=0;                           }
	  if (loadsave_cursor.x<0)                               { loadsave_cursor.x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (loadsave_cursor.y>TRACK_MAX-1)                     { loadsave_cursor.y=0;                           }
	  if (loadsave_cursor.y<0)                               { loadsave_cursor.y=TRACK_MAX-1;                 }  
	  
	  SEQ.setCurrentTrackY(loadsave_cursor.y);
	}

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
	  
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { song_cursor.y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor.y++;  dirty_graphic=1;}
	  
	  if (song_cursor.x>MAX_PATTERN_BY_PROJECT-1)            { song_cursor.x=0;                           }
	  if (song_cursor.x<0)                                   { song_cursor.x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (song_cursor.y>TRACK_MAX-1)                         { song_cursor.y=0;                           }
	  if (song_cursor.y<0)                                   { song_cursor.y=TRACK_MAX-1;                 }  
	  
	  //SEQ.setCurrentTrackY(loadsave_cursor_y);
	}


      if (menu                 == MENU_OFF     && 	  
	  loadsave_cursor_mode == CURSOR_SONG  &&
	  (
	   keyState[BUTTON_B]
	   ))
	{
	  /*
	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1  || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_x--;  dirty_graphic=1;}
	  */
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { song_cursor.x++; dirty_graphic=1; SEQ.getSongSequencer().setPatternNumber(song_cursor.x,song_cursor.y,SEQ.getSongSequencer().getPatternNumber(song_cursor.x-1,song_cursor.y));}

	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { song_cursor.x--; dirty_graphic=1; SEQ.getSongSequencer().setPatternNumber(song_cursor.x,song_cursor.y,SEQ.getSongSequencer().getPatternNumber(song_cursor.x+1,song_cursor.y));}

	  /*
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { song_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_y++;  dirty_graphic=1;}
	  */
	  if (song_cursor.x>MAX_PATTERN_BY_PROJECT-1)            { song_cursor.x=0;                           }
	  if (song_cursor.x<0)                                   { song_cursor.x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (song_cursor.y>TRACK_MAX-1)                         { song_cursor.y=0;                           }
	  if (song_cursor.y<0)                                   { song_cursor.y=TRACK_MAX-1;                 }  
	  
	  //SEQ.setCurrentTrackY(loadsave_cursor_y);
	}


    }

}


void handle_key_bank()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


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
  PECursor=P[cty].getPatternElement(i);  
  if (PECursor.get(MACHINE_TYPE)==SYNTH_PICOSYNTH)    { UI=&PSUI; return ;   }
  if (PECursor.get(MACHINE_TYPE)==SYNTH_PICODRUM)     { UI=&PDUI; return ;   }
  if (PECursor.get(MACHINE_TYPE)==SYNTH_OPL2    )     { UI=&DBUI; return ;   }
  if (PECursor.get(MACHINE_TYPE)==SYNTH_PBSYNTH)      { UI=&PBUI; return ;   }
#ifdef __FPU__
  if (PECursor.get(MACHINE_TYPE)==SYNTH_CURSYNTH)     { UI=&CSUI;   return ; }
  if (PECursor.get(MACHINE_TYPE)==SYNTH_OPEN303)      { UI=&O303UI; return ; }
  if (PECursor.get(MACHINE_TYPE)==SYNTH_TWYTCHSYNTH)  { UI=&TWUI;   return ; }
  if (PECursor.get(MACHINE_TYPE)==SYNTH_MDADRUM)      { UI=&MDUI;   return ; }
#endif
#ifdef __RTMIDI__
  if (PECursor.get(MACHINE_TYPE)==SYNTH_MIDIOUT)      { UI=&MIDIUI; return ; }
#endif
  
  printf("NO UI found\n");
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
  int  cursor_not_on=0;
  // Refresh the PECursor with the current Element
  // if (P[cty].getPatternElement(cursor).get(NOTE_ON))
  //   {
  //     refresh_pecursor_ui(cursor);
  //   }
  // else
  //   {
  for (i=0;i<=16;i++)
    {
      if (P[cty].getPatternElement(i).get(NOTE_ON))
	cursor_not_on=i;
    }
  refresh_pecursor_ui(cursor_not_on);
  //}
}




void handle_key()
{

  int  cty=SEQ.getCurrentTrackY();
  
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  IE.handleKey();

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  //#ifdef OPENDINGUX
  if (keyState[BUTTON_SELECT] && keyState[BUTTON_DOWN])
    if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0)
      {
	AudioMixer & am=AE.getAudioMixer();
	am.setAudioVolume(am.getAudioVolume()-1);
	IE.clearLastKeyEvent();
	return;
      }
  if (keyState[BUTTON_SELECT] && keyState[BUTTON_UP])
    if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0)
      {
	AudioMixer & am=AE.getAudioMixer();
	am.setAudioVolume(am.getAudioVolume()+1);
	IE.clearLastKeyEvent();
	return;
      }
  //  #endif


  if (IE.shouldExit())
    quit=1;
    //exit(0);
  
  //if (start_key==2) 
  //printf("%d %d %d\n",lastKey,lastEvent,lastKey==&& BUTTON_START && lastEvent==KEYRELEASED);
  // DPRINTF("lastevent=%d\n",lastEvent);


  handle_key_menu();
  handle_key_sixteenbox();


  //refresh_pecursor();


  if (menu_cursor==GLOBALMENU_AD)   
    { handle_key_amp_env(); } //handle_key_submenu_ad(); }
  if (menu_cursor==GLOBALMENU_NOTE) 
    { handle_key_note();                             }
  if (menu_cursor==GLOBALMENU_OSC)  
    { handle_key_osc();                              }
  if (menu_cursor==GLOBALMENU_VCO)  
    { handle_key_vco(); } //handle_key_submenu_vco();    }
  if (menu_cursor==GLOBALMENU_LFO)  
    { handle_key_lfo(); } //handle_key_submenu_lfo();    }
  if (menu_cursor==GLOBALMENU_FLTR) 
    { handle_key_fltr(); } //handle_key_submenu_fltr();  }

  if (menu_cursor==GLOBALMENU_LS)   handle_key_load_save();
  if (menu_cursor==GLOBALMENU_BANK) handle_key_bank();
  if (menu_cursor==GLOBALMENU_PSH)  handle_key_psh();
  if (menu_cursor==GLOBALMENU_MAC)  handle_key_mac();
  if (menu_cursor==GLOBALMENU_FX)   handle_key_fx();
  if (menu_cursor==GLOBALMENU_BPM)  handle_key_bpm();
  
 
}

void seq_update_tweakable_knob_one(int machineParam)
{
  int          cty=SEQ.getCurrentTrackY();
  if (TK.get(machineParam)!=0)
    {
      update_SAMM(cty,cursor);
      P[cty].getPatternElement(cursor).set(machineParam,
					   SAM->checkI(machineParam,P[cty].getPatternElement(cursor).get(machineParam)+TK.get(machineParam)));
      //SAM->checkI(machineParam,P[cty].getPatternElement(cursor).get(AMP)+TK.get(machineParam)));
      TK.set(machineParam,0);
      if (debug)
	printf("[param:%d:%d]\n",machineParam,P[cty].getPatternElement(cursor).get(machineParam));
    }  
}

void seq_update_tweakable_knob_all(int machineParam)
{
  int          cty=SEQ.getCurrentTrackY();
  int          i;
  
  if (TK.getAll(machineParam)!=0)
    {
      for (i=0;i<16;i++)
	{
	  update_SAMM(cty,i);
	  P[cty].getPatternElement(i).set(machineParam,
					  SAM->checkI(machineParam,P[cty].getPatternElement(i).get(machineParam)+TK.getAll(machineParam)));
					  //P[cty].getPatternElement(i).get(machineParam)+TK.getAll(machineParam));
					  //SAM->checkI(machineParam,P[cty].getPatternElement(i).get(AMP)+TK.getAll(machineParam)));
	}
      
      TK.setAll(machineParam,0);
      if (debug)
	printf("[paramAll:%d:%d]\n",machineParam,P[cty].getPatternElement(cursor).get(machineParam));
    }  
}



void seq_update_multiple_time_by_step()
{
  AudioMixer & am=AE.getAudioMixer();
  int          cty=SEQ.getCurrentTrackY();
  int          ctx=SEQ.getCurrentTrackX();
  int          step=SEQ.getPatternSequencer(cty).getStep();
  int          oldstep=0;
  int          i=0;
  int          t=0;



  // Change amp Amplification
  seq_update_tweakable_knob_one(AMP);
  seq_update_tweakable_knob_all(AMP);

  // Change amp env Attack
  seq_update_tweakable_knob_one(ADSR_AMP_ATTACK);
  seq_update_tweakable_knob_all(ADSR_AMP_ATTACK);

  // Change amp env Decay
  seq_update_tweakable_knob_one(ADSR_AMP_DECAY);
  seq_update_tweakable_knob_all(ADSR_AMP_DECAY);

  // Change amp env Sustain
  seq_update_tweakable_knob_one(ADSR_AMP_SUSTAIN);
  seq_update_tweakable_knob_all(ADSR_AMP_SUSTAIN);  
  
  // Change amp env Release
  seq_update_tweakable_knob_one(ADSR_AMP_RELEASE);
  seq_update_tweakable_knob_all(ADSR_AMP_RELEASE);

  // Change fltr env Attack
  seq_update_tweakable_knob_one(ADSR_FLTR_ATTACK);
  seq_update_tweakable_knob_all(ADSR_FLTR_ATTACK);

  // Change fltr env Decay
  seq_update_tweakable_knob_one(ADSR_FLTR_DECAY);
  seq_update_tweakable_knob_all(ADSR_FLTR_DECAY);

  // Change fltr env Sustain
  seq_update_tweakable_knob_one(ADSR_FLTR_SUSTAIN);
  seq_update_tweakable_knob_all(ADSR_FLTR_SUSTAIN);

  // Change fltr env Release
  seq_update_tweakable_knob_one(ADSR_FLTR_RELEASE);
  seq_update_tweakable_knob_all(ADSR_FLTR_RELEASE);
  
  // Change fltr type
  seq_update_tweakable_knob_one(FILTER1_TYPE);
  seq_update_tweakable_knob_all(FILTER1_TYPE);

  // Change fltr env Attack
  seq_update_tweakable_knob_one(FILTER1_ALGO);
  seq_update_tweakable_knob_all(FILTER1_ALGO);

  // Change VCOMix
  seq_update_tweakable_knob_one(VCO_MIX);
  seq_update_tweakable_knob_all(VCO_MIX);

  // Change Fx Depth
  seq_update_tweakable_knob_one(FX1_DEPTH);
  seq_update_tweakable_knob_all(FX1_DEPTH);

  // Change Fx Speed
  seq_update_tweakable_knob_one(FX1_SPEED);
  seq_update_tweakable_knob_all(FX1_SPEED);

  // Change osc1 amp
  seq_update_tweakable_knob_one(OSC1_AMP);
  seq_update_tweakable_knob_all(OSC1_AMP);

  // Change osc2 amp
  seq_update_tweakable_knob_one(OSC2_AMP);
  seq_update_tweakable_knob_all(OSC2_AMP);

  // Change osc3 amp
  seq_update_tweakable_knob_one(OSC3_AMP);
  seq_update_tweakable_knob_all(OSC3_AMP);

  // Change osc4 amp
  seq_update_tweakable_knob_one(OSC4_AMP);
  seq_update_tweakable_knob_all(OSC4_AMP);


  // Change osc1 unison
  seq_update_tweakable_knob_one(OSC1_UNISON);
  seq_update_tweakable_knob_all(OSC1_UNISON);

  // Change osc2 unison
  seq_update_tweakable_knob_one(OSC2_UNISON);
  seq_update_tweakable_knob_all(OSC2_UNISON);

  // Change osc1 unison detune
  seq_update_tweakable_knob_one(OSC1_UNISONDT);
  seq_update_tweakable_knob_all(OSC1_UNISONDT);

  // Change osc1 unison detune
  seq_update_tweakable_knob_one(OSC2_UNISONDT);
  seq_update_tweakable_knob_all(OSC2_UNISONDT);





  // Change trig time
  seq_update_tweakable_knob_one(TRIG_TIME_DURATION);
  seq_update_tweakable_knob_all(TRIG_TIME_DURATION);

  // // Change phase osc1
  seq_update_tweakable_knob_one(OSC1_PHASE);
  seq_update_tweakable_knob_all(OSC1_PHASE);

  // Change osc1 detune
  seq_update_tweakable_knob_one(OSC1_DETUNE);
  seq_update_tweakable_knob_all(OSC1_DETUNE);



  // Change lfo depth
  seq_update_tweakable_knob_one(LFO1_DEPTH);
  seq_update_tweakable_knob_all(LFO1_DEPTH);

  // Change lfo depth
  seq_update_tweakable_knob_one(LFO2_DEPTH);
  seq_update_tweakable_knob_all(LFO2_DEPTH);


  // Change lfo1 freq
  seq_update_tweakable_knob_one(LFO1_FREQ);
  seq_update_tweakable_knob_all(LFO1_FREQ);

  // Change lfo1 freq
  seq_update_tweakable_knob_one(LFO2_FREQ);
  seq_update_tweakable_knob_all(LFO2_FREQ);


  // Change pitchbend depth
  seq_update_tweakable_knob_one(PITCHBEND_DEPTH);
  seq_update_tweakable_knob_all(PITCHBEND_DEPTH);

  // Change pb speed
  seq_update_tweakable_knob_one(PITCHBEND_SPEED);
  seq_update_tweakable_knob_all(PITCHBEND_SPEED);

  // Change lfo type
  seq_update_tweakable_knob_one(LFO_TYPE);
  seq_update_tweakable_knob_all(LFO_TYPE);
  
  // Change filter cutoff
  seq_update_tweakable_knob_one(FILTER1_CUTOFF);
  seq_update_tweakable_knob_all(FILTER1_CUTOFF);
 
  // Change filter resonance
  seq_update_tweakable_knob_one(FILTER1_RESONANCE);
  seq_update_tweakable_knob_all(FILTER1_RESONANCE);


  // Change filter saturation in twytch
  seq_update_tweakable_knob_one(FILTER1_SATURATION);
  seq_update_tweakable_knob_all(FILTER1_SATURATION);
 
  // Change osc feedback filter in twytch
  seq_update_tweakable_knob_one(FILTER1_FEEDBACK);
  seq_update_tweakable_knob_all(FILTER1_FEEDBACK);

  
  // Change oscillator one
  seq_update_tweakable_knob_one(OSC1_TYPE);
  seq_update_tweakable_knob_all(OSC1_TYPE);
  
  // Change oscillator two
  seq_update_tweakable_knob_one(OSC2_TYPE);
  seq_update_tweakable_knob_all(OSC2_TYPE);

  // Change oscillator three ( alias sub  in Twytch )
  seq_update_tweakable_knob_one(OSC3_TYPE);
  seq_update_tweakable_knob_all(OSC3_TYPE);

 
  // Change oscillator one
  seq_update_tweakable_knob_one(MACHINE_TYPE);
  seq_update_tweakable_knob_all(MACHINE_TYPE);
   
  // Change Note
  seq_update_tweakable_knob_one(NOTE1);
  seq_update_tweakable_knob_all(NOTE1);

  // Change FM
  seq_update_tweakable_knob_one(FM_TYPE);
  seq_update_tweakable_knob_all(FM_TYPE);

  // change LFO1 WAVEFORM
  seq_update_tweakable_knob_one(LFO1_WAVEFORM);
  seq_update_tweakable_knob_all(LFO1_WAVEFORM);

  // change LFO2 WAVEFORM
  seq_update_tweakable_knob_one(LFO2_WAVEFORM);
  seq_update_tweakable_knob_all(LFO2_WAVEFORM);

  // change VELOCITY
  seq_update_tweakable_knob_one(VELOCITY);
  seq_update_tweakable_knob_all(VELOCITY);

  // change OSC1_MOD
  seq_update_tweakable_knob_one(OSC1_MOD);
  seq_update_tweakable_knob_all(OSC1_MOD);


  // change LFO1 WAVEFORM
  seq_update_tweakable_knob_one(LFO1_ENV_AMOUNT);
  seq_update_tweakable_knob_all(LFO1_ENV_AMOUNT);

  // change LFO2 WAVEFORM
  seq_update_tweakable_knob_one(LFO2_ENV_AMOUNT);
  seq_update_tweakable_knob_all(LFO2_ENV_AMOUNT);


  if (TK.get(BPM_DIVIDER)!=0)
    {	  
      if (TK.get(BPM_DIVIDER)>0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()*2);
      if (TK.get(BPM_DIVIDER)<0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()/2);
      TK.set(BPM_DIVIDER,0);
      P[cty].setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider());
    }


  // Change PatternLength
  if (TK.getAll(PATTERN_LENGTH)!=0)
    { 
      SEQ.getPatternSequencer(cty).setPatternLenght(SEQ.getPatternSequencer(cty).getPatternLenght()+TK.getAll(PATTERN_LENGTH));
      TK.setAll(PATTERN_LENGTH,0);
      P[cty].setSize(SEQ.getPatternSequencer(cty).getPatternLenght());
    }

  // Change PatternLength
  if (TK.get(PATTERN_LENGTH)!=0)
    { 
      SEQ.getPatternSequencer(cty).setPatternLenght(SEQ.getPatternSequencer(cty).getPatternLenght()+TK.get(PATTERN_LENGTH));
      TK.set(PATTERN_LENGTH,0);
      P[cty].setSize(SEQ.getPatternSequencer(cty).getPatternLenght());
    }



  // shift PatternEntry
  if (TK.getAll(PATTERN_SHIFT)!=0)
    {       
      PatternElement Pe;

      if (TK.getAll(PATTERN_SHIFT)<0)
	{
	  Pe=P[cty].getPatternElement(0);
	  for (i=0;i<15;i++)
	    {	    
	      P[cty].getPatternElement(i)=P[cty].getPatternElement(i+1);	    
	    }
	  i++;
	  P[cty].getPatternElement(15)=Pe;	    
	}

      if (TK.getAll(PATTERN_SHIFT)>0)
	{
	  Pe=P[cty].getPatternElement(15);
	  for (i=15;i>0;i--)
	    {	    
	      P[cty].getPatternElement(i)=P[cty].getPatternElement(i-1);	    
	    }
	  i--;
	  P[cty].getPatternElement(0)=Pe;	    
	}

      TK.setAll(PATTERN_SHIFT,0);
      dirty_graphic=1;
     DPRINTF("[pshift_all:left]\n");	  
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
      bpm_diff=bpm_diff/10;
      //save the bpm in the 
      //change the number of time AudioEngine need to be trigged
      //to effectively change pattern step
     DPRINTF("Recalculate BPM\n");
     bpm_current=P[cty].getBPM();
      for(t=0;t<TRACK_MAX;t++)	    
	P[t].setBPM(bpm_current+bpm_diff);
      
      //bpm_current=bpm_current+TK.get(BPM);
      bpm_current=bpm_current+bpm_diff;
      TK.set(BPM,0);
      refresh_bpm();
      
    }
  
  if (TK.get(SWING)!=0)
    {
      //save the swing in the Pattern
      //change the number of time AudioEngine need to be trigged
      //to effectively change pattern step
     DPRINTF("Recalculate Swing\n");
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
      if (P[cty].getPatternElement(cursor).get(NOTE_ON))
	{
	  P[cty].getPatternElement(cursor).set(NOTE_ON,(P[cty].getPatternElement(cursor).get(NOTE_ON)+1)%2);
	  PE=P[cty].getPatternElement(cursor);
	}
      else
	{
	  P[cty].setPatternElement(cursor,PE);
	  P[cty].getPatternElement(cursor).set(NOTE_ON,1);
	  if (P[cty].getPatternElement(cursor).get(NOTE1)==0)
	    {
	      //P[cty].getPatternElement(cursor).setAttack_amp(0);
	      //P[cty].getPatternElement(cursor).setRelease_amp(64);
	      //P[cty].getPatternElement(cursor).setNote(37);
	      P[cty].getPatternElement(cursor).set(NOTE1,25);
	    }
	}
      TK.set(INVERT_TRIG,0);
    }
  
}



int seq_update_by_step()
{
  int  cty=SEQ.getCurrentTrackY();
  int  ctx=SEQ.getCurrentTrackX();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  int  t;

  //SEQ.getPatternSequencer(0).setStepDivider(4);

  if (pattern_song_inc!=0)
    {

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
	      PR.readPatternData(SEQ.getSongSequencer().getPatternNumber(song_cursor.x,t),t,P[t]);
	      //SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
	    }
	  else
	    {
	      P[t].init();
	    }
	}
	    

    }

  if (pattern_song_loop!=0)
    {

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
     DPRINTF("<==[SAVE]==>\n");
      //PR.writePattern(1,ct+1,P[ct]);
      PR.writePattern(loadsave_cursor.x,loadsave_cursor.y,P[cty]);
      load_save_highligth_current[loadsave_cursor.y]=loadsave_cursor.x;
      dirty_graphic=1;
      save=false;
    }

  // Load save only on pattern change
  if (load)
    {
     DPRINTF("<==[LOAD]==>\n");
      if (PR.PatternDataExist(loadsave_cursor.x,loadsave_cursor.y)==true)
	{
	  PR.readPatternData(loadsave_cursor.x,loadsave_cursor.y,P[cty]);
	  load_save_highligth_current[loadsave_cursor.y]=loadsave_cursor.x;

	  // Don't update BPM and Swing on single pattern load
	  // Do it only on "loadall"

	  SEQ.getPatternSequencer(cty).setPatternLenght(P[cty].getSize());
	  SEQ.getPatternSequencer(cty).setBPMDivider(P[cty].getBPMDivider());
	  P[cty].setSize(SEQ.getPatternSequencer(cty).getPatternLenght());

	  refresh_bpm();

	}
      else
	P[cty].init();
      load=false;
    }


  // Load save only on pattern change
  if (loadall)
    {
     DPRINTF("<==[LOAD_ALL]==>\n");
      for (t=0;t<TRACK_MAX;t++)
	{
	  if (PR.PatternDataExist(loadsave_cursor.x,t)==true)
	    {
	      PR.readPatternData(loadsave_cursor.x,t,P[t]);
	      load_save_highligth_current[t]=loadsave_cursor.x;
	      bpm_current=P[t].getBPM();
	      current_swing=P[t].getSwing();

	      SEQ.getPatternSequencer(t).setPatternLenght(P[t].getSize());
	      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
	      P[t].setSize(SEQ.getPatternSequencer(t).getPatternLenght());

	      refresh_bpm();
	    }
	  else
	    {
	      P[t].init();
	      load_save_highligth_current[t]=-10;
	    }
	}
      loadall=false;
    }


  // Load save only on pattern change
  if (saveall)
    {
     DPRINTF("<==[SAVE_ALL]==>\n");
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
     DPRINTF("<==[REMOVE]==>\n");
      if (PR.PatternDataExist(loadsave_cursor.x,loadsave_cursor.y)==true)
	{
	  if (PR.PatternRemove(loadsave_cursor.x,loadsave_cursor.y))
	    {
	     DPRINTF("<==[REMOVE==Success]==>\n");
	      dirty_graphic=1;
	    }
	}
      else
	{
	  P[cty].init();
	 DPRINTF("<==[Remove==Failed]==>\n");
	}
      patternRemove=false;
    }

    
  //if (step==16) 
  //    step=0; 

  if (debug)
   DPRINTF("STEP:%d",step);	  
  
}




void seq_update_track(int t)
{
  //int  cty=SEQ.getCurrentTrackY();
  //int  ctx=SEQ.getCurrentTrackX();
  //int  step=SEQ.getPatternSequencer(cty).getStep();
  int  step=SEQ.getPatternSequencer(t).getStep();

  // Handle only modification of on next step value 
  // for each Machine 
  // of each track 
  //for (t=0;t<TRACK_MAX;t++)
  //{
  if (P[t].getPatternElement(step).get(NOTE_ON)!=0)
	{
	  MM[t]->setAmplitude(P[t].getPatternElement(step).get(AMP));
	  MM[t]->setMachineType(P[t].getPatternElement(step).get(MACHINE_TYPE));
	  M[t]  = MM[t]->getInput();
	  FX[t] = MM[t]->getEffect();


	  FX[t]->setDepth(P[t].getPatternElement(step).get(FX1_DEPTH));
	  FX[t]->setSpeed(P[t].getPatternElement(step).get(FX1_SPEED));

	  //printf("[Freq:%d]\n",i);
	  //M[t]->set(OSC1_FREQ,i);
	  M[t]->reset();
	  //M[t]->setF(OSC1_FREQ,f);
	  M[t]->setI(NOTE1,P[t].getPatternElement(step).get(NOTE1));

	  /*
	  noteOffTrigger[t]=
	    P[t].getPatternElement(step).getAttack_amp()+
	    P[t].getPatternElement(step).getAttack_fltr()
	    ;
	  */
	  //noteOffTrigger[t]=P[t].getPatternElement(step).getTrigTime()/8;

	  // Let the Machine handle NoteOn/NoteOff with a trig time duration
	  M[t]->setI(TRIG_TIME_MODE,1);
	  M[t]->setI(TRIG_TIME_DURATION,P[t].getPatternElement(step).get(TRIG_TIME_DURATION));
	  M[t]->setI(BPM,bpm_current);

	  M[t]->setI(FM_TYPE,P[t].getPatternElement(step).get(FM_TYPE));

	  M[t]->setI(ADSR_ENV0_ATTACK,  P[t].getPatternElement(step).get(ADSR_AMP_ATTACK));
	  M[t]->setI(ADSR_ENV0_DECAY,   P[t].getPatternElement(step).get(ADSR_AMP_DECAY));
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


	  M[t]->setI(OSC1_DETUNE,      P[t].getPatternElement(step).get(OSC1_DETUNE));

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



// This callback run in a really high priority thread
// No IO or syscall is allowed to prevent audio drifting/garbage
// EX : printf      forbiden
//      read(fd...) forbiden
void seq_callback_update_step()
{
  int i;
  int oldstep;
  int songSequencerHasInc=0;


  //for ( i=0;i<10;i++)
  //  {
  //SEQ.getSongSequencer().setPatternNumberAtCursorPosition(i,0,i);
  //}

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
		    PR.readPatternData(SEQ.getSongSequencer().getPatternNumberAtCursorPosition(i),i,P[i]);
		    SEQ.getPatternSequencer(i).setBPMDivider(P[i].getBPMDivider());
		  }
		else
		  P[i].init();
	     
	      //if (i==TRACK_MAX-1)
	      //{
	      //printf("SongSequencer.cursorPosition:%d\n",SEQ.getSongSequencer().getStep());
	      //}
	    }
	  
	  // Update the Machine for the new step
	  seq_update_track(i);	  
	}
    }
  dirty_graphic=1;
  seq_update_by_step_next=1;
  refresh_bpm();
}


int seq()
{
  AudioMixer & am=AE.getAudioMixer();
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
  am.setAudioVolume(DEFAULT_VOLUME);


  dirty_graphic=1;

 DPRINTF("Now in PatternPlayer::seq()\n");

  // Initialize 
  for (t=0;t<TRACK_MAX;t++)
    {

      MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer();
      MM[t]->init();
      MM[t]->setAmplitude(0);  // set the amplitude to 0 to avoid trashing speakers

      
      M[t]=MM[t]->getInput();                             
      M[t]->init();
    }


  // Init all track to the current step, step0 in this particular case 
  for (i=0;i<TRACK_MAX;i++)
    seq_update_track(i);

  refresh_pecursor();

  DPRINTF("openAudio start streaming\n");
  AE.startAudio();
  //AE.startAudioSdl();



  //AE.startAudio();
  seq_update_by_step_next=1;
  seq_update_by_step();  
  while (true)
    {
      //dirty_graphic=1;
      //SDL_LockAudio();
      //display_board();
      
      //SDL_UnlockAudio();
      //seq_update_by_step_next=1;
      cty=SEQ.getCurrentTrackY();
      ctx=SEQ.getCurrentTrackX();

      SDL_LockAudio();
      handle_key();
      SDL_UnlockAudio();

      //printf("sleeping %dms\n",delay);
      SDL_Delay(delay);  

      // A/D, Note, VCO, BPM, more handling...
      // apply the modification done by the user on the gui
      seq_update_multiple_time_by_step();

      
#ifdef PSP
      running = isRunning();
#endif

#if defined(__RTMIDI__)
      MidiOutSystem & MOS=MidiOutSystem::getInstance();
      if (MOS.msgSize())
       	MOS.flushMsg();
#endif
      // if user want to quit via handle_key
      if (quit ||
	  running<=0)
	{
	 DPRINTF("user want to quit\n");
	  return(0);
	}
      
      //display graphic if something has change : handle_key 
      //if (dirty_graphic)
      //display_board();

      nbcb=AE.getNbCallback();

      if (nbcb>last_nbcb)
	{
	  last_nbcb=nbcb;
	}


      if (dirty_graphic)
	{
	  //SDL_LockAudio();
	  //sceKernelDcacheWritebackAll(); 
	  refresh_pecursor();
	  display_board();
	  
	  SDL_LockAudio();
	  SG.refresh();
	  SDL_UnlockAudio();
	  dirty_graphic=0;
	}

      // change step in the pattern
      //if (nbcb-last_nbcb_ch_step>nb_cb_ch_step)
      if (seq_update_by_step_next)
	{	 
	  //last_nbcb_ch_step=nbcb;
	  seq_update_by_step();
	  seq_update_by_step_next=0;
	}
    }
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

  

  
  // WT = new WaveTable();
  // WT->setSize(WAVETABLE_SIZE);
  // G.noise();
  // memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  // WTM.insert(WT,PICO_WAVETABLE_NOISE);
  
 DPRINTF("wavetablemanager.getSize : %d\n",WTM.getSize());

}




int main(int argc,char **argv)
{
  int i;
  int cpu_speed;
  int running=1;

  UI=&PSUI;
  //UI->handle_key(1,2);

#ifdef __RTMIDI__
  MidiOutSystem & MOS=MidiOutSystem::getInstance();
  MOS.init();
  //MOS.chooseMidiPort("TiMidity 128:0");
  MOS.chooseMidiPort("UM-1SX 24:0");
  
#endif



#ifdef PSP
  running = isRunning();
  setupExitCallback();
  cpu_speed=scePowerGetCpuClockFrequencyInt();
 DPRINTF("CURRENT PSP CPU SPEED:%d\n",cpu_speed);
  //cpu_speed=333;
  //scePowerSetCpuClockFrequency(cpu_speed);
  //scePowerSetClockFrequency(333, 333, 166);
  //scePowerSetClockFrequency(233, 233, 133);
  //scePowerSetClockFrequency(300, 300, 150);
  scePowerSetClockFrequency(266, 266, 133);
  cpu_speed=scePowerGetCpuClockFrequencyInt();
 DPRINTF("NEW PSP CPU SPEED:%d\n",cpu_speed);
#else
  running=1; // if we are not on psp, running should be 1  
#endif


  wtg();
  //  exit(0);
  //NF.init();

  NoteFreq & NF = NoteFreq::getInstance();
  NF.init();
  TK.init();

  IE.init();
  IE.printState();
  //IE.handle_key();
  //IE.init();
  //IE.printState();
  //exit(0);
  //for (i=0;i<TRACK_MAX;i++)
  //noteOffTrigger[i]=0;

  PR.init();         // Init the     storage bank
  PR.setBank(bank);  // The current  storage bank will be 0 PWD/bank/bank%d/

  load_pattern();
  DPRINTF("[openVideo output]\n");
  SG.initVideo();
  //SDL_InitSubSystem(SDL_INIT_AUDIO);
  //handle_key(); 
 //  SDL_EnableKeyRepeat(500,500);
  SG.openBMPFont();
  if (SG.openTTFFont()==false) {DPRINTF("ttf font error\n"); exit(1); }
  SG.loadingScreen();

  //sleep(10);
  //AE.setupSequencerCallback(printme);
  refresh_bpm();
  AE.setupSequencerCallback(seq_callback_update_step);
  AE.openAudio();

  //display_board();

  DPRINTF("[openAudio output]\n");
  //exit(0);

  //AE.openAudioSdl();
  
  seq();

  DPRINTF("[closeAudio output]");
  //AE.stopAudioSdl();
  AE.stopAudio();
  AE.closeAudio();
  
  //AE.closeAudioSdl();

  DPRINTF("[closeVideo output]");
  SG.closeVideo();
  //sleep(10);
  //PE.print();
 DPRINTF("Exiting PatternPlayer");

#ifdef PSP
  sceKernelExitGame();	
#endif

}

