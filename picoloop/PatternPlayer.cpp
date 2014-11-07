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

#include "Generator.h"
#include "WaveTableManager.h"
#include "WaveTable.h"

#include "NoteFreq.h"


#define KEY_REPEAT_INTERVAL 400


#define KEY_REPEAT_INTERVAL_SMALLEST  4
#define KEY_REPEAT_INTERVAL_SMALL     8
#define KEY_REPEAT_INTERVAL_MIDDLE    32
#define KEY_REPEAT_INTERVAL_LONG      64
#define KEY_REPEAT_INTERVAL_LONGEST   128

#define DISABLEDBOX_COLOR   0x0AF0FE
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

enum {
  CURSOR_LOADSAVE,
  CURSOR_SONG
};
  

//menu_cursor
enum {
  GLOBALMENU_AD,         // 0
  GLOBALMENU_NOTE,       // 1
  GLOBALMENU_OSC,        // 2 
  GLOBALMENU_VCO,        // 3
  GLOBALMENU_LFO,        // 4
  GLOBALMENU_FLTR,       // 5

  GLOBALMENU_LS,         // 6
  GLOBALMENU_BANK,       // 7
  GLOBALMENU_PSH,        // 8
  GLOBALMENU_MAC,        // 9
  GLOBALMENU_FX,         // 10
  GLOBALMENU_BPM,        // 11

};


//menu_ad
enum {
  MENU_AD_AMP_ATTACK_RELEASE,
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
  MENU_VCO_OSCAMP,
  MENU_VCO_FMTYPE
};

enum {
  MENU_FX_DEPTH_SPEED
};

enum {
  MENU_LS_PATTERN,
  MENU_LS_SONG,
};



  

vector <Pattern>            P(TRACK_MAX);  
vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);
vector <Effect    *>        FX(TRACK_MAX);

Sequencer      SEQ;         // used to  store/get information about sequencer 
AudioEngine    AE;          // used to  init alsa/rtaudio
PatternReader  PR;          // used to  read data.pic file
PatternElement PE;          // used for copy paste PatternElement
PatternElement PECursor;    // used for storing the current PatternElement ( under the cursor )
InputManager   IE;          // used to  fetch key
SDL_GUI        SG;          // used to  open a gui and display stuff
//NoteFreq       NF;
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


int pshift_all=0;
int plength_all=0;

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

int fx_depth=0;
int fx_speed=0;

int fx_depth_all=0;
int fx_speed_all=0;

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

int fmtype=0;
int fmtype_all=0;



int loadsave_cursor_x=0; // index in the load/save menu
int loadsave_cursor_y=0; // index in the load/save menu

int song_cursor_x=0; // index in the song menu
int song_cursor_y=0; // index in the song menu

int loadsave_cursor_mode=0; // 0 loadsave mode cursor // 1 song mode cursor
int pattern_song_inc=0;     // increment/decrement the current value of song_cursor_x/song_cursor_y
int pattern_song_reload=0;  // if > 1 reload from current position
int pattern_song_loop=0;    // if > 1 loop from current song position to loop point

int bank=0;
int bank_inc=0;
int bank_to_load=0;

int start_key=0;        // start key pressed ?
//int step=0;             // current step in the sequencer
int divider=0;           // divider - => /1 /2 /4 /8  ; divider + => /8 /4 /2 /1
int menu_cursor=GLOBALMENU_AD;      // index int the menu
int menu=MENU_ON_PAGE1;             // menu mode
int menu_note=ENABLE;
int menu_ad=MENU_AD_AMP_ATTACK_RELEASE;
int menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;
int menu_fx=MENU_FX_DEPTH_SPEED;
int menu_vco=MENU_VCO_OSCMIX_PHASE;
int menu_ls=MENU_LS_PATTERN;

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
      
      if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE)
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
      printf("HIT\n");
      SG.clearScreen();
      
      sprintf(str_bank,"Current Bank %d ",bank);

      SG.guiTTFText(30,10, str_bank);

      sprintf(str_bank,"Bank to load %d ",bank_to_load);
      SG.guiTTFText(30,30, str_bank);
    }
}


void display_board_load_save()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  int  bank=PR.getBank();

  int loadsave_cursor_x_div_sixteen=loadsave_cursor_x/16;
  int loadsave_cursor_x_divmul_sixteen=loadsave_cursor_x_div_sixteen*16;

  int song_cursor_x_div_sixteen=song_cursor_x/16;
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
      printf("HIT\n");
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
	      SG.middleBoxNumberUp(x%16,y,NOTE_COLOR);
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
	SG.middleBoxNumberUp(loadsave_cursor_x%16,
			     loadsave_cursor_y,
			     TRIG_COLOR);

      // we are in song mode
      // Display your current position
      if (loadsave_cursor_mode==CURSOR_SONG)
	SG.middleBoxNumberDown(song_cursor_x%16,
			       song_cursor_y,
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
	    //printf("%d\n",SEQ.getSongSequencer().getPatternNumber(x,y));
	    //printf("\n");
	}
      
      //exit(0);
	  //SG.drawTTFTextLoadSaveBoxNumberDown(x,y,txt_tab[SEQ.getSongSequencer().getPatternNumber(x,y)]);
      //SG.drawTTFTextLoadSaveBoxNumberDown(x,y,txt_tab[x%16]);
      
    }





  // Draw LFO when load/save is not selected
  // Why ? need to display something, so why not ?
  if (menu!=MENU_OFF && 
      menu_cursor==GLOBALMENU_LS
      )
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
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	      
		  // LFO
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getLfoDepth(),128,SMALLBOX_COLOR);
	      SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getLfoSpeed(),SMALLBOX_COLOR);
	    }
	}
    }                  
}



