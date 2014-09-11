using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#include "Master.h"
#include "Note.h"
#include "PatternReader.h"
#include "PatternPlayer.h"
#include "AudioEngine.h"
//#include "Wave.h"
#include "MonoMixer.h"
#include "SDL_GUI.h"
#include "InputManager.h"
#include "Machine.h"
#include "Sequencer.h"

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
  GLOBALMENU_AD,   // 0
  GLOBALMENU_NOTE, // 1
  GLOBALMENU_OSC,  // 2 
  GLOBALMENU_VCO,  // 3
  GLOBALMENU_MAC,  // 4

  GLOBALMENU_LS,   // 5
  GLOBALMENU_LFO,  // 6
  GLOBALMENU_FLTR, // 7
  GLOBALMENU_BPM,  // 8
  GLOBALMENU_FX    // 9
};


//menu_ad
enum {
  MENU_AD_AMP_ATTACK_RELEASE_,
  MENU_AD_AMP_DECAY_SUSTAIN,
  MENU_AD_FLTR_ATTACK_RELEASE,
  MENU_AD_FLTR_DECAY_SUSTAIN,
  MENU_AD_TRIGTIME_AMP,
};

enum {
  MENU_FLTR_CUTOFF_RESONANCE,
  MENU_FLTR_ALGO_TYPE, 
};

enum {
  MENU_VCO_OSCMIX_PHASE,
  MENU_VCO_OSCAMP
};
  

vector <Pattern>            P(TRACK_MAX);  
vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);

Sequencer      SEQ;         // used to  store/get information about sequencer 
AudioEngine    AE;          // used to  init alsa/rtaudio
PatternReader  PR;          // used to  read data.pic file
PatternElement PE;          // used for copy paste PatternElement
InputManager   IE;          // used to  fetch key
SDL_GUI        SG;          // used to  open a gui and display stuff
//Wave           cowbell;     // used ?
//Instrument     inst;        // used ?


int save=false;
int load=false;

int saveall=false;
int loadall=false;

int patternRemove=false;

int bpm_current=120;    // current value for the four ( TRACK_MAX ) tracks
int bpm=0;              // change from -10 to +10
int nbcb=0;             // current nb audio callback 
int last_nbcb=0;        // number of occurence of AudioEngine callback before changing step
//int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm); // Weird ?
int nb_cb_ch_step;              //=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm_current);
int nb_tick_before_step_change; //=(60*DEFAULT_FREQ)/(bpm_current*4);

int last_nbcb_ch_step=0;// nb audio callback from last step
int debug=1;
int t=0;                // index of track



int repeat=0;
int quit=0;             // do we need to quit ?
int cursor=0;           // cursor position in a sequencer track

int note=0;
int note_all=0;

int amp=0;              // variation of the amp of monomixer
int amp_all=0;              // variation of the amp of monomixer

int attack_amp=0;
int decay_amp=0;
int sustain_amp=0;
int release_amp=0;

int attack_fltr=0;
int decay_fltr=0;
int sustain_fltr=0;
int release_fltr=0;


int attack_amp_all=0;
int decay_amp_all=0;
int sustain_amp_all=0;
int release_amp_all=0;

int attack_fltr_all=0;
int decay_fltr_all=0;
int sustain_fltr_all=0;
int release_fltr_all=0;

//int adsr_note=0;
//int adsr_note_all=0;
int trig_time=0;
int trig_time_all=0;


int lfo_depth=0;
int lfo_depth_all=0;
int lfo_speed=0;
int lfo_speed_all=0;

int cutoff=0;
int resonance=0;

int cutoff_all=0;
int resonance_all=0;

int vcomix=0;
int vcomix_all=0;

int osconetype=0;
int osctwotype=0;

int osconetype_all=0;
int osctwotype_all=0;

int machine_type=0;
int machine_type_all=0;

int phase_osc1=0;
int phase_osc1_all=0;

int filter_algo=0;
int filter_algo_all=0;

int filter_type=0;
int filter_type_all=0;

int osc1_amp=0;
int osc1_amp_all=0;

int osc2_amp=0;
int osc2_amp_all=0;



int loadsave_cursor_x=0; // index in the load/save menu
int loadsave_cursor_y=0; // index in the load/save menu

int start_key=0;        // start key pressed ?
//int step=0;             // current step in the sequencer
int divider=0;           // divider - => /1 /2 /4 /8  ; divider + => /8 /4 /2 /1
int menu_cursor=GLOBALMENU_AD;      // index int the menu
int menu=MENU_ON_PAGE1;             // menu mode
int menu_note=ENABLE;
int menu_ad=MENU_AD_AMP_ATTACK_RELEASE_;
int menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;
int menu_vco=MENU_VCO_OSCMIX_PHASE;

int menu_ad_dirty_keyboard=0;

//int ct=0;               // current_track
int ct_x=0;             
int ct_y=0;
int invert_trig=0;

int dirty_graphic=1;
int seq_update_by_step_next=0;

int current_swing=50;  
//int current_swing=64;
int swing;

int noteOffTrigger[TRACK_MAX];

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

  nb_cb_ch_step=(60*DEFAULTFREQ)/(BUFFER_FRAME*4*bpm_current);
  nb_tick_before_step_change=(60*DEFAULTFREQ)/(bpm_current*4);
  refresh_swing();
}

//char * tmp_str;


void display_board_amp_env()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  // Attack/Release
  if (menu_cursor==GLOBALMENU_AD)
    {
      // Cursor & step postion      
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.drawBoxNumber(step,STEP_COLOR);  
      //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
      
      if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE_)
	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
		  
		  // AdsR
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getRelease_amp(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getAttack_amp(),SMALLBOX_COLOR);      
		}
	    }
	}

      if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)
	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
		  
		  // AdsR
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getDecay_amp(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getSustain_amp(),SMALLBOX_COLOR);      
		}
	    }
	}



      if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE)
	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
		  
		  // AdsR
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getRelease_fltr(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getAttack_fltr(),SMALLBOX_COLOR);      
		}
	    }
	}


      if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN)
	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
		  
		  // AdsR
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getDecay_fltr(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getSustain_fltr(),SMALLBOX_COLOR);      
		}
	    }
	}



      if (menu_ad==MENU_AD_TRIGTIME_AMP)
	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
		  
		  // AdsR
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getAmp(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getTrigTime(),SMALLBOX_COLOR);      
		}
	    }
	}
    }
}


void display_board_note()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  // Note
  if (menu_cursor==GLOBALMENU_NOTE)
    {

      if (menu_note==DISABLE)
	{

	  SG.drawBoxNumber(cursor,CURSOR_COLOR);	  
	  SG.drawBoxNumber(step,STEP_COLOR);  
    
	  /*
	  SG.smallBoxNumber(cursor,
			    (P[cty].getPatternElement(cursor).getNote()%12)*10,
			    (P[cty].getPatternElement(cursor).getNote()/12)*10,
			    SMALLBOX_COLOR);
	  SG.smallBoxNumber(step,  
			    (P[cty].getPatternElement(step).getNote()%12)*10,
			    (P[cty].getPatternElement(step).getNote()/12)*10,
			    SMALLBOX_COLOR);
	  */
	}
      // Note C3 
      if (menu_note==ENABLE)
	{	  
	  SG.drawBoxNumber(step,STEP_COLOR);  
	  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	  SG.drawBoxNumber(cursor,CURSOR_COLOR);

	  for (i=0;i<16;i++)
	    {
	      // Draw trig note color
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,NOTE_COLOR);
		  
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		    //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);
		  SG.drawTTFTextNumberFirstLine(i,P[cty].getPatternElement(i).getNoteCharStar());
		}
	      //      if (P[cty].getPatternElement(i).getTrig())
		
	    }
	}
      // Note Cursor
      if (menu_note==DISABLE)      
	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trig note color
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,NOTE_COLOR);	    
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  

		  //		  if (i==SEQ.getPatternSequencer(cty).getStep())
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  

		  SG.smallBoxNumber(i,
				    (P[cty].getPatternElement(i).getNote()%12)*10,
				    (128-(P[cty].getPatternElement(i).getNote()/12)*10),
				    SMALLBOX_COLOR);

		  
		}
	    }
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

      // Cursor & step postion      
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.drawBoxNumber(step,STEP_COLOR);  
      //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
      
      //if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE_)
      //	{
      for (i=0;i<16;i++)
	{
	  // Draw trigged box trig color   
	  if (P[cty].getPatternElement(i).getTrig())
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	      
		  // LFO
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getLfoDepth(),128,SMALLBOX_COLOR);
	      SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getLfoSpeed(),SMALLBOX_COLOR);
	    }
	}
    }
  
}

void display_board_load_save()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  //int  ctx=SEQ.getCurrentTrackX();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  int loadsave_cursor_x_div_sixteen=loadsave_cursor_x/16;
  //int loadsave_cursor_x_mod_sixteen=loadsave_cursor_x/16;
  //int loadsave_cursor_y_div_sixteen=loadsave_cursor_y/16;

  int loadsave_cursor_x_divmul_sixteen=loadsave_cursor_x_div_sixteen*16;
  //int loadsave_cursor_y_divmul_sixteen=loadsave_cursor_y_divmul_sixteen*16;

  //printf("*************************** loadsave_cursor_x_divmul_sixteen=%d\n",loadsave_cursor_x_divmul_sixteen);
  //printf("*************************** loadsave_cursor_x                %d\n",loadsave_cursor_x);

  static const char * txt_tab[] = 
    { 
    "0","1","2","3",
    "4","5","6","7",
    "8","9","A","B",
    "C","D","E","F" 
    }; 


  if (menu!=MENU_OFF && 
      menu_cursor==GLOBALMENU_LS)
    {

      // Cursor & step postion      
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.drawBoxNumber(step,STEP_COLOR);  
      //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
      
      //if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE_)
      //	{
      for (i=0;i<16;i++)
	{
	  // Draw trigged box trig color   
	  if (P[cty].getPatternElement(i).getTrig())
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	      
		  // LFO
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getLfoDepth(),128,SMALLBOX_COLOR);
	      SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getLfoSpeed(),SMALLBOX_COLOR);
	    }
	}
    }
      
      
      
  if (menu==MENU_OFF && 
      menu_cursor==GLOBALMENU_LS)
    {
      printf("HIT\n");
      //      const char * tmp_txt;

      //tmp_txt="0";
      
      SG.clearScreen();

      // Display box loaded/unloaded
      for (x=loadsave_cursor_x_divmul_sixteen;
	   x<(loadsave_cursor_x_divmul_sixteen)+16;
	   x++)
	for (y=0;y<TRACK_MAX;y++)
	  {
	    if (PR.PatternDataExist(x,y))
	      SG.middleBoxNumber(x%16,y,NOTE_COLOR);
	    else
	      SG.middleBoxNumber(x%16,y,STEP_COLOR);
	  }
      
      // Display your current position
      SG.middleBoxNumber(loadsave_cursor_x%16,
			 loadsave_cursor_y,
			 TRIG_COLOR);
      
      // Display text 0..9..F
      for (x=loadsave_cursor_x_divmul_sixteen;
	   x<(loadsave_cursor_x_divmul_sixteen)+16;
	   x++)
	for (y=0;y<TRACK_MAX;y++)
	  SG.drawTTFTextLoadSaveBoxNumer(x%16,y,txt_tab[x%16]);
      //SG.drawTTFTextLoadSaveBoxNumer(x,y,tmp_txt);
    }
}

void display_board_vco()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCMIX_PHASE)
    {
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.drawBoxNumber(step,STEP_COLOR);        
      for (i=0;i<16;i++)
	{
	  // MIX / PHASE
	  if (P[cty].getPatternElement(i).getTrig())
	    {	      
	      SG.drawBoxNumber(i,NOTE_COLOR);
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  

	      
	      if (i==step)
		SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),128-P[cty].getPatternElement(i).getPhaseOsc1(),STEP_COLOR);
	      if (i==cursor)
		SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),128-P[cty].getPatternElement(i).getPhaseOsc1(),CURSOR_COLOR);

	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),128-P[cty].getPatternElement(i).getPhaseOsc1(),SMALLBOX_COLOR);
	    }

	}
    }

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCAMP)
    {
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.drawBoxNumber(step,STEP_COLOR);        
      for (i=0;i<16;i++)
	{
	  // OSC1AMP OSC2AMP
	  if (P[cty].getPatternElement(i).getTrig())
	    {	      
	      SG.drawBoxNumber(i,NOTE_COLOR);
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  

	      
	      if (i==step)
		SG.smallBoxNumber(i,P[cty].getPatternElement(i).getOsc1Amp(),128-P[cty].getPatternElement(i).getOsc2Amp(),STEP_COLOR);
	      if (i==cursor)
		SG.smallBoxNumber(i,P[cty].getPatternElement(i).getOsc1Amp(),128-P[cty].getPatternElement(i).getOsc2Amp(),CURSOR_COLOR);

	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getOsc1Amp(),128-P[cty].getPatternElement(i).getOsc2Amp(),SMALLBOX_COLOR);
	    }
	}
    }


}


void display_board_lfo()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // LFO

  if (menu_cursor==GLOBALMENU_LFO)
    {
      // Cursor & step postion      
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.drawBoxNumber(step,STEP_COLOR);  
      //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
      
      //if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE_)
      //	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
		  
		  // LFO
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getLfoDepth(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getLfoSpeed(),SMALLBOX_COLOR);
		}
	    }
	  //}
      
    }
}

void display_board_osc()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

    if (menu_cursor==GLOBALMENU_OSC)
    {
      for (i=0;i<16;i++)
	{
	  if (P[cty].getPatternElement(i).getTrig())
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)       SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)         SG.drawBoxNumber(step,STEP_COLOR);  
	      SG.drawTTFTextNumberFirstLine(i, P[cty].getPatternElement(i).getOscOneTypeCharStar());
	      SG.drawTTFTextNumberSecondLine(i,P[cty].getPatternElement(i).getOscTwoTypeCharStar());
	    }
	  else
	    {
	      if (i==cursor)SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)  SG.drawBoxNumber(step,STEP_COLOR);  
	    }

	}
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
	  if (P[cty].getPatternElement(i).getTrig())
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


void display_board_fltr()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_FLTR)
    {

      if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)
	{
	  // Cursor & step postion      
	  SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  SG.drawBoxNumber(step,STEP_COLOR);  
	  
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  
		  // AdsR
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getResonance(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getCutoff(),SMALLBOX_COLOR);      
		}
	    }
	}
      if (menu_fltr==MENU_FLTR_ALGO_TYPE)
	{
	  // Cursor & step postion      
	  SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  SG.drawBoxNumber(step,STEP_COLOR);  
	  
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).getTrig())
		{
		  SG.drawBoxNumber(i,TRIG_COLOR);
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		  
		  SG.drawTTFTextNumberFirstLine(i,P[cty].getPatternElement(i).getFilterAlgoCharStar()); 
		  SG.drawTTFTextNumberSecondLine(i,P[cty].getPatternElement(i).getFilterTypeCharStar()); 
		}
	    }
	}

    }
}


void display_board()
{
  int  i;
  char str_up[8];
  char str_down[24];
  char str_divider[8];
  char str_submenu[16];

  int  cty=SEQ.getCurrentTrackY();
  int  stepdiv=SEQ.getPatternSequencer(cty).getBPMDivider();
  dirty_graphic=0;

  SG.clearScreen();
  //  sprintf(str,"Track %d ",ct);
  sprintf(str_up,"Track %d ",cty);
  SG.guiTTFText(200,20,str_up);

  sprintf(str_divider,"/%d",stepdiv);
  SG.guiTTFText(200,80,str_divider);

  if (menu_cursor!=GLOBALMENU_AD ||
      menu_cursor!=GLOBALMENU_NOTE)    
    {
      sprintf(str_submenu,"               ");
      SG.guiTTFText(200,60,str_submenu);
    }
  if (menu_note==ENABLE &&
      menu_cursor==GLOBALMENU_NOTE)
    {
      sprintf(str_submenu,"NOTE");
      SG.guiTTFText(200,60,str_submenu);
    }
  if (menu_note==DISABLE &&
      menu_cursor==GLOBALMENU_NOTE)
    {
      sprintf(str_submenu,"DOT");
      SG.guiTTFText(200,60,str_submenu);
    }
  if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE_ &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"AMP  A/R");
      SG.guiTTFText(200,60,str_submenu);
    }

  if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"AMP  D/S");
      SG.guiTTFText(200,60,str_submenu);
    }


  if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"FLTR A/R");
      SG.guiTTFText(200,60,str_submenu);
    }

  if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"FLTR D/S");
      SG.guiTTFText(200,60,str_submenu);
    }


  if (menu_ad==MENU_AD_TRIGTIME_AMP &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"T/N AMP");
      SG.guiTTFText(200,60,str_submenu);
    }

  if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE &&
      menu_cursor==GLOBALMENU_FLTR)
    {
      sprintf(str_submenu,"CUTOFF/RES");
      SG.guiTTFText(200,60,str_submenu);
    }

  if (menu_fltr==MENU_FLTR_ALGO_TYPE &&
      menu_cursor==GLOBALMENU_FLTR
      )
    {
      sprintf(str_submenu,"ALGO/TYPE");
      SG.guiTTFText(200,60,str_submenu);
    }


  if (menu_vco==MENU_VCO_OSCAMP &&
      menu_cursor==GLOBALMENU_VCO
      )
    {
      sprintf(str_submenu,"OP1AMP/AP2AMP");
      SG.guiTTFText(200,60,str_submenu);
    }

  if (menu_vco==MENU_VCO_OSCMIX_PHASE &&
      menu_cursor==GLOBALMENU_VCO
      )
    {
      sprintf(str_submenu,"OP1Mult/OP2Mult");
      SG.guiTTFText(200,60,str_submenu);
    }



  /*
  if (menu_ad==MENU_AD_ATTACK_AMP &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"ATTACK/AMP");
      SG.guiTTFText(200,60,str_submenu);
    }
  */
  if (menu_cursor==GLOBALMENU_BPM)
    {
      //sprintf(str_submenu,"SWING %d",(current_swing*100)/127);
      sprintf(str_submenu,"SWING %d",current_swing);
      SG.guiTTFText(200,60,str_submenu);
    }