void display_board_psh()
{
  int x,y;
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();




  //Draw LFO when load/save is not selected
  // Why ? need to display something, so why not ?
  if (menu!=MENU_OFF && 
      menu_cursor==GLOBALMENU_PSH
      )
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
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	      
		  // LFO
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getLfoDepth(),128,SMALLBOX_COLOR);
	      SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getLfoSpeed(),SMALLBOX_COLOR);
	    }
	}
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

  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_FMTYPE)
    {	  
      SG.drawBoxNumber(step,STEP_COLOR);  
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
	      SG.drawTTFTextNumberFirstLine(i,P[cty].getPatternElement(i).getFMTypeCharStar());
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


void display_board_fx()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_FX)
    {

      if (menu_fx==MENU_FX_DEPTH_SPEED)
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
		  SG.smallBoxNumber(i,P[cty].getPatternElement(i).getFxDepth(),128,SMALLBOX_COLOR);
		  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).getFxSpeed(),SMALLBOX_COLOR);      
		}
	    }
	}
      /*
      if (menu_fltr==MENU_FX_ALGO)
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
      */

    }
}




void display_board()
{
  int  i;
  char str_up[8];
  char str_down[24];
  char str_divider[8];
  char str_submenu[16];

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
  //  sprintf(str,"Track %d ",ct);
  sprintf(str_up,"Track %d ",cty);
  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line1,str_up);

  sprintf(str_divider,"/%d",stepdiv);
  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line3,str_divider);

  if (menu_cursor!=GLOBALMENU_AD ||
      menu_cursor!=GLOBALMENU_NOTE)    
    {
      sprintf(str_submenu,"               ");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }
  if (menu_note==ENABLE &&
      menu_cursor==GLOBALMENU_NOTE)
    {
      sprintf(str_submenu,"NOTE");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }
  if (menu_note==DISABLE &&
      menu_cursor==GLOBALMENU_NOTE)
    {
      sprintf(str_submenu,"DOT");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }
  if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"AMP  A/R");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"AMP  D/S");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"FLTR A/R");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"FLTR D/S");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  if (menu_ad==MENU_AD_TRIGTIME_AMP &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"T/N AMP");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE &&
      menu_cursor==GLOBALMENU_FLTR)
    {
      sprintf(str_submenu,"CUTOFF/RES");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_fltr==MENU_FLTR_ALGO_TYPE &&
      menu_cursor==GLOBALMENU_FLTR
      )
    {
      sprintf(str_submenu,"ALGO/TYPE");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  if (menu_vco==MENU_VCO_OSCAMP &&
      menu_cursor==GLOBALMENU_VCO
      )
    {
      sprintf(str_submenu,"OP1AMP/AP2AMP");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_vco==MENU_VCO_OSCMIX_PHASE &&
      menu_cursor==GLOBALMENU_VCO
      )
    {
      sprintf(str_submenu,"Frq1&Frq2|OSCMix&Ph");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  if (menu_vco==MENU_VCO_FMTYPE &&
      menu_cursor==GLOBALMENU_VCO
      )
    {
      sprintf(str_submenu,"FMType");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }



  /*
  if (menu_ad==MENU_AD_ATTACK_AMP &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"ATTACK/AMP");
      SG.guiTTFText(right_x_display_offset,60,str_submenu);
    }
  */
  if (menu_cursor==GLOBALMENU_BPM)
    {
      //sprintf(str_submenu,"SWING %d",(current_swing*100)/127);
      sprintf(str_submenu,"SWING %d",current_swing);
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

//  sprintf(str_down,"[A/D] Note L/S",cty);
  
  //  printf("           AD:%d FX:%d\n",AD,FX);
  //  exit(0);

  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_AD)    sprintf(str_down,"[A/R] Note  OSC   VCO   LFO   FLTR   ",cty);  
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_NOTE)  sprintf(str_down," A/R [Note] OSC   VCO   LFO   FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_OSC)   sprintf(str_down," A/R  Note [OSC]  VCO   LFO   FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_VCO)   sprintf(str_down," A/R  Note  OSC  [VCO]  LFO   FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_LFO)   sprintf(str_down," A/R  Note  OSC   VCO  [LFO]  FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_FLTR)  sprintf(str_down," A/R  Note  OSC   VCO   LFO  [FLTR]  ",cty);

  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_LS)    sprintf(str_down,"[L/S] BANK  PSH   MAC   FX    BPM    ",cty);  
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_BANK)  sprintf(str_down," L/S [BANK] PSH   MAC   FX    BPM    ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_PSH)   sprintf(str_down," L/S  BANK [PSH]  MAC   FX    BPM    ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_MAC)   sprintf(str_down," L/S  BANK  PSH  [MAC]  FX    BPM    ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_FX)    sprintf(str_down," L/S  BANK  PSH   MAC  [FX]   BPM    ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==GLOBALMENU_BPM)   sprintf(str_down," L/S  BANK  PSH   MAC   FX   [BPM]   ",cty);

  if (menu==0)                         sprintf(str_down,"                     ",cty);

  if (menu_cursor==GLOBALMENU_AD)               sprintf(str_up,"A/R     ");
  if (menu_cursor==GLOBALMENU_NOTE)             sprintf(str_up,"Note    ");
  if (menu_cursor==GLOBALMENU_OSC)              sprintf(str_up,"OSC     ");
  if (menu_cursor==GLOBALMENU_VCO)              sprintf(str_up,"VCO     ");
  if (menu_cursor==GLOBALMENU_LFO)              sprintf(str_up,"LFO     ");
  if (menu_cursor==GLOBALMENU_FLTR)             sprintf(str_up,"FLTR    ");


  if (menu_cursor==GLOBALMENU_LS)               sprintf(str_up,"L/S     ");
  if (menu_cursor==GLOBALMENU_BANK)             sprintf(str_up,"BANK    ");
  if (menu_cursor==GLOBALMENU_PSH)              sprintf(str_up,"PSH     ");
  if (menu_cursor==GLOBALMENU_MAC)              sprintf(str_up,"MAC     ");
  if (menu_cursor==GLOBALMENU_BPM)              sprintf(str_up,"BPM %d",bpm_current);
  if (menu_cursor==GLOBALMENU_FX)               sprintf(str_up,"FX      ");




  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line4,str_up);

  SG.guiTTFText(menu_x_display_offset,
		menu_y_display_offset,str_down);

  // Draw all box default color   
  //for (i=0;i<16;i++)
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
	  if (menu_cursor<6) 
	    menu=MENU_ON_PAGE1;
	  if (menu_cursor>=6)
	    menu=MENU_ON_PAGE2;
	  break;

	case MENU_ON_PAGE1: 
	  menu=MENU_ON_PAGE2;
	  if (menu_cursor<6)
	    menu_cursor+=6; 
	  else
	    menu_cursor=6;
	  break;

	case MENU_ON_PAGE2: 
	  menu=MENU_ON_PAGE1; 
	  if (menu_cursor>=6)
	    menu_cursor-=6; 
	  else
	    menu_cursor=0;
	  break;

	  //	case MENU_ON_PAGE1: menu=MENU_OFF;      break;
	  //case MENU_ON_PAGE1: menu=MENU_ON_PAGE2; menu_cursor=+4; break;
	  //case MENU_ON_PAGE2: menu=MENU_ON_PAGE1; menu_cursor=-4; break;
	}

      //PR.saveSong(SEQ.getSongSequencer().getSongVector());
      PR.saveSong(SEQ.getSongSequencer());

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
	  if (menu_cursor<0     && menu==MENU_ON_PAGE1) menu_cursor=5;
	  if (menu_cursor<6     && menu==MENU_ON_PAGE2) menu_cursor=11;

	  dirty_graphic=1;
	  printf("\t\t[menu_cursor:%d]\n",menu_cursor);
	  printf("key left\n");            
	}      
      
      if(keyState[BUTTON_RIGHT])
	{
	  if (keyRepeat[BUTTON_RIGHT]    == 1 || 
	      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG == 0)
	    menu_cursor++;
	  //if (menu_cursor>GLOBALMENU_VCO  && menu==MENU_ON_PAGE1) menu_cursor=GLOBALMENU_AD;
	  //if (menu_cursor>GLOBALMENU_BPM  && menu==MENU_ON_PAGE2) menu_cursor=GLOBALMENU_OSC;
	  if (menu_cursor>5      && menu==MENU_ON_PAGE1) menu_cursor=0;
	  if (menu_cursor>11     && menu==MENU_ON_PAGE2) menu_cursor=6;

	  dirty_graphic=1;
	  printf("\t\t[menu_cursor:%d]\n",menu_cursor);
	  printf("key right\n");            
	}
      
      if(keyState[BUTTON_UP])
	{
	  if (keyRepeat[BUTTON_UP]    == 1 || 
	      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG == 0)
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


  // Check this ??? do not think it works
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
	      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor=cursor-4;
	  if (cursor < 0) cursor=cursor +16;
	  printf("key down : up \n");
	  dirty_graphic=1;
	}
      
      if(keyState[BUTTON_DOWN] && 
	 !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor=( cursor+4 ) %16;
	  printf("key down : down\n");
	  dirty_graphic=1;
	}
      
      if(keyState[BUTTON_LEFT] && 
	 !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_LEFT]==1 || 
	      keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)
	    cursor--;
	  
	  if (cursor<0) cursor=15;
	  printf("key left\n");            
	  dirty_graphic=1;
	}
      
      if (keyState[BUTTON_RIGHT] && 
	  !keyState[BUTTON_B])
	{
	  if (keyRepeat[BUTTON_RIGHT]   ==1 || 
	      keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0)
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
      menu_ad       == MENU_AD_AMP_ATTACK_RELEASE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_amp=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { attack_amp=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
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
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_amp=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { sustain_amp=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { sustain_amp=-1; 	  dirty_graphic=1; }
    }  


  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_ATTACK_RELEASE)
    {
      //printf("***********************\n");
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_amp_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_amp_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { attack_amp_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
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
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_amp_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_amp_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { sustain_amp_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
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
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_fltr=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_fltr=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { attack_fltr=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
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
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_fltr=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_fltr=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { sustain_fltr=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
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
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_fltr_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { release_fltr_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { attack_fltr_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
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
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_fltr_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { decay_fltr_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { sustain_fltr_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
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
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { amp=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { trig_time=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { trig_time=-1; 	  dirty_graphic=1; }
    }  


  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { amp_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { amp_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { trig_time_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { trig_time_all=-1; 	  dirty_graphic=1; }
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	{ note=-1; 	  dirty_graphic=1;}

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { note=1;  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { note=12;   	  dirty_graphic=1;}

      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	{ note=-12;  	  dirty_graphic=1;}
    }  

    // GLOBALMENU_NOTE
  // change note
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {
      
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	{ note_all=-1; 	  dirty_graphic=1;}

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { note_all=1;  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { note_all=12;   	  dirty_graphic=1;}

      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osconetype=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osconetype=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osctwotype=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osctwotype=-1;  	  dirty_graphic=1;}
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osconetype_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osconetype_all=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osctwotype_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { osctwotype_all=-1;  	  dirty_graphic=1;}
    }

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

      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_depth=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_depth=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_speed=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_speed=-1;  	  dirty_graphic=1;}
    }

  // GLOBALMENU_FX
  // change fx_depth and fx_speed
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_FX)
    {
      // Insert/Remove Trig
      //sub_handle_invert_trig();

      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_depth_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_depth_all=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_speed_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { fx_speed_all=-1;  	  dirty_graphic=1;}
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { vcomix=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { vcomix=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { phase_osc1=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { phase_osc1=-1;  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { vcomix_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { vcomix_all=1;  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { phase_osc1_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc1_amp=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc1_amp=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc2_amp=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc2_amp=-1;  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCAMP
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc1_amp_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc1_amp_all=1;  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc2_amp_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { osc2_amp_all=-1;  	  dirty_graphic=1;}
      //????
    }


  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_FMTYPE
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { fmtype=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { fmtype=-1;  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_FMTYPE
      )
    {

      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { fmtype_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { fmtype_all=-1;  	  dirty_graphic=1;}

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
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { machine_type=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { machine_type=-1;  	  dirty_graphic=1;}
    }

  // GLOBALMENU_MAC
  // change machine type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_MAC )
    {
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { machine_type_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_depth=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_depth=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_speed=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_speed=-1;  	  dirty_graphic=1;}
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_depth_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_depth_all=1;  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_speed_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { lfo_speed_all=-1;  	  dirty_graphic=1;}
      //????
    }

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
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { pshift_all=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { pshift_all=1;  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { plength_all=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { plength_all=-1;  	  dirty_graphic=1;}
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
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { resonance=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { resonance=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
	  { cutoff=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
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
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { resonance_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { resonance_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
	  { cutoff_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
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
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { filter_algo=-1;   dirty_graphic=1; }
            if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { filter_algo=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
	  { filter_type=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
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
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { filter_algo_all=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>128)
	  if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==1 )  
	  { filter_algo_all=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>128) 
	  if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
	  { filter_type_all=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>128 ) 
	  if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==1 ) 
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { bpm=-1; 	  dirty_graphic=1; printf("[B+LEFT  t=%d]\n",bpm_current); }

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { bpm=1; 	  dirty_graphic=1; printf("[B+RIGHT t=%d]\n",bpm_current);}

      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { bpm=-10; 	  dirty_graphic=1; printf("[B+DOWN  t=%d]\n",bpm_current); }

      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { bpm=10; 	  dirty_graphic=1; printf("[B+UP    t=%d]\n",bpm_current);}


      //if (bpm < 20) bpm=20;
      //if (bpm > 260) bpm=260;

      //nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm);
    }  

  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {

      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { divider=-1; 	  dirty_graphic=1; printf("[A+DOWN  t=%d] divider\n",divider); }

      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { divider=1; 	  dirty_graphic=1; printf("[A+UP    t=%d] divider\n",divider);}
    }  


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {

      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONGEST==0)
	  { swing=-1; 	  dirty_graphic=1; printf("[A+DOWN  t=%d] swing\n",current_swing); }

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
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
	  lastEvent   == SDL_KEYUP   &&
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
	  keyState[BUTTON_B])
	{
	  if (keyState[BUTTON_DOWN])
	    pattern_song_inc=-1;
	  if (keyState[BUTTON_UP])	
	    pattern_song_inc=1;
	}


      if (menu                 == MENU_OFF && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
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
      
      if (menu                 == MENU_OFF        && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
	  keyState[BUTTON_A] )
	{
	  if (keyState[BUTTON_DOWN])
	    saveall=true;
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
	      { loadsave_cursor_x--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { loadsave_cursor_x++;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { loadsave_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { loadsave_cursor_y++;  dirty_graphic=1;}
	  
	  if (loadsave_cursor_x>MAX_PATTERN_BY_PROJECT-1)        { loadsave_cursor_x=0;                           }
	  if (loadsave_cursor_x<0)                               { loadsave_cursor_x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (loadsave_cursor_y>TRACK_MAX-1)                     { loadsave_cursor_y=0;                           }
	  if (loadsave_cursor_y<0)                               { loadsave_cursor_y=TRACK_MAX-1;                 }  
	  
	  SEQ.setCurrentTrackY(loadsave_cursor_y);
	}

      if (menu                 == MENU_OFF     && 	  
	  loadsave_cursor_mode == CURSOR_SONG  &&
	  (!(
	     keyState[BUTTON_B] ||
	     keyState[BUTTON_A])))
	{
	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1  || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_x--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { song_cursor_x++;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { song_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_y++;  dirty_graphic=1;}
	  
	  if (song_cursor_x>MAX_PATTERN_BY_PROJECT-1)            { song_cursor_x=0;                           }
	  if (song_cursor_x<0)                                   { song_cursor_x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (song_cursor_y>TRACK_MAX-1)                         { song_cursor_y=0;                           }
	  if (song_cursor_y<0)                                   { song_cursor_y=TRACK_MAX-1;                 }  
	  
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
	      { song_cursor_x++;  dirty_graphic=1; SEQ.getSongSequencer().setPatternNumber(song_cursor_x,song_cursor_y,SEQ.getSongSequencer().getPatternNumber(song_cursor_x-1,song_cursor_y)); }
	  /*
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { song_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_y++;  dirty_graphic=1;}
	  */
	  if (song_cursor_x>MAX_PATTERN_BY_PROJECT-1)            { song_cursor_x=0;                           }
	  if (song_cursor_x<0)                                   { song_cursor_x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (song_cursor_y>TRACK_MAX-1)                         { song_cursor_y=0;                           }
	  if (song_cursor_y<0)                                   { song_cursor_y=TRACK_MAX-1;                 }  
	  
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
	  lastEvent   == SDL_KEYUP   &&
	  lastKey     == BUTTON_START
	  //keyState[BUTTON_START]
	  )
	{
	  
	  bank=bank_to_load;
	  PR.init();
	  PR.setBank(bank);
	  load_pattern();
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	  return;
	}



      if (menu                 == MENU_OFF    && 
	  keyRepeat[BUTTON_B]                 &&
	  keyRepeat[BUTTON_B]%KEY_REPEAT_INTERVAL_LONG==0)
	{
	  if (keyState[BUTTON_DOWN])
	    bank_inc=-1;
	  if (keyState[BUTTON_UP])	
	    bank_inc=1;
	}


      if (menu                 == MENU_OFF && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
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
      
      if (menu                 == MENU_OFF        && 
	  loadsave_cursor_mode == CURSOR_LOADSAVE &&
	  keyState[BUTTON_A] )
	{
	  if (keyState[BUTTON_DOWN])
	    saveall=true;
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
	      { loadsave_cursor_x--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { loadsave_cursor_x++;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { loadsave_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { loadsave_cursor_y++;  dirty_graphic=1;}
	  
	  if (loadsave_cursor_x>MAX_PATTERN_BY_PROJECT-1)        { loadsave_cursor_x=0;                           }
	  if (loadsave_cursor_x<0)                               { loadsave_cursor_x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (loadsave_cursor_y>TRACK_MAX-1)                     { loadsave_cursor_y=0;                           }
	  if (loadsave_cursor_y<0)                               { loadsave_cursor_y=TRACK_MAX-1;                 }  
	  
	  SEQ.setCurrentTrackY(loadsave_cursor_y);
	}

      if (menu                 == MENU_OFF     && 	  
	  loadsave_cursor_mode == CURSOR_SONG  &&
	  (!(
	     keyState[BUTTON_B] ||
	     keyState[BUTTON_A])))
	{
	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1  || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_x--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_RIGHT])
	    if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { song_cursor_x++;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { song_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_y++;  dirty_graphic=1;}
	  
	  if (song_cursor_x>MAX_PATTERN_BY_PROJECT-1)            { song_cursor_x=0;                           }
	  if (song_cursor_x<0)                                   { song_cursor_x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (song_cursor_y>TRACK_MAX-1)                         { song_cursor_y=0;                           }
	  if (song_cursor_y<0)                                   { song_cursor_y=TRACK_MAX-1;                 }  
	  
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
	      { song_cursor_x++;  dirty_graphic=1; SEQ.getSongSequencer().setPatternNumber(song_cursor_x,song_cursor_y,SEQ.getSongSequencer().getPatternNumber(song_cursor_x-1,song_cursor_y)); }
	  /*
	  if (keyState[BUTTON_UP])
	    if (keyRepeat[BUTTON_UP]==1    || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0)    
	      { song_cursor_y--;  dirty_graphic=1;}
	  
	  if (keyState[BUTTON_DOWN])
	    if (keyRepeat[BUTTON_DOWN]==1  || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0)  
	      { song_cursor_y++;  dirty_graphic=1;}
	  */
	  if (song_cursor_x>MAX_PATTERN_BY_PROJECT-1)            { song_cursor_x=0;                           }
	  if (song_cursor_x<0)                                   { song_cursor_x=MAX_PATTERN_BY_PROJECT-1;    }

	  if (song_cursor_y>TRACK_MAX-1)                         { song_cursor_y=0;                           }
	  if (song_cursor_y<0)                                   { song_cursor_y=TRACK_MAX-1;                 }  
	  
	  //SEQ.setCurrentTrackY(loadsave_cursor_y);
	}


    }

}


void handle_key_submenu_ad()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  // change GLOBALMENU_AD SUBMENU
  if (keyState[BUTTON_START]       &&
      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==127   &&
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
      keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==127  &&
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
	  if      (menu_ad==MENU_AD_AMP_ATTACK_RELEASE)       { menu_ad=MENU_AD_AMP_DECAY_SUSTAIN;        }
	  else if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)        { menu_ad=MENU_AD_FLTR_ATTACK_RELEASE;      }   
	  else if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE)      { menu_ad=MENU_AD_FLTR_DECAY_SUSTAIN;       }   
	  else if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN)       { menu_ad=MENU_AD_TRIGTIME_AMP;             }   
	  else if (menu_ad==MENU_AD_TRIGTIME_AMP)             { menu_ad=MENU_AD_AMP_ATTACK_RELEASE;      }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu env : %d]\n",menu_ad);
    }


}


void handle_key_submenu_vco()
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
	  if      (menu_vco==MENU_VCO_FMTYPE)              { menu_vco=MENU_VCO_OSCAMP;            }
	  else if (menu_vco==MENU_VCO_OSCAMP)              { menu_vco=MENU_VCO_OSCMIX_PHASE;      }   
	  else if (menu_vco==MENU_VCO_OSCMIX_PHASE)        { menu_vco=MENU_VCO_FMTYPE;            }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu env : %d]\n",menu_ad);
    }
}

void handle_key_submenu_fltr()
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

  if (IE.shouldExit())
    quit=1;
    //exit(0);
  
  //if (start_key==2) 
  //printf("%d %d %d\n",lastKey,lastEvent,lastKey==&& BUTTON_START && lastEvent==SDL_KEYUP);
  //  printf("lastevent=%d\n",lastEvent);


  handle_key_menu();
  handle_key_sixteenbox();

  // Refresh the PECursor with the current Element
  PECursor=P[cty].getPatternElement(cursor);  

  if (menu_cursor==GLOBALMENU_AD)   
    { handle_key_amp_env(); handle_key_submenu_ad(); }
  if (menu_cursor==GLOBALMENU_NOTE) 
    { handle_key_note();                             }
  if (menu_cursor==GLOBALMENU_OSC)  
    { handle_key_osc();                              }
  if (menu_cursor==GLOBALMENU_VCO)  
    { handle_key_vco(); handle_key_submenu_vco();    }
  if (menu_cursor==GLOBALMENU_LFO)  
    { handle_key_lfo();                              }
  if (menu_cursor==GLOBALMENU_FLTR) 
    { handle_key_fltr(); handle_key_submenu_fltr();  }

  if (menu_cursor==GLOBALMENU_LS)   handle_key_load_save();
  if (menu_cursor==GLOBALMENU_BANK) handle_key_bank();
  if (menu_cursor==GLOBALMENU_PSH)  handle_key_psh();
  if (menu_cursor==GLOBALMENU_MAC)  handle_key_mac();
  if (menu_cursor==GLOBALMENU_FX)   handle_key_fx();
  if (menu_cursor==GLOBALMENU_BPM)  handle_key_bpm();
  
 
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


  // Change Fx Depth
  if (fx_depth!=0)
    {
      P[cty].getPatternElement(cursor).setFxDepth(P[cty].getPatternElement(cursor).getFxDepth()+fx_depth);
      fx_depth=0;
      if (debug)
	printf("[fx_depth:%d]\n",P[cty].getPatternElement(cursor).getFxDepth());      
    }
  
  // Change Fx Depth All
  if (fx_depth_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setFxDepth(P[cty].getPatternElement(i).getFxDepth()+fx_depth_all);
      fx_depth_all=0;
      if (debug)
	printf("[fx_depth_all:%d]\n",P[cty].getPatternElement(cursor).getFxDepth());      
    }


  // Change Fx Speed
  if (fx_speed!=0)
    {
      P[cty].getPatternElement(cursor).setFxSpeed(P[cty].getPatternElement(cursor).getFxSpeed()+fx_speed);
      fx_speed=0;
      if (debug)
	printf("[fx_speed:%d]\n",P[cty].getPatternElement(cursor).getFxSpeed());      
    }
  
  // Change Fx Speed
  if (fx_speed_all!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setFxSpeed(P[cty].getPatternElement(i).getFxSpeed()+fx_speed_all);
      fx_speed_all=0;
      if (debug)
	printf("[fx_speed_all:%d]\n",P[cty].getPatternElement(cursor).getFxSpeed());      
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


  // Change Note
  if (fmtype!=0)
    { 
      P[cty].getPatternElement(cursor).setFmType(P[cty].getPatternElement(cursor).getFmType()+fmtype);
      fmtype=0;
      printf("[fmtype:%d]\n",P[cty].getPatternElement(cursor).getFmType());	  
    }
  
  // Change Note
  if (fmtype_all!=0)
    { 
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).setFmType(P[cty].getPatternElement(i).getFmType()+fmtype_all);
      fmtype_all=0;
      printf("[fmtype_all:%d]\n",P[cty].getPatternElement(cursor).getFmType());	  
    }



  // Change Note
  if (plength_all!=0)
    { 
      SEQ.getPatternSequencer(cty).setPatternLenght(SEQ.getPatternSequencer(cty).getPatternLenght()+plength_all);
      plength_all=0;
    }



  // shift PatternEntry
  if (pshift_all!=0)
    {       
      PatternElement Pe;

      if (pshift_all<0)
	{
	  Pe=P[cty].getPatternElement(0);
	  for (i=0;i<15;i++)
	    {	    
	      P[cty].getPatternElement(i)=P[cty].getPatternElement(i+1);	    
	    }
	  i++;
	  P[cty].getPatternElement(15)=Pe;	    
	}

      if (pshift_all>0)
	{
	  Pe=P[cty].getPatternElement(15);
	  for (i=15;i>0;i--)
	    {	    
	      P[cty].getPatternElement(i)=P[cty].getPatternElement(i-1);	    
	    }
	  i--;
	  P[cty].getPatternElement(0)=Pe;	    
	}

      pshift_all=0;
      dirty_graphic=1;
      printf("[pshift_all:left]\n");	  
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
		  //P[cty].getPatternElement(cursor).setAttack_amp(0);
		  //P[cty].getPatternElement(cursor).setRelease_amp(64);
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

  if (pattern_song_inc!=0)
    {

      SEQ.getSongSequencer().setPatternNumber(song_cursor_x,
					      song_cursor_y,
					      SEQ.getSongSequencer().getPatternNumber(song_cursor_x,
										      song_cursor_y)+
					      pattern_song_inc);
      pattern_song_inc=0;
    }

  if (pattern_song_reload!=0)
    {

      SEQ.getSongSequencer().setStep(song_cursor_x);      
      pattern_song_reload=0;
      SEQ.setSongMode(1);
    }

  if (pattern_song_loop!=0)
    {

      SEQ.getSongSequencer().setLoopPoint(song_cursor_x);
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
	  //float   f=P[t].getPatternElement(step).getNoteFreq();
	  //float   f=NF.getFNoteFreq(P[t].getPatternElement(step).getNote());

	  //int     i=f;

	  //int     i_c;
	  //float   f_c;

	  //int     i_r;
	  //float   f_r;

	  //M[t]->set(NOTE_ON,0);
	  MM[t]->setAmplitude(P[t].getPatternElement(step).getAmp());
	  MM[t]->setMachineType(P[t].getPatternElement(step).getMachineType());
	  M[t]  = MM[t]->getInput();                             
	  FX[t] = MM[t]->getEffect();                             


	  FX[t]->setDepth(P[t].getPatternElement(step).getFxDepth());
	  FX[t]->setSpeed(P[t].getPatternElement(step).getFxSpeed());

	  //printf("[Freq:%d]\n",i);
	  //M[t]->set(OSC1_FREQ,i);
	  M[t]->reset();
	  //M[t]->setF(OSC1_FREQ,f);
	  M[t]->setI(OSC1_NOTE,P[t].getPatternElement(step).getNote());

	  /*
	  noteOffTrigger[t]=
	    P[t].getPatternElement(step).getAttack_amp()+
	    P[t].getPatternElement(step).getAttack_fltr()
	    ;
	  */
	  noteOffTrigger[t]=P[t].getPatternElement(step).getTrigTime()/8;


	  M[t]->setI(FM_TYPE,P[t].getPatternElement(step).getFmType());

	  M[t]->setI(ADSR_ENV0_ATTACK,  P[t].getPatternElement(step).getAttack_amp());
	  M[t]->setI(ADSR_ENV0_DECAY,   P[t].getPatternElement(step).getDecay_amp());
	  M[t]->setI(ADSR_ENV0_SUSTAIN, P[t].getPatternElement(step).getSustain_amp());
	  M[t]->setI(ADSR_ENV0_RELEASE, P[t].getPatternElement(step).getRelease_amp());

	  M[t]->setI(ADSR_ENV1_ATTACK,  P[t].getPatternElement(step).getAttack_fltr());
	  M[t]->setI(ADSR_ENV1_DECAY ,  P[t].getPatternElement(step).getDecay_fltr());
	  M[t]->setI(ADSR_ENV1_SUSTAIN, P[t].getPatternElement(step).getSustain_fltr());
	  M[t]->setI(ADSR_ENV1_RELEASE, P[t].getPatternElement(step).getRelease_fltr());



	  M[t]->setI(OSC12_MIX,P[t].getPatternElement(step).getVCOMix());
	  M[t]->setI(OSC1_PHASE,P[t].getPatternElement(step).getPhaseOsc1());

	  M[t]->setI(LFO1_DEPTH,P[t].getPatternElement(step).getLfoDepth());
	  M[t]->setF(LFO1_FREQ,P[t].getPatternElement(step).getLfoSpeed());
	  //M[t]->set(OSC12_MIX,phase);

	  M[t]->setI(OSC1_TYPE,P[t].getPatternElement(step).getOscillatorOneType());
	  M[t]->setI(OSC2_TYPE,P[t].getPatternElement(step).getOscillatorTwoType());

	  M[t]->setI(OSC1_AMP,P[t].getPatternElement(step).getOsc1Amp());
	  M[t]->setI(OSC2_AMP,P[t].getPatternElement(step).getOsc2Amp());


	  M[t]->setI(FILTER1_TYPE,P[t].getPatternElement(step).getFilterType());
	  M[t]->setI(FILTER1_ALGO,P[t].getPatternElement(step).getFilterAlgo());


	  M[t]->setI(FILTER1_CUTOFF,P[t].getPatternElement(step).getCutoff());
	  M[t]->setI(FILTER1_RES,P[t].getPatternElement(step).getResonance());




	  M[t]->setI(NOTE_ON,1);

	}
      else
	{
	  if (noteOffTrigger[t]<0)
	    {
	      printf("$$$NOTEOFF\n");
	      //M[t]->getADSRAmp().setNoteOff();
	      //M[t]->getADSRFltr().setNoteOff();
	      //M[t]->reset();

	      M[t]->setI(NOTE_ON,0);
	    }
	  else
	    {
	      noteOffTrigger[t]=noteOffTrigger[t]-1;
	      printf("noteOffTrigger[%d]=%d\n",noteOffTrigger[t],t);
	    }
	}
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
  int p;

  string fileName="data.pic";

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

  /*
  for (t=0;t<TRACK_MAX;t++)
    for (p=0;p<MAX_PATTERN_BY_PROJECT;p++)
	PR.readPatternData(p,t,P[t]);
  */
  /*
  for (t=0;t<TRACK_MAX;t++)
    {
      PR.readPatternData(0,t,P[t]);
      
      // Ugly hack for BPM management
      bpm_current=P[t].getBPM();
      current_swing=P[t].getSwing();
      refresh_bpm();

      SEQ.getPatternSequencer(t).setBPMDivider(P[t].getBPMDivider());
    }
  */
    //PR.readPatternData(1,i+1,P[i]);

}


void wtg()
{
  Generator G;
  WaveTableManager & WTM = WaveTableManager::getInstance();
  WaveTable* WT;

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.noise();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_NOISE);


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

  printf("wavetablemanager.getSize : %d\n",WTM.getSize());

}



int main(int argc,char **argv)
{
  int i;

  wtg();
  //  exit(0);
  //NF.init();

  NoteFreq & NF = NoteFreq::getInstance();
  NF.init();

  IE.init();
  IE.printState();
  //IE.handle_key();
  //IE.init();
  //IE.printState();
  //exit(0);
  for (i=0;i<TRACK_MAX;i++)
    noteOffTrigger[i]=0;

  PR.init();         // Init the     storage bank
  PR.setBank(bank);  // The current  storage bank will be 0 PWD/bank/bank%d/

  load_pattern();
  printf("[openVideo output]\n");
  SG.initVideo();
  //SDL_InitSubSystem(SDL_INIT_AUDIO);
  //handle_key();
  //  SDL_EnableKeyRepeat(500,500);
  SG.openBMPFont();
  if (SG.openTTFFont()==false) { printf("ttf font error\n"); exit(1); }
  SG.loadingScreen();
  //sleep(10);
  //AE.setupSequencerCallback(printme);
  refresh_bpm();
  AE.setupSequencerCallback(seq_callback_update_step);
  AE.openAudio();

  //display_board();

  printf("[openAudio output]\n");


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