//  sprintf(str_down,"[A/D] Note L/S",cty);
  
  //  printf("           AD:%d FX:%d\n",AD,FX);
  //  exit(0);

  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_AD)    sprintf(str_down,"[A/R] Note  OSC   VCO   MAC ",cty);  
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_NOTE)  sprintf(str_down," A/R [Note] OSC   VCO   MAC ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_OSC)   sprintf(str_down," A/R  Note [OSC]  VCO   MAC ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_VCO)   sprintf(str_down," A/R  Note  OSC  [VCO]  MAC ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_MAC)   sprintf(str_down," A/R  Note  OSC   VCO  [MAC]",cty);

  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_LS)    sprintf(str_down,"[L/S] LFO   FLTR  BPM   FX ",cty);  
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_LFO)   sprintf(str_down," L/S [LFO]  FLTR  BPM   FX ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_FLTR)  sprintf(str_down," L/S  LFO  [FLTR] BPM   FX ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_BPM)   sprintf(str_down," L/S  LFO   FLTR [BPM]  FX ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_FX)    sprintf(str_down," L/S  LFO   FLTR  BPM  [FX]",cty);

  if (menu==0)                         sprintf(str_down,"                     ",cty);

  if (menu_cursor==GLOBALMENU_AD)               sprintf(str_up,"A/R   ");
  if (menu_cursor==GLOBALMENU_NOTE)             sprintf(str_up,"Note  ");
  if (menu_cursor==GLOBALMENU_OSC)              sprintf(str_up,"OSC   ");
  if (menu_cursor==GLOBALMENU_VCO)              sprintf(str_up,"VCO   ");
  if (menu_cursor==GLOBALMENU_MAC)              sprintf(str_up,"MAC   ");

  if (menu_cursor==GLOBALMENU_LS)               sprintf(str_up,"L/S   ");
  if (menu_cursor==GLOBALMENU_LFO)              sprintf(str_up,"LFO   ");
  if (menu_cursor==GLOBALMENU_FLTR)             sprintf(str_up,"FLTR  ");
  if (menu_cursor==GLOBALMENU_BPM)              sprintf(str_up,"BPM %d",bpm_current);
  if (menu_cursor==GLOBALMENU_FX)               sprintf(str_up,"FX    ");




  SG.guiTTFText(200,40,str_up);
  SG.guiTTFText(10,200,str_down);

  // Draw all box default color   
  for (i=0;i<16;i++)
    { SG.drawBoxNumber(i,BOX_COLOR); }
     

  display_board_amp_env();
  display_board_note();
  display_board_osc();
  display_board_vco();
  display_board_mac();

  display_board_load_save();
  display_board_lfo();
  display_board_fltr();
  display_board_bpm();

  SG.refresh();
}


void sub_handle_invert_trig()
{
  int    lastEvent;
  int    lastKey;

  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  if (lastKey   == BUTTON_A && 
      lastEvent == SDL_KEYDOWN)
    {
      invert_trig=1;
      printf("key lalt\n");      
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

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  // Enter and cycle thru menu pages
  if (lastKey   ==  BUTTON_SELECT && 
      lastEvent ==  SDL_KEYUP)
    {
      switch (menu)
	{
	case MENU_OFF:      
	  if (menu_cursor<5) 
	    menu=MENU_ON_PAGE1;
	  if (menu_cursor>=5)
	    menu=MENU_ON_PAGE2;
	  break;

	case MENU_ON_PAGE1: 
	  menu=MENU_ON_PAGE2;
	  if (menu_cursor<5)
	    menu_cursor+=5; 
	  else
	    menu_cursor=5;
	  break;

	case MENU_ON_PAGE2: 
	  menu=MENU_ON_PAGE1; 
	  if (menu_cursor>=5)
	    menu_cursor-=5; 
	  else
	    menu_cursor=0;
	  break;

	  //	case MENU_ON_PAGE1: menu=MENU_OFF;      break;
	  //case MENU_ON_PAGE1: menu=MENU_ON_PAGE2; menu_cursor=+4; break;
	  //case MENU_ON_PAGE2: menu=MENU_ON_PAGE1; menu_cursor=-4; break;
	}
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      printf("[gmenu : %d cmenu : %d]\n",menu,menu_cursor);
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
	    menu_cursor--;
	  //	  if (menu_cursor<GLOBALMENU_AD  && menu==MENU_ON_PAGE1) menu_cursor=GLOBALMENU_VCO;
	  //	  if (menu_cursor<GLOBALMENU_OSC && menu==MENU_ON_PAGE2) menu_cursor=GLOBALMENU_BPM;
	  if (menu_cursor<0     && menu==MENU_ON_PAGE1) menu_cursor=4;
	  if (menu_cursor<5     && menu==MENU_ON_PAGE2) menu_cursor=9;

	  dirty_graphic=1;
	  printf("\t\t[menu_cursor:%d]\n",menu_cursor);
	  printf("key left\n");            
	}      
      
      if(keyState[BUTTON_RIGHT])
	{
	  if (keyRepeat[BUTTON_RIGHT]    == 1 || 
	      keyRepeat[BUTTON_RIGHT]%64 == 0)
	    menu_cursor++;
	  //if (menu_cursor>GLOBALMENU_VCO  && menu==MENU_ON_PAGE1) menu_cursor=GLOBALMENU_AD;
	  //if (menu_cursor>GLOBALMENU_BPM  && menu==MENU_ON_PAGE2) menu_cursor=GLOBALMENU_OSC;
	  if (menu_cursor>4      && menu==MENU_ON_PAGE1) menu_cursor=0;
	  if (menu_cursor>9      && menu==MENU_ON_PAGE2) menu_cursor=5;

	  dirty_graphic=1;
	  printf("\t\t[menu_cursor:%d]\n",menu_cursor);
	  printf("key right\n");            
	}
      
      if(keyState[BUTTON_UP])
	{
	  if (keyRepeat[BUTTON_UP]    == 1 || 
	      keyRepeat[BUTTON_UP]%64 == 0)
	    {
	      if (SEQ.getCurrentTrackY()>0)
		SEQ.setCurrentTrackY(SEQ.getCurrentTrackY()-1);
	      else 
		SEQ.setCurrentTrackY(TRACK_MAX-1);
	    }
	  printf("[key up : change track : %d] \n",SEQ.getCurrentTrackY());
	  dirty_graphic=1;
	}

      if(keyState[BUTTON_DOWN])
	{
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%64==0)
	    {
	      if (SEQ.getCurrentTrackY()<TRACK_MAX-1)
		SEQ.setCurrentTrackY(SEQ.getCurrentTrackY()+1);
	      else 
		SEQ.setCurrentTrackY(0);	      
	    }
	  printf("[key down : change track : %d] \n",SEQ.getCurrentTrackY());
	  dirty_graphic=1;
	}
    }

  //leave menu 
  if (lastKey      ==  BUTTON_B       && 
      lastEvent    ==  SDL_KEYUP      &&
      (menu        ==  MENU_ON_PAGE1  ||
       menu        ==  MENU_ON_PAGE2))
    menu=MENU_OFF;



}


void handle_key_sixteenbox()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

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
	      keyRepeat[BUTTON_UP]%64==0)
	    cursor=cursor-4;
	  if (cursor < 0) cursor=cursor +16;
	  printf("key down : up \n");
	  dirty_graphic=1;
	}
      
      if(keyState[BUTTON_DOWN] && 
	 !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%64==0)
	    cursor=( cursor+4 ) %16;
	  printf("key down : down\n");
	  dirty_graphic=1;
	}
      
      if(keyState[BUTTON_LEFT] && 
	 !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_LEFT]==1 || 
	      keyRepeat[BUTTON_LEFT]%64==0)
	    cursor--;
	  
	  if (cursor<0) cursor=15;
	  printf("key left\n");            
	  dirty_graphic=1;
	}
      
      if (keyState[BUTTON_RIGHT] && 
	  !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_RIGHT]   ==1 || 
	      keyRepeat[BUTTON_RIGHT]%64==0)
	    cursor++;
	  if (cursor>15) cursor=0;
	  printf("key right\n");      
	  dirty_graphic=1;
	}
    }

  
}


void handle_key_amp_env()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad       == MENU_AD_AMP_ATTACK_RELEASE_)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { release_amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { release_amp=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { attack_amp=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { attack_amp=-1; 	  dirty_graphic=1; }
    }  


  // M_DS AMP
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_DECAY_SUSTAIN)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { decay_amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { decay_amp=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { sustain_amp=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { sustain_amp=-1; 	  dirty_graphic=1; }
    }  


  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_ATTACK_RELEASE_)
    {
      //printf("***********************\n");
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { release_amp_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { release_amp_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { attack_amp_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { attack_amp_all=-1; 	  dirty_graphic=1; }
    }  

  // GLOBALMENU_AD AMP
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_DECAY_SUSTAIN)
    {
      //printf("***********************\n");
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { decay_amp_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { decay_amp_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { sustain_amp_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { sustain_amp_all=-1; 	  dirty_graphic=1; }
    }  




  // GLOBALMENU_AD FLTR
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_ATTACK_RELEASE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { release_fltr=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { release_fltr=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { attack_fltr=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { attack_fltr=-1; 	  dirty_graphic=1; }
    }  

  // GLOBALMENU_AD FLTR
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_DECAY_SUSTAIN)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { decay_fltr=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { decay_fltr=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { sustain_fltr=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { sustain_fltr=-1; 	  dirty_graphic=1; }
    }  



  // GLOBALMENU_AD FLTR
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_ATTACK_RELEASE)
    {
      //printf("***********************\n");
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { release_fltr_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { release_fltr_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { attack_fltr_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { attack_fltr_all=-1; 	  dirty_graphic=1; }
    }  

  // GLOBALMENU_AD FLTR
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_DECAY_SUSTAIN)
    {
      //printf("***********************\n");
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { decay_fltr_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { decay_fltr_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { sustain_fltr_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { sustain_fltr_all=-1; 	  dirty_graphic=1; }
    }  




  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      if (lastKey   == BUTTON_A && 
	  lastEvent == SDL_KEYDOWN)
	{
	  invert_trig=1;
	  printf("key lalt\n");      
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	}
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { amp=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { trig_time=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { trig_time=-1; 	  dirty_graphic=1; }
    }  


  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>4) 
	  { amp_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { amp_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { trig_time_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { trig_time_all=-1; 	  dirty_graphic=1; }
    }  




  // change GLOBALMENU_AD SUBMENU
  if (keyState[BUTTON_START]       &&
      keyRepeat[BUTTON_UP]%128==127   &&
      menu_cursor ==  GLOBALMENU_AD)
    {
      menu_ad--;
      if (menu_ad<=-1)
	menu_ad=4;
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      menu_ad_dirty_keyboard=1;
      printf("[sub menu env : %d]\n",menu_ad);
    }

  // change GLOBALMENU_AD SUBMENU
  if (keyState[BUTTON_START] &&
      keyRepeat[BUTTON_DOWN]%128==127  &&
      menu_cursor ==  GLOBALMENU_AD)
    {
      menu_ad++;
      if (menu_ad>=4)
	menu_ad=0;
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      menu_ad_dirty_keyboard=1;
      printf("[sub menu env : %d]\n",menu_ad);
    }

  // change GLOBALMENU_AD SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  SDL_KEYUP     && 
      menu_cursor ==  GLOBALMENU_AD)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_ad==MENU_AD_AMP_ATTACK_RELEASE_)      { menu_ad=MENU_AD_AMP_DECAY_SUSTAIN;       }
	  else if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)       { menu_ad=MENU_AD_FLTR_ATTACK_RELEASE;     }   
	  else if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE)     { menu_ad=MENU_AD_FLTR_DECAY_SUSTAIN;      }   
	  else if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN)      { menu_ad=MENU_AD_TRIGTIME_AMP;        }   
	  else if (menu_ad==MENU_AD_TRIGTIME_AMP)        { menu_ad=MENU_AD_AMP_ATTACK_RELEASE_;      }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu env : %d]\n",menu_ad);
    }


}


void handle_key_note()
{

  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

    // GLOBALMENU_NOTE
  // change note
  // copy/paste
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {
      // copy/paste/insert/delete trig 
      sub_handle_invert_trig();
      /*
      if (lastKey   == BUTTON_A && 
	  lastEvent ==  SDL_KEYUP)
	{
	  invert_trig=1;
	  printf("key lalt\n");      
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	}
      */
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%64==0) 
	{ note=-1; 	  dirty_graphic=1;}

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%64==0) 
	  { note=1;  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%64==0) 
	  { note=12;   	  dirty_graphic=1;}

      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%64==0) 
	{ note=-12;  	  dirty_graphic=1;}
    }  

    // GLOBALMENU_NOTE
  // change note
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {
      
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%64==0) 
	{ note_all=-1; 	  dirty_graphic=1;}

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%64==0) 
	  { note_all=1;  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%64==0) 
	  { note_all=12;   	  dirty_graphic=1;}

      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%64==0) 
	{ note_all=-12;  	  dirty_graphic=1;}
    }  


  // change note from box to value e.g C3 D4...
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  SDL_KEYUP     && 
      menu_cursor ==  GLOBALMENU_NOTE)
    {
      if      (menu_note==0)        { menu_note=1;  }
      else if (menu_note==1)        { menu_note=0;  }   
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      printf("[sub menu note : %d]\n",menu_note);
    }


}

void handle_key_osc()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%128==0) 
	  { osconetype=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%128==0) 
	  { osconetype=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%128==0) 
	  { osctwotype=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%128==0) 
	  { osctwotype=-1;  	  dirty_graphic=1;}
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%128==0) 
	  { osconetype_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%128==0) 
	  { osconetype_all=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%128==0) 
	  { osctwotype_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%128==0) 
	  { osctwotype_all=-1;  	  dirty_graphic=1;}
    }

}


void handle_key_vco()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  SDL_KEYUP     && 
      menu_cursor ==  GLOBALMENU_VCO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_vco==MENU_VCO_OSCMIX_PHASE)        { menu_vco=MENU_VCO_OSCAMP;            }
	  else if (menu_vco==MENU_VCO_OSCAMP)              { menu_vco=MENU_VCO_OSCMIX_PHASE;      }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu env : %d]\n",menu_ad);
    }


  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==0) 
	  { vcomix=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==0) 
	  { vcomix=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==0) 
	  { phase_osc1=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==0) 
	  { phase_osc1=-1;  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==0) 
	  { vcomix_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==0) 
	  { vcomix_all=1;  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==0) 
	  { phase_osc1_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==0) 
	  { phase_osc1_all=-1;  	  dirty_graphic=1;}
      //????
    }


  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSCAMP
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==0) 
	  { osc1_amp=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==0) 
	  { osc1_amp=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==0) 
	  { osc2_amp=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==0) 
	  { osc2_amp=-1;  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCAMP
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==0) 
	  { osc1_amp_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==0) 
	  { osc1_amp_all=1;  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==0) 
	  { osc2_amp_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==0) 
	  { osc2_amp_all=-1;  	  dirty_graphic=1;}
      //????
    }


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

      
      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%128==0) 
	  { machine_type=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%128==0) 
	  { machine_type=-1;  	  dirty_graphic=1;}
    }

  // GLOBALMENU_MAC
  // change machine type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_MAC )
    {
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%128==0) 
	  { machine_type_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%128==0) 
	  { machine_type_all=-1;  	  dirty_graphic=1;}
    }

}


void handle_key_lfo()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO    
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==0) 
	  { lfo_depth=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==0) 
	  { lfo_depth=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==0) 
	  { lfo_speed=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==0) 
	  { lfo_speed=-1;  	  dirty_graphic=1;}
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==0) 
	  { lfo_depth_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==0) 
	  { lfo_depth_all=1;  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==0) 
	  { lfo_speed_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==0) 
	  { lfo_speed_all=-1;  	  dirty_graphic=1;}
      //????
    }

}



void handle_key_fltr()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  // change GLOBALMENU_FLTR SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  SDL_KEYUP     && 
      menu_cursor ==  GLOBALMENU_FLTR)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)   { menu_fltr=MENU_FLTR_ALGO_TYPE;            }
	  else if (menu_fltr==MENU_FLTR_ALGO_TYPE)          { menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;     }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu_fltr : %d]\n",menu_fltr);
    }






  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      sub_handle_invert_trig();

      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>128) 
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==1 )  
	  { resonance=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==1 )  
	  { resonance=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==1 ) 
	  { cutoff=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==1 ) 
	  { cutoff=-1; 	  dirty_graphic=1; }
    }  

  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>128) 
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==1 )  
	  { resonance_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==1 )  
	  { resonance_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==1 ) 
	  { cutoff_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==1 ) 
	  { cutoff_all=-1; 	  dirty_graphic=1; }
    }  

  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      sub_handle_invert_trig();

      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>128) 
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==1 )  
	  { filter_algo=-1;   dirty_graphic=1; }
            if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==1 )  
	  { filter_algo=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==1 ) 
	  { filter_type=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==1 ) 
	  { filter_type=-1; 	  dirty_graphic=1; }
    }  


  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      //sub_handle_invert_trig();

      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>128) 
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==1 )  
	  { filter_algo_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==1 )  
	  { filter_algo_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%4==1 ) 
	  { filter_type_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%4==1 ) 
	  { filter_type_all=-1; 	  dirty_graphic=1; }
    }  


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
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%128==0) 
	  { bpm=-1; 	  dirty_graphic=1; printf("[B+LEFT  t=%d]\n",bpm_current); }

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%128==0) 
	  { bpm=1; 	  dirty_graphic=1; printf("[B+RIGHT t=%d]\n",bpm_current);}

      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%128==0) 
	  { bpm=-10; 	  dirty_graphic=1; printf("[B+DOWN  t=%d]\n",bpm_current); }

      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%128==0) 
	  { bpm=10; 	  dirty_graphic=1; printf("[B+UP    t=%d]\n",bpm_current);}


      //if (bpm < 20) bpm=20;
      //if (bpm > 260) bpm=260;

      //nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm);
    }  

  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {

      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%128==0) 
	  { divider=-1; 	  dirty_graphic=1; printf("[A+DOWN  t=%d] divider\n",divider); }

      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%128==0) 
	  { divider=1; 	  dirty_graphic=1; printf("[A+UP    t=%d] divider\n",divider);}
    }  


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {

      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%128==0)
	  { swing=-1; 	  dirty_graphic=1; printf("[A+DOWN  t=%d] swing\n",current_swing); }

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%128==0) 
	  { swing=1; 	  dirty_graphic=1; printf("[A+UP    t=%d] swing\n",current_swing);}
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


      if (menu        == MENU_OFF && 
	  keyState[BUTTON_B]      &&
	  keyState[BUTTON_A]      &&
	  keyState[BUTTON_DOWN])

	{
	  patternRemove=true;
	  return;
	}


      if (menu        == MENU_OFF && 
	  keyState[BUTTON_B])
	{
	  if (keyState[BUTTON_DOWN])
	    save=true;
	  if (keyState[BUTTON_UP])	
	    load=true;
	}
      
      // GLOBALMENU_LS
      // in the load/save view 
      // move loasavecursor position 
      // Save/load bund of Pattern
      
      if (menu        == MENU_OFF && 
	  keyState[BUTTON_A] )
	{
	  if (keyState[BUTTON_DOWN])
	    saveall=true;
	  if (keyState[BUTTON_UP])	
	    loadall=true;
	}
      
      // GLOBALMENU_LS
      // in the load/save view 
      // move load/save cursor position 
      if (menu        == MENU_OFF && 	  
	  (!(
	     keyState[BUTTON_B] ||
	     keyState[BUTTON_A])))
	{
	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1  || keyRepeat[BUTTON_LEFT]%64==0)  
	      { loadsave_cursor_x--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%64==0) 
	      { loadsave_cursor_x++;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%64==0)    
	      { loadsave_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%64==0)  
	      { loadsave_cursor_y++;  dirty_graphic=1;}
	  
	  if (loadsave_cursor_x>MAX_PATTERN_BY_PROJECT-1)        { loadsave_cursor_x=0;                           }
	  if (loadsave_cursor_x<0)                               { loadsave_cursor_x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (loadsave_cursor_y>TRACK_MAX-1)                     { loadsave_cursor_y=0;                           }
	  if (loadsave_cursor_y<0)                               { loadsave_cursor_y=TRACK_MAX-1;                 }  
	  
	  SEQ.setCurrentTrackY(loadsave_cursor_y);
	}
    }

}


void handle_key()
{
  
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  IE.handleKey();

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  if (IE.shouldExit())
    quit=1;
    //exit(0);
  
  //if (start_key==2) 
  //printf("%d %d %d\n",lastKey,lastEvent,lastKey==&& BUTTON_START && lastEvent==SDL_KEYUP);
  //  printf("lastevent=%d\n",lastEvent);


  handle_key_menu();
  
  handle_key_sixteenbox();

  handle_key_amp_env();
  handle_key_note();
  handle_key_vco();
  handle_key_osc(); 
  handle_key_mac(); 

  handle_key_load_save();
  handle_key_fltr();
  handle_key_lfo();
  handle_key_bpm();
 
  int delay=1;
  //printf("sleeping %dms\n",delay);
  SDL_Delay(delay);  
}


void seq_update_multiple_time_by_step()
{
  AudioMixer & am=AE.getAudioMixer();
  int          cty=SEQ.getCurrentTrackY();
  int          ctx=SEQ.getCurrentTrackX();
  int          step=SEQ.getPatternSequencer(cty).getStep();
  int          oldstep=0;
  int          i=0;

  
  // Change amp Amplification
  if (amp!=0)
    {
      P[cty].getPatternElement(cursor).setAmp(P[cty].getPatternElement(cursor).getAmp()+amp);
      amp=0;
      if (debug)
	printf("[amp:%d]\n",P[cty].getPatternElement(cursor).getAmp());
    }
  
  // Change amp Amplification
  if (amp_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setAmp(P[cty].getPatternElement(i).getAmp()+amp_all);
      amp_all=0;
      if (debug)
	printf("[amp_all:%d]\n",P[cty].getPatternElement(cursor).getAmp());
    }
  
  // Change amp env Attack
  if (attack_amp!=0)
    {
      P[cty].getPatternElement(cursor).setAttack_amp(P[cty].getPatternElement(cursor).getAttack_amp()+attack_amp);
      attack_amp=0;
      if (debug)
	printf("[attack_amp:%d]\n",P[cty].getPatternElement(cursor).getAttack_amp());
    }
  
  // Change amp env Attack
  if (attack_amp_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setAttack_amp(P[cty].getPatternElement(i).getAttack_amp()+attack_amp_all);
      attack_amp_all=0;
      if (debug)
	printf("[attack_all:%d]\n",P[cty].getPatternElement(cursor).getAttack_amp());
    }


  // Change amp env Decay
  if (decay_amp!=0)
    {
      P[cty].getPatternElement(cursor).setDecay_amp(P[cty].getPatternElement(cursor).getDecay_amp()+decay_amp);
      decay_amp=0;
      if (debug)
	printf("[decay_amp:%d]\n",P[cty].getPatternElement(cursor).getDecay_amp());
    }
  
  // Change amp env Decay
  if (decay_amp_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setDecay_amp(P[cty].getPatternElement(i).getDecay_amp()+decay_amp_all);
      decay_amp_all=0;
      if (debug)
	printf("[decay_all:%d]\n",P[cty].getPatternElement(cursor).getDecay_amp());
    }



  // Change amp env Sustain
  if (sustain_amp!=0)
    {
      P[cty].getPatternElement(cursor).setSustain_amp(P[cty].getPatternElement(cursor).getSustain_amp()+sustain_amp);
      sustain_amp=0;
      if (debug)
	printf("[sustain_amp:%d]\n",P[cty].getPatternElement(cursor).getSustain_amp());
    }
  
  // Change amp env Sustain
  if (sustain_amp_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setSustain_amp(P[cty].getPatternElement(i).getSustain_amp()+sustain_amp_all);
      sustain_amp_all=0;
      if (debug)
	printf("[sustain_all:%d]\n",P[cty].getPatternElement(cursor).getSustain_amp());
    }
  
  
  // Change amp env Release
  if (release_amp!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setRelease_amp(P[cty].getPatternElement(cursor).getRelease_amp()+release_amp);
      release_amp=0;
      if (debug)
	printf("[release_amp:%d]\n",P[cty].getPatternElement(cursor).getRelease_amp());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change amp env Release
  if (release_amp_all!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setRelease_amp(P[cty].getPatternElement(i).getRelease_amp()+release_amp_all);
      release_amp_all=0;
      if (debug)
	printf("[release_amp_all:%d]\n",P[cty].getPatternElement(cursor).getRelease_amp());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  




  // Change fltr env Attack
  if (attack_fltr!=0)
    {
      P[cty].getPatternElement(cursor).setAttack_fltr(P[cty].getPatternElement(cursor).getAttack_fltr()+attack_fltr);
      attack_fltr=0;
      if (debug)
	printf("[attack_fltr:%d]\n",P[cty].getPatternElement(cursor).getAttack_fltr());
    }
  
  // Change fltr env Attack
  if (attack_fltr_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setAttack_fltr(P[cty].getPatternElement(i).getAttack_fltr()+attack_fltr_all);
      attack_fltr_all=0;
      if (debug)
	printf("[attack_all:%d]\n",P[cty].getPatternElement(cursor).getAttack_amp());
    }


  // Change fltr env Decay
  if (decay_fltr!=0)
    {
      P[cty].getPatternElement(cursor).setDecay_fltr(P[cty].getPatternElement(cursor).getDecay_fltr()+decay_fltr);
      decay_fltr=0;
      if (debug)
	printf("[decay_fltr:%d]\n",P[cty].getPatternElement(cursor).getDecay_fltr());
    }
  
  // Change fltr env Decay
  if (decay_fltr_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setDecay_fltr(P[cty].getPatternElement(i).getDecay_fltr()+decay_fltr_all);
      decay_fltr_all=0;
      if (debug)
	printf("[decay_all:%d]\n",P[cty].getPatternElement(cursor).getDecay_amp());
    }


  // Change fltr env Sustain
  if (sustain_fltr!=0)
    {
      P[cty].getPatternElement(cursor).setSustain_fltr(P[cty].getPatternElement(cursor).getSustain_fltr()+sustain_fltr);
      sustain_fltr=0;
      if (debug)
	printf("[sustain_fltr:%d]\n",P[cty].getPatternElement(cursor).getSustain_fltr());
    }
  
  // Change fltr env Sustain
  if (sustain_fltr_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setSustain_fltr(P[cty].getPatternElement(i).getSustain_fltr()+sustain_fltr_all);
      sustain_fltr_all=0;
      if (debug)
	printf("[sustain_all:%d]\n",P[cty].getPatternElement(cursor).getSustain_amp());
    }

  
  
  // Change fltr env Release
  if (release_fltr!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setRelease_fltr(P[cty].getPatternElement(cursor).getRelease_fltr()+release_fltr);
      release_fltr=0;
      if (debug)
	printf("[release_fltr:%d]\n",P[cty].getPatternElement(cursor).getRelease_fltr());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change fltr env Release
  if (release_fltr_all!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setRelease_fltr(P[cty].getPatternElement(i).getRelease_fltr()+release_fltr_all);
      release_fltr_all=0;
      if (debug)
	printf("[release_fltr_all:%d]\n",P[cty].getPatternElement(cursor).getRelease_fltr());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  

  // Change fltr env Attack
  if (filter_type!=0)
    {
      P[cty].getPatternElement(cursor).setFilterType(P[cty].getPatternElement(cursor).getFilterType()+filter_type);
      filter_type=0;
      if (debug)
	printf("[filter_type:%d]\n",P[cty].getPatternElement(cursor).getFilterType());
    }
  
  // Change fltr env Attack
  if (filter_type_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setFilterType(P[cty].getPatternElement(i).getFilterType()+filter_type_all);
      filter_type_all=0;
      if (debug)
	printf("[filter_type:%d]\n",P[cty].getPatternElement(cursor).getFilterType());
    }

  // Change fltr env Attack
  if (filter_algo!=0)
    {
      P[cty].getPatternElement(cursor).setFilterAlgo(P[cty].getPatternElement(cursor).getFilterAlgo()+filter_algo);
      filter_algo=0;
      if (debug)
	printf("[filter_algo:%d]\n",P[cty].getPatternElement(cursor).getFilterAlgo());
    }
  
  // Change fltr env Attack
  if (filter_algo_all!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).setFilterAlgo(P[cty].getPatternElement(i).getFilterAlgo()+filter_algo_all);
      filter_algo_all=0;
      if (debug)
	printf("[filter_algo:%d]\n",P[cty].getPatternElement(cursor).getFilterAlgo());
    }




  
  // Change VCOMix
  if (vcomix!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setVCOMix(P[cty].getPatternElement(cursor).getVCOMix()+vcomix);
      vcomix=0;
      if (debug)
	printf("[vcomix:%d]\n",P[cty].getPatternElement(cursor).getVCOMix());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change VCOMix
  if (vcomix_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setVCOMix(P[cty].getPatternElement(i).getVCOMix()+vcomix_all);
      vcomix_all=0;
      if (debug)
	printf("[vcomix_all:%d]\n",P[cty].getPatternElement(cursor).getVCOMix());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }


  // Change osc1 amp
  if (osc1_amp!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setOsc1Amp(P[cty].getPatternElement(cursor).getOsc1Amp()+osc1_amp);
      osc1_amp=0;
      if (debug)
	printf("[osc1_amp:%d]\n",P[cty].getPatternElement(cursor).getOsc1Amp());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change osc1 amp all
  if (osc1_amp_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setOsc1Amp(P[cty].getPatternElement(i).getOsc1Amp()+osc1_amp_all);
      osc1_amp_all=0;
      if (debug)
	printf("[osc1_amp_all:%d]\n",P[cty].getPatternElement(cursor).getOsc1Amp());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }

  // Change osc2 amp
  if (osc2_amp!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setOsc2Amp(P[cty].getPatternElement(cursor).getOsc2Amp()+osc2_amp);
      osc2_amp=0;
      if (debug)
	printf("[osc2_amp:%d]\n",P[cty].getPatternElement(cursor).getOsc2Amp());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change osc2 amp all
  if (osc2_amp_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setOsc2Amp(P[cty].getPatternElement(i).getOsc2Amp()+osc2_amp_all);
      osc2_amp_all=0;
      if (debug)
	printf("[osc2_amp_all:%d]\n",P[cty].getPatternElement(cursor).getOsc2Amp());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }





  // Change trig time
  if (trig_time!=0)
    {
      P[cty].getPatternElement(cursor).setTrigTime(P[cty].getPatternElement(cursor).getTrigTime()+trig_time);
      trig_time=0;
      if (debug)
	printf("[trig_time:%d]\n",P[cty].getPatternElement(cursor).getTrigTime());      
    }
  
  // Change all trig time
  if (trig_time_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setTrigTime(P[cty].getPatternElement(i).getTrigTime()+trig_time_all);
      trig_time_all=0;
      if (debug)
	printf("[trig_time_all:%d]\n",P[cty].getPatternElement(cursor).getTrigTime());      
    }




  // Change phase osc1
  if (phase_osc1!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setPhaseOsc1(P[cty].getPatternElement(cursor).getPhaseOsc1()+phase_osc1);
      phase_osc1=0;
      if (debug)
	printf("[phase_osc1:%d]\n",P[cty].getPatternElement(cursor).getPhaseOsc1());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }

  // Change phase osc1
  if (phase_osc1_all!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setPhaseOsc1(P[cty].getPatternElement(i).getPhaseOsc1()+phase_osc1_all);
      phase_osc1_all=0;
      if (debug)
	printf("[phase_osc1_all:%d]\n",P[cty].getPatternElement(cursor).getPhaseOsc1());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }



  // Change lfo depth
  if (lfo_depth!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setLfoDepth(P[cty].getPatternElement(cursor).getLfoDepth()+lfo_depth);
      lfo_depth=0;
      if (debug)
	printf("[lfo_depth:%d]\n",P[cty].getPatternElement(cursor).getLfoDepth());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }


  // Change lfo depth
  if (lfo_depth_all!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setLfoDepth(P[cty].getPatternElement(i).getLfoDepth()+lfo_depth_all);
      lfo_depth_all=0;
      if (debug)
	printf("[lfo_depth_all:%d]\n",P[cty].getPatternElement(cursor).getLfoDepth());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }


  // Change lfo speed
  if (lfo_speed!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).setLfoSpeed(P[cty].getPatternElement(cursor).getLfoSpeed()+lfo_speed);
      lfo_speed=0;
      if (debug)
	printf("[lfo_speed:%d]\n",P[cty].getPatternElement(cursor).getLfoSpeed());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }

  // Change lfo speed
  if (lfo_speed_all!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setLfoSpeed(P[cty].getPatternElement(i).getLfoSpeed()+lfo_speed_all);
      lfo_speed_all=0;
      if (debug)
	printf("[lfo_speed:%d]\n",P[cty].getPatternElement(cursor).getLfoSpeed());
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }

  
  
  // Change filter cutoff
  if (cutoff!=0)
    {
      P[cty].getPatternElement(cursor).setCutoff(P[cty].getPatternElement(cursor).getCutoff()+cutoff);
      cutoff=0;
      if (debug) printf("[cutoff:%d]\n",P[cty].getPatternElement(cursor).getCutoff());	  
    }
  
  // Change filter cutoff
  if (cutoff_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setCutoff(P[cty].getPatternElement(i).getCutoff()+cutoff_all);
      cutoff_all=0;
      if (debug) printf("[cutoff_all:%d]\n",P[cty].getPatternElement(cursor).getCutoff());	  
    }
  
  // Change filter resonance
  if (resonance!=0)
    {
      P[cty].getPatternElement(cursor).setResonance(P[cty].getPatternElement(cursor).getResonance()+resonance);
      resonance=0;
      if (debug) printf("[resonance:%d]\n",P[cty].getPatternElement(cursor).getResonance());	  
    }
  
  // Change filter resonance
  if (resonance_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setResonance(P[cty].getPatternElement(i).getResonance()+resonance_all);
      resonance_all=0;
      if (debug) printf("[resonance_all:%d]\n",P[cty].getPatternElement(cursor).getResonance());	  
    }
  
    // Change step divider
  if (divider!=0)
    {	  
      if (divider>0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()*2);
      if (divider<0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()/2);
      divider=0;
      P[cty].setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider());
    }
  
  // Change oscillator one
  if (osconetype!=0)
    {
      P[cty].getPatternElement(cursor).setOscillatorOneType(P[cty].getPatternElement(cursor).getOscillatorOneType()+osconetype);
      osconetype=0;
      if (debug)
	printf("[osconetype:%d]\n",P[cty].getPatternElement(cursor).getOscillatorOneType());	  
    }
  
  // Change oscillator one
  if (osconetype_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setOscillatorOneType(P[cty].getPatternElement(i).getOscillatorOneType()+osconetype_all);
      osconetype_all=0;
      if (debug)
	printf("[osconetype_all:%d]\n",P[cty].getPatternElement(cursor).getOscillatorOneType());	  
    }
  
  // Change oscillator two
  if (osctwotype!=0)
    {
      P[cty].getPatternElement(cursor).setOscillatorTwoType(P[cty].getPatternElement(cursor).getOscillatorTwoType()+osctwotype);
      osctwotype=0;
      if (debug)
	printf("[osctwotype:%d]\n",P[cty].getPatternElement(cursor).getOscillatorTwoType());	  
    }
  
  // Change oscillator two
  if (osctwotype_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setOscillatorTwoType(P[cty].getPatternElement(i).getOscillatorTwoType()+osctwotype_all);
      osctwotype_all=0;
      if (debug)
	printf("[osctwotype:%d]\n",P[cty].getPatternElement(cursor).getOscillatorTwoType());	  
    }
  


  // Change oscillator one
  if (machine_type!=0)
    {
      P[cty].getPatternElement(cursor).setMachineType(P[cty].getPatternElement(cursor).getMachineType()+machine_type);
      machine_type=0;
      if (debug)
	printf("[machine_type:%d]\n",P[cty].getPatternElement(cursor).getMachineType());	  
    }
  
  // Change oscillator one
  if (machine_type_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setMachineType(P[cty].getPatternElement(i).getMachineType()+machine_type_all);
      machine_type_all=0;
      if (debug)
	printf("[machine_type_all:%d]\n",P[cty].getPatternElement(cursor).getMachineType());	  
    }
  
  
  // Change Note
  if (note!=0)
    { 
      P[cty].getPatternElement(cursor).setNote(P[cty].getPatternElement(cursor).getNote()+note);
      note=0;
      printf("[note:%d]\n",P[cty].getPatternElement(cursor).getNote());	  
    }
  
  // Change Note
  if (note_all!=0)
    { 
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setNote(P[cty].getPatternElement(i).getNote()+note_all);
      note_all=0;
      printf("[note_all:%d]\n",P[cty].getPatternElement(cursor).getNote());	  
    }

      if (bpm!=0)
	{
	  //save the bpm in the 
	  //change the number of time AudioEngine need to be trigged
	  //to effectively change pattern step

	  bpm_current=P[cty].getBPM();
	  for(t=0;t<TRACK_MAX;t++)	    
	    P[t].setBPM(bpm_current+bpm);

	  bpm_current=bpm_current+bpm;
	  bpm=0;
	  refresh_bpm();

	}

      if (swing!=0)
	{
	  //save the swing in the Pattern
	  //change the number of time AudioEngine need to be trigged
	  //to effectively change pattern step

	  current_swing=current_swing+swing;
	  swing=0;

	  if (current_swing>75)
	    current_swing=75;
	  if (current_swing<25)
	    current_swing=25;

	  for(t=0;t<TRACK_MAX;t++)	    
	    P[t].setSwing(current_swing);

	  refresh_bpm();
	}



      // invert trig => insert/remove/copy note 
      if (invert_trig)
	{
	  if (P[cty].getPatternElement(cursor).getTrig())
	    {
	      P[cty].getPatternElement(cursor).setTrig(! P[cty].getPatternElement(cursor).getTrig());
	      PE=P[cty].getPatternElement(cursor);
	    }
	  else
	    {
	      P[cty].setPatternElement(cursor,PE);
	      P[cty].getPatternElement(cursor).setTrig(true);
	      if (P[cty].getPatternElement(cursor).getNote()==0)
		{
		  P[cty].getPatternElement(cursor).setAttack_amp(0);
		  P[cty].getPatternElement(cursor).setRelease_amp(64);
		  //P[cty].getPatternElement(cursor).setNote(37);
		  P[cty].getPatternElement(cursor).setNote(25);
		}
	    }
	  invert_trig=0;
	}
  
}



int seq_update_by_step()
{
  int  cty=SEQ.getCurrentTrackY();
  int  ctx=SEQ.getCurrentTrackX();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  int  t;

  //SEQ.getPatternSequencer(0).setStepDivider(4);

  // Load save only on pattern change
  if (save)
    {
      printf("<==[SAVE]==>\n");
      //PR.writePattern(1,ct+1,P[ct]);
      PR.writePattern(loadsave_cursor_x,loadsave_cursor_y,P[cty]);
      dirty_graphic=1;
      save=false;
    }

  // Load save only on pattern change
  if (load)
    {
      printf("<==[LOAD]==>\n");
      if (PR.PatternDataExist(loadsave_cursor_x,loadsave_cursor_y)==true)
	{
	  PR.readPatternData(loadsave_cursor_x,loadsave_cursor_y,P[cty]);

	  // Don't update BPM and Swing on single pattern load
	  // Do it only on "loadall"

	  //bpm_current=P[cty].getBPM();
	  //current_swing=P[t].getSwing();
	  //nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm_current);
	  //nb_tick_before_step_change=(60*DEFAULT_FREQ)/(bpm_current*4);
	  //AE.setNbTickBeforeStepChange(nb_tick_before_step_change);
	  refresh_bpm();


	  SEQ.getPatternSequencer(cty).setBPMDivider(P[cty].getBPMDivider());
	}
      else
	P[cty].init();
      load=false;
    }


  // Load save only on pattern change
  if (loadall)
    {
      printf("<==[LOAD_ALL]==>\n");
      for (t=0;t<TRACK_MAX;t++)
	{
	  if (PR.PatternDataExist(loadsave_cursor_x,t)==true)
	    {
	      PR.readPatternData(loadsave_cursor_x,t,P[t]);
	      bpm_current=P[t].getBPM();
	      current_swing=P[t].getSwing();
	      //nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm_current);
	      //nb_tick_before_step_change=(60*DEFAULT_FREQ)/(bpm_current*4);
	      //AE.setNbTickBeforeStepChange(nb_tick_before_step_change);
	      
	      refresh_bpm();

	      
	      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
	    }
	  else
	    P[t].init();
	}
      loadall=false;
    }


  // Load save only on pattern change
  if (saveall)
    {
      printf("<==[SAVE_ALL]==>\n");
      //PR.writePattern(1,ct+1,P[ct]);
      for (t=0;t<TRACK_MAX;t++)
	PR.writePattern(loadsave_cursor_x,t,P[t]);
      dirty_graphic=1;
      saveall=false;      
    }


  // Load save only on pattern change
  if (patternRemove)
    {
      printf("<==[REMOVE]==>\n");
      if (PR.PatternDataExist(loadsave_cursor_x,loadsave_cursor_y)==true)
	{
	  if (PR.PatternRemove(loadsave_cursor_x,loadsave_cursor_y))
	    {
	      printf("<==[REMOVE==Success]==>\n");
	      dirty_graphic=1;
	    }
	}
      else
	{
	  P[cty].init();
	  printf("<==[Remove==Failed]==>\n");
	}
      patternRemove=false;
    }

    
  //if (step==16) 
  //    step=0; 

  if (debug)
    printf("STEP:%d\n",step);	  
  
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
      if (P[t].getPatternElement(step).getTrig()==true)
	{
	  float   f=P[t].getPatternElement(step).getNoteFreq();
	  int     i=f;

	  int     i_c;
	  float   f_c;

	  int     i_r;
	  float   f_r;

	  //M[t]->set(NOTE_ON,0);
	  MM[t]->setAmplitude(P[t].getPatternElement(step).getAmp());
	  MM[t]->setMachineType(P[t].getPatternElement(step).getMachineType());
	  M[t]=MM[t]->getInput();                             


	  printf("[Freq:%d]\n",i);
	  M[t]->set(OSC1_FREQ,i);

	  /*
	  noteOffTrigger[t]=
	    P[t].getPatternElement(step).getAttack_amp()+
	    P[t].getPatternElement(step).getAttack_fltr()
	    ;
	  */
	  noteOffTrigger[t]=P[t].getPatternElement(step).getTrigTime()/8;

	  M[t]->set(ADSR_ENV0_ATTACK, P[t].getPatternElement(step).getAttack_amp());
	  M[t]->set(ADSR_ENV0_DECAY,  P[t].getPatternElement(step).getDecay_amp());
	  M[t]->set(ADSR_ENV0_SUSTAIN,P[t].getPatternElement(step).getSustain_amp());
	  M[t]->set(ADSR_ENV0_RELEASE,P[t].getPatternElement(step).getRelease_amp());

	  M[t]->set(ADSR_ENV1_ATTACK, P[t].getPatternElement(step).getAttack_fltr());
	  M[t]->set(ADSR_ENV1_ATTACK, P[t].getPatternElement(step).getDecay_fltr());
	  M[t]->set(ADSR_ENV1_ATTACK, P[t].getPatternElement(step).getSustain_fltr());
	  M[t]->set(ADSR_ENV1_RELEASE,P[t].getPatternElement(step).getRelease_fltr());



	  M[t]->set(OSC12_MIX,P[t].getPatternElement(step).getVCOMix());
	  M[t]->set(OSC1_PHASE,P[t].getPatternElement(step).getPhaseOsc1());

	  M[t]->set(LFO1_DEPTH,P[t].getPatternElement(step).getLfoDepth());
	  M[t]->set(LFO1_FREQ,P[t].getPatternElement(step).getLfoSpeed());
	  //M[t]->set(OSC12_MIX,phase);

	  M[t]->set(OSC1_TYPE,P[t].getPatternElement(step).getOscillatorOneType());
	  M[t]->set(OSC2_TYPE,P[t].getPatternElement(step).getOscillatorTwoType());

	  M[t]->set(OSC1_AMP,P[t].getPatternElement(step).getOsc1Amp());
	  M[t]->set(OSC2_AMP,P[t].getPatternElement(step).getOsc2Amp());


	  M[t]->set(FILTER1_TYPE,P[t].getPatternElement(step).getFilterType());
	  M[t]->set(FILTER1_ALGO,P[t].getPatternElement(step).getFilterAlgo());


	  M[t]->set(FILTER1_CUTOFF,P[t].getPatternElement(step).getCutoff());
	  M[t]->set(FILTER1_RES,P[t].getPatternElement(step).getResonance());




	  M[t]->set(NOTE_ON,1);

	}
      else
	{
	  if (noteOffTrigger[t]<0)
	    {
	      printf("$$$NOTEOFF\n");
	      //M[t]->getADSRAmp().setNoteOff();
	      //M[t]->getADSRFltr().setNoteOff();
	      //M[t]->reset();

	      M[t]->set(NOTE_ON,0);
	    }
	  else
	    {
	      noteOffTrigger[t]=noteOffTrigger[t]-1;
	      printf("noteOffTrigger[%d]=%d\n",noteOffTrigger[t],t);
	    }
	}
}


void seq_callback_update_step()
{
  int i;
  int oldstep;

  for(i=0;i<TRACK_MAX;i++)
    {
      oldstep=0;

      oldstep=SEQ.getPatternSequencer(i).getStep();
      SEQ.getPatternSequencer(i).incStep();
      if (oldstep!=SEQ.getPatternSequencer(i).getStep())
        seq_update_track(i);
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

  printf("openAudio start streaming\n");
  //AE.startAudio();
  //AE.startAudioSdl();
  AE.startAudio();
  seq_update_by_step();  
  while (true)
    {
      cty=SEQ.getCurrentTrackY();
      ctx=SEQ.getCurrentTrackX();

      handle_key();

      // A/D, Note, VCO, BPM, more handling...
      // apply the modification done by the user on the gui
      seq_update_multiple_time_by_step();

      
      
            
      // if user want to quit via handle_key
      if (quit)
	{
	  printf("user want to quit\n");
	  return(0);
	}
      
      //display graphic if something has change : handle_key 
      if (dirty_graphic)
	display_board();

      nbcb=AE.getNbCallback();

      if (nbcb>last_nbcb)
	{
	  last_nbcb=nbcb;
	}


      // change step in the pattern
      //if (nbcb-last_nbcb_ch_step>nb_cb_ch_step)
      if (seq_update_by_step_next)
	{	 

	  if (dirty_graphic)
	    display_board();
	  //printf("[cursor:%d]\n",cursor);
	  
	  //printf("loop\n");    
	  last_nbcb_ch_step=nbcb;
	  //**** step++;
	  //step++;

	  seq_update_by_step();
	  seq_update_by_step_next=0;

	}
    }
}

void load_pattern()
{
  int t;

  string fileName="data.pic";
  //PR.setFileName("data.pic");
  PR.init();
  PR.setFileName(fileName);
  for (t=0;t<TRACK_MAX;t++)
    {
      PR.readPatternData(0,t,P[t]);
      
      // Ugly hack for BPM management
      bpm_current=P[t].getBPM();
      current_swing=P[t].getSwing();
      refresh_bpm();

      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());

    }

    //PR.readPatternData(1,i+1,P[i]);

}



int main()
{
  int i;
  //  exit(0);
  IE.init();
  IE.printState();
  //IE.handle_key();
  //IE.init();
  //IE.printState();
  //exit(0);
  for (i=0;i<TRACK_MAX;i++)
    noteOffTrigger[i]=0;
  load_pattern();
  printf("[openVideo output]\n");
  SG.initVideo();
  //SDL_InitSubSystem(SDL_INIT_AUDIO);
  //handle_key();
  //  SDL_EnableKeyRepeat(500,500);
  SG.openBMPFont();
  if (SG.openTTFFont()==false) { printf("ttf font error\n"); exit(1); }
  display_board();

  printf("[openAudio output]\n");
  //AE.setupSequencerCallback(printme);
  refresh_bpm();
  AE.setupSequencerCallback(seq_callback_update_step);
  AE.openAudio();
  //AE.openAudioSdl();
  
  seq();

  printf("[closeAudio output]\n");
  //AE.stopAudioSdl();

  AE.closeAudio();
  
  //AE.closeAudioSdl();

  printf("[closeVideo output]\n");
  SG.closeVideo();
  //sleep(10);
  //PE.print();
  printf("Exiting PatternPlayer\n");
}
