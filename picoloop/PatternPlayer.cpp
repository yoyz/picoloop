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
//PSP_MODULE_INFO("TESTPRX", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
//PSP_HEAP_SIZE_MAX();
//PSP_HEAP_SIZE_KB(16384);
//PSP_HEAP_SIZE_KB(-1024);
//PSP_HEAP_SIZE_KB(20480) ;
//PSP_HEAP_SIZE_KB(-1024) ;
PSP_HEAP_SIZE_KB(-4096) ;
#endif

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
#include "TweakableKnob.h"

#define KEY_REPEAT_INTERVAL 400



#define DISABLEDBOX_COLOR   0x0AF0FE
#define BOX_COLOR           0xAECD15
#define TRIG_COLOR          0x0E4C15
#define NOTE_COLOR          0x46DC65
#define CURSOR_COLOR        0x1515CD
#define STEP_COLOR          0x242C45

#define SMALLBOX_COLOR      0x442233

#define DISABLE             0
#define ENABLE              1



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
  MENU_LFO_LFOPITCH,
  MENU_LFO_PITCHBEND,
  MENU_LFO_TYPE,
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

int running;

int save=false;
int load=false;

int saveall=false;
int loadall=false;

int patternRemove=false;

int bpm_current=120;    // current value for the four ( TRACK_MAX ) tracks


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

//int start_key=0;        // start key pressed ?
//int step=0;             // current step in the sequencer

int menu_cursor=GLOBALMENU_AD;      // index int the menu
int menu=MENU_ON_PAGE1;             // menu mode
int menu_note=ENABLE;
int menu_ad=MENU_AD_AMP_ATTACK_RELEASE;
int menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;
int menu_fx=MENU_FX_DEPTH_SPEED;
int menu_vco=MENU_VCO_OSCMIX_PHASE;
int menu_lfo=MENU_LFO_LFOPITCH;
int menu_ls=MENU_LS_PATTERN;

int menu_ad_dirty_keyboard=0;

//int ct=0;               // current_track
//int ct_x=0;             
//int ct_y=0;


int dirty_graphic=1;           // 1 : the UI need to be updated

int seq_update_by_step_next=0; // 0 : the UI audio and seq are sync
                               // 1 : the audio callback update the seq, 
                               //   : other stuff need to be updated 
                               //   : seq_update_by_step(), load,save...

int current_swing=50;  
//int current_swing=64;


//int noteOffTrigger[TRACK_MAX];

void update_SAMM(int trackNumber,int stepNumber)
{
  SAMM.setMachineType(P[trackNumber].getPatternElement(stepNumber).get(MACHINE_TYPE));	  
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

  nb_cb_ch_step=(60*DEFAULTFREQ)/(BUFFER_FRAME*4*bpm_current);
  nb_tick_before_step_change=(60*DEFAULTFREQ)/(bpm_current*4);
  refresh_swing();
}

//char * tmp_str;


void display_board_two_param(int machineParam1,int machineParam2)
{
  int i;
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
	  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	  
	  // LFO
	  SG.smallBoxNumber(i,P[cty].getPatternElement(i).get(machineParam1),128,SMALLBOX_COLOR);
	  SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).get(machineParam2),SMALLBOX_COLOR);
	}
    }  
}


void display_board_amp_env()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  // Attack/Release
  if (menu_cursor==GLOBALMENU_AD)
    {

      if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE)
	display_board_two_param(ADSR_AMP_RELEASE,ADSR_AMP_ATTACK);

      if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)
	display_board_two_param(ADSR_AMP_DECAY,ADSR_AMP_SUSTAIN);

      if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE)
	display_board_two_param(ADSR_FLTR_RELEASE,ADSR_FLTR_ATTACK);

      if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN)
	display_board_two_param(ADSR_FLTR_DECAY,ADSR_FLTR_SUSTAIN);

      if (menu_ad==MENU_AD_TRIGTIME_AMP)
	display_board_two_param(AMP,TRIG_TIME_DURATION);	
    }
}


void display_board_note()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  NoteFreq & NF = NoteFreq::getInstance();
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
	      if (P[cty].getPatternElement(i).get(NOTE_ON))
		{
		  SG.drawBoxNumber(i,NOTE_COLOR);
		  
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		    //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);
		  //  SG.drawTTFTextNumberFirstLine(i,P[cty].getPatternElement(i).getNoteCharStar());
		  SG.drawTTFTextNumberFirstLine(i,NF.getNoteCharStar(P[cty].getPatternElement(i).get(NOTE)));
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
	      if (P[cty].getPatternElement(i).get(NOTE_ON))
		  {
		  SG.drawBoxNumber(i,NOTE_COLOR);	    
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  

		  //		  if (i==SEQ.getPatternSequencer(cty).getStep())
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  

		  SG.smallBoxNumber(i,
				    (P[cty].getPatternElement(i).get(NOTE)%12)*10,
				    (128-(P[cty].getPatternElement(i).get(NOTE)/12)*10),
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
      display_board_two_param(LFO1_DEPTH,LFO1_FREQ);
      // // Cursor & step postion      
      // SG.drawBoxNumber(cursor,CURSOR_COLOR);
      // SG.drawBoxNumber(step,STEP_COLOR);  
      // //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
      
      // //if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE_)
      // //	{
      // for (i=0;i<16;i++)
      // 	{
      // 	  // Draw trigged box trig color   
      // 	  if (P[cty].getPatternElement(i).get(NOTE_ON))
      // 	    {
      // 	      SG.drawBoxNumber(i,TRIG_COLOR);
      // 	      if (i==cursor)
      // 		SG.drawBoxNumber(cursor,CURSOR_COLOR);
      // 	      if (i==step)
      // 		SG.drawBoxNumber(step,STEP_COLOR);  
      // 		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	      
      // 		  // LFO
      // 	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).get(LFO1_DEPTH),128,SMALLBOX_COLOR);
      // 	      SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).get(LFO1_FREQ),SMALLBOX_COLOR);
      // 	    }
      // 	}
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
	    //printf("!!!!!!!!!!!!!!!!!!!!!!!x:%d y:%d\n",x,y);
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
	  if (P[cty].getPatternElement(i).get(NOTE_ON))
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	      
		  // LFO
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).get(LFO1_DEPTH),128,SMALLBOX_COLOR);
	      SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).get(LFO1_DEPTH),SMALLBOX_COLOR);
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
	  if (P[cty].getPatternElement(i).get(NOTE_ON))
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	      
		  // LFO
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).get(LFO1_DEPTH),128,SMALLBOX_COLOR);
	      SG.smallBoxNumber(i,0,128-P[cty].getPatternElement(i).get(LFO1_FREQ),SMALLBOX_COLOR);
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
      display_board_two_param(VCO_MIX,OSC1_PHASE);
    }

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCAMP)
    {
       display_board_two_param(OSC1_AMP,OSC2_AMP);
    }

  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_FMTYPE)
    {	  
      SG.drawBoxNumber(step,STEP_COLOR);  
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      
      for (i=0;i<16;i++)
	{
	  // Draw trig note color
	  if (P[cty].getPatternElement(i).get(NOTE_ON))
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

  // LFOPITCH

  if (menu_cursor==GLOBALMENU_LFO &&
      menu_lfo   ==MENU_LFO_LFOPITCH)
    {

      display_board_two_param(LFO1_DEPTH,LFO1_FREQ);
    }

  // PITCHBEND

  if (menu_cursor==GLOBALMENU_LFO &&
      menu_lfo   ==MENU_LFO_PITCHBEND)
    {
      display_board_two_param(PITCHBEND_DEPTH,PITCHBEND_SPEED);
    }

  if (menu_lfo==MENU_LFO_TYPE)
    {
      // Cursor & step postion      
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.drawBoxNumber(step,STEP_COLOR);  
      
      for (i=0;i<16;i++)
	{
	  // Draw trigged box trig color   
	  if (P[cty].getPatternElement(i).get(NOTE_ON))
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  
	      
	      SG.drawTTFTextNumberFirstLine(i,P[cty].getPatternElement(i).getLFOTypeCharStar()); 
	      //SG.drawTTFTextNumberSecondLine(i,P[cty].getPatternElement(i).getFilterTypeCharStar()); 
	    }
	}
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
	  if (P[cty].getPatternElement(i).get(NOTE_ON))
	    {
	      SG.drawBoxNumber(i,TRIG_COLOR);
	      if (i==cursor)       SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)         SG.drawBoxNumber(step,STEP_COLOR);  
	      //SG.drawTTFTextNumberFirstLine(i, P[cty].getPatternElement(i).getOscOneTypeCharStar());
	      //SG.drawTTFTextNumberSecondLine(i,P[cty].getPatternElement(i).getOscTwoTypeCharStar());
	      update_SAMM(cty,i);
	      SG.drawTTFTextNumberFirstLine(i, SAM->getMachineParamCharStar(OSC1_TYPE,P[cty].getPatternElement(i).get(OSC1_TYPE)));
	      SG.drawTTFTextNumberSecondLine(i, SAM->getMachineParamCharStar(OSC2_TYPE,P[cty].getPatternElement(i).get(OSC2_TYPE)));
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


void display_board_fltr()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_FLTR)
    {

      if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)
	{
	  display_board_two_param(FILTER1_RESONANCE,FILTER1_CUTOFF);
	}
      if (menu_fltr==MENU_FLTR_ALGO_TYPE)
	{
	  // Cursor & step postion      
	  SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  SG.drawBoxNumber(step,STEP_COLOR);  
	  
	  for (i=0;i<16;i++)
	    {
	      // Draw trigged box trig color   
	      if (P[cty].getPatternElement(i).get(NOTE_ON))
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
	  display_board_two_param(FX1_DEPTH,FX1_SPEED);
	}
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



  if (menu_lfo==MENU_LFO_LFOPITCH &&
      menu_cursor==GLOBALMENU_LFO
      )
    {
      sprintf(str_submenu,"LFOPitch Depth/Speed");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_lfo==MENU_LFO_PITCHBEND &&
      menu_cursor==GLOBALMENU_LFO
      )
    {
      sprintf(str_submenu,"PitchBend Depth/Speed");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  if (menu_lfo==MENU_LFO_TYPE &&
      menu_cursor==GLOBALMENU_LFO
      )
    {
      sprintf(str_submenu,"LFOType");
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

  int last_menu=0xffff;
  int last_menu_cursor=0xffff;

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

	}
      
      // We exit the LS Menu so we had to 
      // sync SEQ.Y with the cursor in the load save screen
      // save the song to a file
      if (last_menu        != MENU_OFF &&
	  menu             == MENU_OFF &&
	  menu_cursor      == GLOBALMENU_LS)
	{
	  SEQ.setCurrentTrackY(loadsave_cursor_y);
	  PR.saveSong(SEQ.getSongSequencer());
	}

      dirty_graphic=1;
      IE.clearLastKeyEvent();
      printf("[gmenu : %d cmenu : %d]\n",menu,menu_cursor);
    }

  //We enter the LS Menu so we had to sync the cursor with SEQ 
  if (lastKey      ==  BUTTON_B       && 
      lastEvent    ==  KEYRELEASED    &&
      (menu        ==  MENU_ON_PAGE1  ||
       menu        ==  MENU_ON_PAGE2))
    {
      last_menu        = menu;
      menu=MENU_OFF;
      loadsave_cursor_y=SEQ.getCurrentTrackY();
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
	  { TK.set(ADSR_AMP_RELEASE,-1);   dirty_graphic=1; }
        //{ TK.release_amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_AMP_RELEASE,1);   dirty_graphic=1; }	  
	  //{ TK.release_amp=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_AMP_ATTACK,1);   dirty_graphic=1; }	  
	  //{ TK.attack_amp=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_AMP_ATTACK,-11);   dirty_graphic=1; }	  
      //{ TK.attack_amp=-1; 	  dirty_graphic=1; }
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
	  { TK.set(ADSR_AMP_DECAY,-1);   dirty_graphic=1; }
	  //{ TK.decay_amp=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_AMP_DECAY,1);    dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_AMP_SUSTAIN,1);  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_AMP_SUSTAIN,-1); dirty_graphic=1; }
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
	  { TK.setAll(ADSR_AMP_RELEASE,-1); dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_AMP_RELEASE,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_AMP_ATTACK,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_AMP_ATTACK,-1);  	  dirty_graphic=1; }
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
	  { TK.setAll(ADSR_AMP_DECAY,-1); dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_AMP_DECAY,1);  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_AMP_SUSTAIN,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_AMP_SUSTAIN,-1); 	  dirty_graphic=1; }
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
	  { TK.set(ADSR_FLTR_RELEASE,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_FLTR_RELEASE,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_FLTR_ATTACK,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_FLTR_ATTACK,-1); 	  dirty_graphic=1; }
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
	  { TK.set(ADSR_FLTR_DECAY,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_FLTR_DECAY,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_FLTR_SUSTAIN,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(ADSR_FLTR_SUSTAIN,-1); 	  dirty_graphic=1; }
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
	  { TK.setAll(ADSR_FLTR_RELEASE,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_FLTR_RELEASE,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_FLTR_ATTACK,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_FLTR_ATTACK,-1); 	  dirty_graphic=1; }
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
	  { TK.setAll(ADSR_FLTR_DECAY,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_FLTR_DECAY,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_FLTR_SUSTAIN,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(ADSR_FLTR_SUSTAIN,-1); 	  dirty_graphic=1; }
    }  




  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      if (lastKey   == BUTTON_A && 
	  lastEvent == KEYPRESSED)
	{
	  sub_handle_invert_trig();
	  // TK.invert_trig=1;
	  // printf("key lalt\n");      
	  // dirty_graphic=1;
	  // IE.clearLastKeyEvent();
	}
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(AMP,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(AMP,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(TRIG_TIME_DURATION,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(TRIG_TIME_DURATION,-1); 	  dirty_graphic=1; }
    }  


  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 ||  keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(AMP,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(AMP,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(TRIG_TIME_DURATION,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(TRIG_TIME_DURATION,-1); 	  dirty_graphic=1; }
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
	  lastEvent ==  KEYRELEASED)
	{
	  invert_trig=1;
	  printf("key lalt\n");      
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	}
      */
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(NOTE,-1); 	  dirty_graphic=1;}

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(NOTE,1);  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(NOTE,12);   	  dirty_graphic=1;}

      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(NOTE,-12);  	  dirty_graphic=1;}
    }  

    // GLOBALMENU_NOTE
  // change note
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {
      
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(NOTE,-1); 	  dirty_graphic=1;}

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(NOTE,1);  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(NOTE,12);   	  dirty_graphic=1;}

      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(NOTE,-12);  	  dirty_graphic=1;}
    }  


  // change note from box to value e.g C3 D4...
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
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
	  { TK.setAll(OSC1_TYPE,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(OSC1_TYPE,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(OSC2_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(OSC2_TYPE,-1);  	  dirty_graphic=1;}
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(OSC1_TYPE,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(OSC1_TYPE,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(OSC2_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(OSC2_TYPE,-1);  	  dirty_graphic=1;}
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
	  { TK.set(FX1_DEPTH,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(FX1_DEPTH,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(FX1_SPEED,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.set(FX1_SPEED,-1);  	  dirty_graphic=1;}
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
	  { TK.setAll(FX1_DEPTH,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(FX1_DEPTH,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(FX1_SPEED,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST) 
	  { TK.setAll(FX1_SPEED,-1);  	  dirty_graphic=1;}
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
	  { TK.set(VCO_MIX,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.set(VCO_MIX,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.set(OSC1_PHASE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.set(OSC1_PHASE,-1);  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(VCO_MIX,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(VCO_MIX,1);  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(OSC1_PHASE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(OSC1_PHASE,-1);  	  dirty_graphic=1;}
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
	  { TK.set(OSC1_AMP,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.set(OSC1_AMP,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.set(OSC2_AMP,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.set(OSC2_AMP,-1);  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCAMP
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(OSC1_AMP,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(OSC1_AMP,1);  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(OSC2_AMP,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALLEST==0) 
	  { TK.setAll(OSC2_AMP,-1);  	  dirty_graphic=1;}
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
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(FM_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(FM_TYPE,-1);  	  dirty_graphic=1;}
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_FMTYPE
      )
    {

      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(FM_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.setAll(FM_TYPE,-1);  	  dirty_graphic=1;}

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
	  { TK.set(MACHINE_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.set(MACHINE_TYPE,-1);  	  dirty_graphic=1;}
    }

  // GLOBALMENU_MAC
  // change machine type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_MAC )
    {
      
      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.setAll(MACHINE_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.setAll(MACHINE_TYPE,-1);  	  dirty_graphic=1;}
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

  // GLOBALMENU_LFO
  // LFO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(LFO1_DEPTH,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(LFO1_DEPTH,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(LFO1_FREQ,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(LFO1_FREQ,-1);  	  dirty_graphic=1;}
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(LFO1_DEPTH,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(LFO1_DEPTH,1);  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(LFO1_FREQ,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(LFO1_FREQ,-1);  	  dirty_graphic=1;}
      //????
    }

  // GLOBALMENU_LFO
  // LFO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(PITCHBEND_DEPTH,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(PITCHBEND_DEPTH,1);  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(PITCHBEND_SPEED,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.set(PITCHBEND_SPEED,-1);  	  dirty_graphic=1;}
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(PITCHBEND_DEPTH,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(PITCHBEND_DEPTH,1);  	  dirty_graphic=1;}

      // ????
      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(PITCHBEND_SPEED,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(PITCHBEND_SPEED,-1);  	  dirty_graphic=1;}
      //????
    }


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {


      if (keyState[BUTTON_UP]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(LFO_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONG==0) 
	  { TK.set(LFO_TYPE,-1);  	  dirty_graphic=1;}

    }



  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {


      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(LFO_TYPE,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_SMALL==0) 
	  { TK.setAll(LFO_TYPE,-1);  	  dirty_graphic=1;}

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
	  { TK.setAll(PATTERN_SHIFT,-1); 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.setAll(PATTERN_SHIFT,1);  	  dirty_graphic=1;}

      if (keyState[BUTTON_UP]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.setAll(PATTERN_LENGTH,1);   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.setAll(PATTERN_LENGTH,-1);  	  dirty_graphic=1;}
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST )  
	  { TK.set(FILTER1_RESONANCE,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST )  
	  { TK.set(FILTER1_RESONANCE,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST ) 
	  { TK.set(FILTER1_CUTOFF,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST ) 
	  { TK.set(FILTER1_CUTOFF,-1); 	  dirty_graphic=1; }
    }  

  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      if (keyState[BUTTON_LEFT]  && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST )  
	  { TK.setAll(FILTER1_RESONANCE,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST )  
	  { TK.setAll(FILTER1_RESONANCE,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST ) 
	  { TK.setAll(FILTER1_CUTOFF,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST ) 
	  { TK.setAll(FILTER1_CUTOFF,-1); 	  dirty_graphic=1; }
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
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_LONG )  
	  { TK.set(FILTER1_ALGO,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_LONG )  
	  { TK.set(FILTER1_ALGO,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_LONG) 
	  { TK.set(FILTER1_TYPE,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_LONG ) 
	  { TK.set(FILTER1_TYPE,-1); 	  dirty_graphic=1; }
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
	  if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]>KEY_REPEAT_INTERVAL_SMALLEST )  
	    { TK.setAll(FILTER1_ALGO,-1);   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>KEY_REPEAT_INTERVAL_SMALLEST )  
	  { TK.setAll(FILTER1_ALGO,1); 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]>KEY_REPEAT_INTERVAL_SMALLEST ) 
	  { TK.setAll(FILTER1_TYPE,1);  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>KEY_REPEAT_INTERVAL_SMALLEST ) 
	  { TK.setAll(FILTER1_TYPE,-1); 	  dirty_graphic=1; }
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
	  { TK.set(BPM,-1); 	  dirty_graphic=1; printf("[B+LEFT  t=%d]\n",bpm_current); }

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.set(BPM,1); 	  dirty_graphic=1; printf("[B+RIGHT t=%d]\n",bpm_current);}

      if (keyState[BUTTON_DOWN] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.set(BPM,-10); 	  dirty_graphic=1; printf("[B+DOWN  t=%d]\n",bpm_current); }

      if (keyState[BUTTON_UP] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.set(BPM,10); 	  dirty_graphic=1; printf("[B+UP    t=%d]\n",bpm_current);}


      //if (bpm < 20) bpm=20;
      //if (bpm > 260) bpm=260;

      //nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*bpm);
    }  

  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {

      if (keyState[BUTTON_DOWN] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.set(BPM_DIVIDER,-1); 	  dirty_graphic=1; printf("[A+DOWN  t=%d] divider\n",TK.get(BPM_DIVIDER)); }

      if (keyState[BUTTON_UP] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.set(BPM_DIVIDER,1); 	  dirty_graphic=1; printf("[A+UP    t=%d] divider\n",TK.get(BPM_DIVIDER));}
    }  


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_BPM )
    {

      if (keyState[BUTTON_LEFT] && keyState[BUTTON_A])
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONGEST==0)
	  { TK.set(SWING,-1); 	  dirty_graphic=1; printf("[A+DOWN  t=%d] swing\n",current_swing); }

      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_A]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%KEY_REPEAT_INTERVAL_LONGEST==0) 
	  { TK.set(SWING,1); 	  dirty_graphic=1; printf("[A+UP    t=%d] swing\n",current_swing);}
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
	      { song_cursor_x++; dirty_graphic=1; SEQ.getSongSequencer().setPatternNumber(song_cursor_x,song_cursor_y,SEQ.getSongSequencer().getPatternNumber(song_cursor_x-1,song_cursor_y));}

	  if (keyState[BUTTON_LEFT])
	    if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%KEY_REPEAT_INTERVAL_LONG==0) 
	      { song_cursor_x--; dirty_graphic=1; SEQ.getSongSequencer().setPatternNumber(song_cursor_x,song_cursor_y,SEQ.getSongSequencer().getPatternNumber(song_cursor_x+1,song_cursor_y));}

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
	  lastEvent   == KEYRELEASED   &&
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
      lastEvent   ==  KEYRELEASED     && 
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
      lastEvent   ==  KEYRELEASED     && 
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

void handle_key_submenu_lfo()
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
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_LFO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_lfo==MENU_LFO_LFOPITCH)               { menu_lfo=MENU_LFO_PITCHBEND;       }
	  else if (menu_lfo==MENU_LFO_PITCHBEND)              { menu_lfo=MENU_LFO_TYPE;            } 
	  else if (menu_lfo==MENU_LFO_TYPE)                   { menu_lfo=MENU_LFO_LFOPITCH;        }  
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu lfo : %d]\n",menu_lfo);
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
      lastEvent   ==  KEYRELEASED     && 
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
  //printf("%d %d %d\n",lastKey,lastEvent,lastKey==&& BUTTON_START && lastEvent==KEYRELEASED);
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
    { handle_key_lfo(); handle_key_submenu_lfo();    }
  if (menu_cursor==GLOBALMENU_FLTR) 
    { handle_key_fltr(); handle_key_submenu_fltr();  }

  if (menu_cursor==GLOBALMENU_LS)   handle_key_load_save();
  if (menu_cursor==GLOBALMENU_BANK) handle_key_bank();
  if (menu_cursor==GLOBALMENU_PSH)  handle_key_psh();
  if (menu_cursor==GLOBALMENU_MAC)  handle_key_mac();
  if (menu_cursor==GLOBALMENU_FX)   handle_key_fx();
  if (menu_cursor==GLOBALMENU_BPM)  handle_key_bpm();
  
 
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
  if (TK.get(AMP)!=0)
    {
      P[cty].getPatternElement(cursor).set(AMP,
					   P[cty].getPatternElement(cursor).get(AMP)+TK.get(AMP));
      TK.set(AMP,0);
      if (debug)
	printf("[amp:%d]\n",P[cty].getPatternElement(cursor).get(AMP));
    }
  
  // Change amp Amplification
  if (TK.getAll(AMP)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(AMP,
					P[cty].getPatternElement(i).get(AMP)+TK.getAll(AMP));
      TK.setAll(AMP,0);
      if (debug)
	printf("[amp_all:%d]\n",P[cty].getPatternElement(cursor).get(AMP));
    }
  
  // Change amp env Attack
  if (TK.get(ADSR_AMP_ATTACK)!=0)
    {
      //P[cty].getPatternElement(cursor).setAttack_amp(P[cty].getPatternElement(cursor).getAttack_amp()+TK.attack_amp);
      P[cty].getPatternElement(cursor).set(ADSR_AMP_ATTACK,
					   P[cty].getPatternElement(cursor).get(ADSR_AMP_ATTACK)+TK.get(ADSR_AMP_ATTACK));
      TK.set(ADSR_AMP_ATTACK,0);
      if (debug)
	printf("[attack_amp:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_ATTACK));
    }
  
  // Change amp env Attack
  if (TK.getAll(ADSR_AMP_ATTACK)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_AMP_ATTACK,
					P[cty].getPatternElement(i).get(ADSR_AMP_ATTACK)+TK.getAll(ADSR_AMP_ATTACK));

      TK.setAll(ADSR_AMP_ATTACK,0);
      if (debug)
	printf("[attack_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_ATTACK));
    }


  // Change amp env Decay
  if (TK.get(ADSR_AMP_DECAY)!=0)
    {
      P[cty].getPatternElement(cursor).set(ADSR_AMP_DECAY,
					   P[cty].getPatternElement(cursor).get(ADSR_AMP_DECAY)+TK.get(ADSR_AMP_DECAY));
      TK.set(ADSR_AMP_DECAY,0);
      if (debug)
	printf("[decay_amp:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_DECAY));
    }
  
  // Change amp env Decay
  if (TK.getAll(ADSR_AMP_DECAY)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_AMP_DECAY,
					P[cty].getPatternElement(i).get(ADSR_AMP_DECAY)+TK.getAll(ADSR_AMP_DECAY));
      TK.setAll(ADSR_AMP_DECAY,0);
      if (debug)
	printf("[decay_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_DECAY));
    }



  // Change amp env Sustain
  if (TK.get(ADSR_AMP_SUSTAIN)!=0)
    {
      P[cty].getPatternElement(cursor).set(ADSR_AMP_SUSTAIN,
					   P[cty].getPatternElement(cursor).get(ADSR_AMP_SUSTAIN)+TK.get(ADSR_AMP_SUSTAIN));
      TK.set(ADSR_AMP_SUSTAIN,0);
      if (debug)
	printf("[sustain_amp:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_SUSTAIN));
    }
  
  // Change amp env Sustain
  if (TK.getAll(ADSR_AMP_SUSTAIN)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_AMP_SUSTAIN,
					P[cty].getPatternElement(i).get(ADSR_AMP_SUSTAIN)+TK.getAll(ADSR_AMP_SUSTAIN));
      TK.setAll(ADSR_AMP_SUSTAIN,0);
      if (debug)
	printf("[sustain_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_SUSTAIN));
    }
  
  
  // Change amp env Release
  if (TK.get(ADSR_AMP_RELEASE)!=0)
    {
      P[cty].getPatternElement(cursor).set(ADSR_AMP_RELEASE,
					   P[cty].getPatternElement(cursor).get(ADSR_AMP_RELEASE)+TK.get(ADSR_AMP_RELEASE));
      TK.set(ADSR_AMP_RELEASE,0);
      if (debug)
	printf("[release_amp:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_RELEASE));
      
    }
  
  // Change amp env Release
  if (TK.getAll(ADSR_AMP_RELEASE)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_AMP_RELEASE,
					P[cty].getPatternElement(i).get(ADSR_AMP_RELEASE)+TK.getAll(ADSR_AMP_RELEASE));
      TK.setAll(ADSR_AMP_RELEASE,0);
      if (debug)
	printf("[release_amp_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_AMP_RELEASE));
      
    }
  




  // Change fltr env Attack
  if (TK.get(ADSR_FLTR_ATTACK)!=0)
    {
      P[cty].getPatternElement(cursor).set(ADSR_FLTR_ATTACK,
					   P[cty].getPatternElement(cursor).get(ADSR_FLTR_ATTACK)+TK.get(ADSR_FLTR_ATTACK));
      TK.set(ADSR_FLTR_ATTACK,0);
      if (debug)
	printf("[attack_fltr:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_ATTACK));
    }
  
  // Change fltr env Attack
  if (TK.getAll(ADSR_FLTR_ATTACK)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_FLTR_ATTACK,P[cty].getPatternElement(i).get(ADSR_FLTR_ATTACK)+TK.getAll(ADSR_FLTR_ATTACK));
      TK.setAll(ADSR_FLTR_ATTACK,0);
      if (debug)
	printf("[attack_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_ATTACK));
    }


  // Change fltr env Decay
  if (TK.get(ADSR_FLTR_DECAY)!=0)
    {
      P[cty].getPatternElement(cursor).set(ADSR_FLTR_DECAY,
					   P[cty].getPatternElement(cursor).get(ADSR_FLTR_DECAY)+TK.get(ADSR_FLTR_DECAY));
      TK.set(ADSR_FLTR_DECAY,0);
      if (debug)
	printf("[decay_fltr:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_DECAY));
    }
  
  // Change fltr env Decay
  if (TK.getAll(ADSR_FLTR_DECAY)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_FLTR_DECAY,
					P[cty].getPatternElement(i).get(ADSR_FLTR_DECAY)+TK.getAll(ADSR_FLTR_DECAY));
      TK.setAll(ADSR_FLTR_DECAY,0);
      if (debug)
	printf("[decay_fltr_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_DECAY));
    }


  // Change fltr env Sustain
  if (TK.get(ADSR_FLTR_SUSTAIN)!=0)
    {
      P[cty].getPatternElement(cursor).set(ADSR_FLTR_SUSTAIN,
					   P[cty].getPatternElement(cursor).get(ADSR_FLTR_SUSTAIN)+TK.get(ADSR_FLTR_SUSTAIN));
      TK.set(ADSR_FLTR_SUSTAIN,0);
      if (debug)
	printf("[sustain_fltr:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_SUSTAIN));
    }
  
  // Change fltr env Sustain
  if (TK.getAll(ADSR_FLTR_SUSTAIN)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_FLTR_SUSTAIN,
					P[cty].getPatternElement(i).get(ADSR_FLTR_SUSTAIN)+TK.getAll(ADSR_FLTR_SUSTAIN));
      TK.setAll(ADSR_FLTR_SUSTAIN,0);
      if (debug)
	printf("[sustain_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_SUSTAIN));
    }

  
  
  // Change fltr env Release
  if (TK.get(ADSR_FLTR_RELEASE)!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).set(ADSR_FLTR_RELEASE,
					   P[cty].getPatternElement(cursor).get(ADSR_FLTR_RELEASE)+TK.get(ADSR_FLTR_RELEASE));
      TK.set(ADSR_FLTR_RELEASE,0);
      if (debug)
	printf("[release_fltr:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_RELEASE));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change fltr env Release
  if (TK.getAll(ADSR_FLTR_RELEASE)!=0)
    {
      for(i=0;i<16;i++)
	P[cty].getPatternElement(i).set(ADSR_FLTR_RELEASE,
					P[cty].getPatternElement(i).get(ADSR_FLTR_RELEASE)+TK.getAll(ADSR_FLTR_RELEASE));
      TK.setAll(ADSR_FLTR_RELEASE,0);
      if (debug)
	printf("[release_fltr_all:%d]\n",P[cty].getPatternElement(cursor).get(ADSR_FLTR_RELEASE));
      
    }
  

  // Change fltr type
  if (TK.get(FILTER1_TYPE)!=0)
    {
      update_SAMM(cty,cursor);
      P[cty].getPatternElement(cursor).set(FILTER1_TYPE,
					   SAM->checkI(FILTER1_TYPE,P[cty].getPatternElement(cursor).get(FILTER1_TYPE)+TK.get(FILTER1_TYPE)));
      TK.set(FILTER1_TYPE,0);
      if (debug)
	printf("[filter_type:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_TYPE));
    }
  
  // Change fltr
  if (TK.getAll(FILTER1_TYPE)!=0)
    {
      for(i=0;i<16;i++)
	{
	  update_SAMM(cty,i);
	  P[cty].getPatternElement(i).set(FILTER1_TYPE,
					  SAM->checkI(FILTER1_TYPE,P[cty].getPatternElement(i).get(FILTER1_TYPE)+TK.getAll(FILTER1_TYPE)));
	}
      TK.setAll(FILTER1_TYPE,0);
      if (debug)
	printf("[filter_type:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_TYPE));
    }

  // Change fltr env Attack
  if (TK.get(FILTER1_ALGO)!=0)
    {
      update_SAMM(cty,cursor);
      P[cty].getPatternElement(cursor).set(FILTER1_ALGO,
					   SAM->checkI(FILTER1_ALGO,P[cty].getPatternElement(cursor).get(FILTER1_ALGO)+TK.get(FILTER1_ALGO)));
      TK.set(FILTER1_ALGO,0);
      if (debug)
	printf("[filter_algo:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_ALGO));
    }
  
  // Change fltr env Attack
  if (TK.getAll(FILTER1_ALGO)!=0)
    {
      for(i=0;i<16;i++)
	{
	  update_SAMM(cty,i);
	  P[cty].getPatternElement(i).set(FILTER1_ALGO,
					  SAM->checkI(FILTER1_ALGO,P[cty].getPatternElement(i).get(FILTER1_ALGO)+TK.getAll(FILTER1_ALGO)));
	}
      TK.setAll(FILTER1_ALGO,0);
      if (debug)
	printf("[filter_algo:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_ALGO));
    }




  
  // Change VCOMix
  if (TK.get(VCO_MIX)!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).set(VCO_MIX,P[cty].getPatternElement(cursor).get(VCO_MIX)+TK.get(VCO_MIX));
      TK.set(VCO_MIX,0);
      if (debug)
	printf("[vcomix:%d]\n",P[cty].getPatternElement(cursor).get(VCO_MIX));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change VCOMix
  if (TK.getAll(VCO_MIX)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(VCO_MIX,
					P[cty].getPatternElement(i).get(VCO_MIX)+TK.getAll(VCO_MIX));
      TK.setAll(VCO_MIX,0);
      if (debug)
	printf("[vcomix_all:%d]\n",P[cty].getPatternElement(cursor).get(VCO_MIX));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }


  // Change Fx Depth
  if (TK.get(FX1_DEPTH)!=0)
    {
      P[cty].getPatternElement(cursor).set(FX1_DEPTH,P[cty].getPatternElement(cursor).get(FX1_DEPTH)+TK.get(FX1_DEPTH));
      TK.set(FX1_DEPTH,0);
      if (debug)
	printf("[fx_depth:%d]\n",P[cty].getPatternElement(cursor).get(FX1_DEPTH));      
    }
  
  // Change Fx Depth All
  if (TK.getAll(FX1_DEPTH)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(FX1_DEPTH,P[cty].getPatternElement(i).get(FX1_DEPTH)+TK.getAll(FX1_DEPTH));
      TK.setAll(FX1_DEPTH,0);
      if (debug)
	printf("[fx_depth_all:%d]\n",P[cty].getPatternElement(cursor).get(FX1_DEPTH));      
    }


  // Change Fx Speed
  if (TK.get(FX1_SPEED)!=0)
    {
      P[cty].getPatternElement(cursor).set(FX1_SPEED,
					   P[cty].getPatternElement(cursor).get(FX1_SPEED)+TK.get(FX1_SPEED));
      TK.set(FX1_SPEED,0);
      if (debug)
	printf("[fx_speed:%d]\n",P[cty].getPatternElement(cursor).get(FX1_SPEED));      
    }
  
  // Change Fx Speed
  if (TK.getAll(FX1_SPEED)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(FX1_SPEED,P[cty].getPatternElement(i).get(FX1_SPEED)+TK.getAll(FX1_SPEED));
      TK.setAll(FX1_SPEED,0);
      if (debug)
	printf("[fx_speed_all:%d]\n",P[cty].getPatternElement(cursor).get(FX1_SPEED));      
    }



  // Change osc1 amp
  if (TK.get(OSC1_AMP)!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).set(OSC1_AMP,
					   P[cty].getPatternElement(cursor).get(OSC1_AMP)+TK.get(OSC1_AMP));
      TK.set(OSC1_AMP,0);
      if (debug)
	printf("[osc1_amp:%d]\n",P[cty].getPatternElement(cursor).get(OSC1_AMP));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change osc1 amp all
  if (TK.getAll(OSC1_AMP)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(OSC1_AMP,
					P[cty].getPatternElement(i).get(OSC1_AMP)+TK.getAll(OSC1_AMP));
      TK.setAll(OSC1_AMP,0);
      if (debug)
	printf("[osc1_amp_all:%d]\n",P[cty].getPatternElement(cursor).get(OSC1_AMP));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }

  // Change osc2 amp
  if (TK.get(OSC2_AMP)!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).set(OSC2_AMP,
					   P[cty].getPatternElement(cursor).get(OSC2_AMP)+TK.get(OSC2_AMP));
      TK.set(OSC2_AMP,0);
      if (debug)
	printf("[osc2_amp:%d]\n",P[cty].getPatternElement(cursor).get(OSC2_AMP));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }
  
  // Change osc2 amp all
  if (TK.getAll(OSC2_AMP)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(OSC2_AMP,
					P[cty].getPatternElement(i).get(OSC2_AMP)+TK.getAll(OSC2_AMP));
      TK.setAll(OSC2_AMP,0);
      if (debug)
	printf("[osc2_amp_all:%d]\n",P[cty].getPatternElement(cursor).get(OSC2_AMP));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }





  // Change trig time
  if (TK.get(TRIG_TIME_DURATION)!=0)
    {
      P[cty].getPatternElement(cursor).set(TRIG_TIME_DURATION,
					   P[cty].getPatternElement(cursor).get(TRIG_TIME_DURATION)+TK.get(TRIG_TIME_DURATION));
      TK.set(TRIG_TIME_DURATION,0);
      if (debug)
	printf("[trig_time:%d]\n",P[cty].getPatternElement(cursor).get(TRIG_TIME_DURATION));
    }
  
  // Change all trig time
  if (TK.getAll(TRIG_TIME_DURATION)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(TRIG_TIME_DURATION,
					P[cty].getPatternElement(i).get(TRIG_TIME_DURATION)+TK.getAll(TRIG_TIME_DURATION));
      TK.setAll(TRIG_TIME_DURATION,0);
      if (debug)
	printf("[trig_time_all:%d]\n",P[cty].getPatternElement(cursor).get(TRIG_TIME_DURATION));      
    }




  // Change phase osc1
  if (TK.get(OSC1_PHASE)!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[cty].getPatternElement(cursor).set(OSC1_PHASE,
					   P[cty].getPatternElement(cursor).get(OSC1_PHASE)+TK.get(OSC1_PHASE));
      TK.set(OSC1_PHASE,0);
      if (debug)
	printf("[phase_osc1:%d]\n",P[cty].getPatternElement(cursor).get(OSC1_PHASE));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }

  // Change phase osc1
  if (TK.getAll(OSC1_PHASE)!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(OSC1_PHASE,
					P[cty].getPatternElement(i).get(OSC1_PHASE)+TK.getAll(OSC1_PHASE));
      TK.setAll(OSC1_PHASE,0);
      if (debug)
	printf("[phase_osc1_all:%d]\n",P[cty].getPatternElement(cursor).get(OSC1_PHASE));
      //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
      
    }



  // Change lfo depth
  if (TK.get(LFO1_DEPTH)!=0)
    {      
      P[cty].getPatternElement(cursor).set(LFO1_DEPTH,
					   P[cty].getPatternElement(cursor).get(LFO1_DEPTH)+TK.get(LFO1_DEPTH));
      TK.set(LFO1_DEPTH,0);
      if (debug)
	printf("[lfo_depth:%d]\n",P[cty].getPatternElement(cursor).get(LFO1_DEPTH));
      
    }


  // Change lfo depth
  if (TK.getAll(LFO1_DEPTH)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(LFO1_DEPTH,P[cty].getPatternElement(i).get(LFO1_DEPTH)+TK.getAll(LFO1_DEPTH));
      TK.setAll(LFO1_DEPTH,0);  
      if (debug)
	printf("[lfo_depth_all:%d]\n",P[cty].getPatternElement(cursor).get(LFO1_DEPTH));
      
    }


  // Change lfo speed
  if (TK.get(LFO1_FREQ)!=0)
    {
      P[cty].getPatternElement(cursor).set(LFO1_FREQ,P[cty].getPatternElement(cursor).get(LFO1_FREQ)+TK.get(LFO1_FREQ));
      TK.set(LFO1_FREQ,0);
      if (debug)
	printf("[lfo_speed:%d]\n",P[cty].getPatternElement(cursor).get(LFO1_FREQ));
      
    }

  // Change lfo speed
  if (TK.getAll(LFO1_FREQ)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(LFO1_FREQ,P[cty].getPatternElement(i).get(LFO1_FREQ)+TK.getAll(LFO1_FREQ));
      TK.setAll(LFO1_FREQ,0);
      if (debug)
	printf("[lfo_speed:%d]\n",P[cty].getPatternElement(cursor).get(LFO1_FREQ));
      
    }


  // Change pb depth
  if (TK.get(PITCHBEND_DEPTH)!=0)
    {      
      P[cty].getPatternElement(cursor).set(PITCHBEND_DEPTH,P[cty].getPatternElement(cursor).get(PITCHBEND_DEPTH)+TK.get(PITCHBEND_DEPTH));
      TK.set(PITCHBEND_DEPTH,0);
      if (debug)
	printf("[pb_depth:%d]\n",P[cty].getPatternElement(cursor).get(PITCHBEND_DEPTH));
      
    }


  // Change pb depth
  if (TK.getAll(PITCHBEND_DEPTH)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(PITCHBEND_DEPTH,P[cty].getPatternElement(i).get(PITCHBEND_DEPTH)+TK.getAll(PITCHBEND_DEPTH));
      TK.setAll(PITCHBEND_DEPTH,0);
      if (debug)
	printf("[pb_depth_all:%d]\n",P[cty].getPatternElement(cursor).get(PITCHBEND_DEPTH));
      
    }


  // Change pb speed
  if (TK.get(PITCHBEND_SPEED)!=0)
    {
      P[cty].getPatternElement(cursor).set(PITCHBEND_SPEED,
					   P[cty].getPatternElement(cursor).get(PITCHBEND_SPEED)+TK.get(PITCHBEND_SPEED));
      TK.set(PITCHBEND_SPEED,0);
      if (debug)
	printf("[pb_speed:%d]\n",P[cty].getPatternElement(cursor).get(PITCHBEND_SPEED));
      
    }

  // Change pb speed
  if (TK.getAll(PITCHBEND_SPEED)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(PITCHBEND_SPEED,
					P[cty].getPatternElement(i).get(PITCHBEND_SPEED)+TK.getAll(PITCHBEND_SPEED));
      TK.setAll(PITCHBEND_SPEED,0);
      if (debug)
	printf("[pb_speed:%d]\n",P[cty].getPatternElement(cursor).get(PITCHBEND_SPEED));
      
    }


  // Change lfo type
  if (TK.get(LFO_TYPE)!=0)
    {
      P[cty].getPatternElement(cursor).set(LFO_TYPE,
					   P[cty].getPatternElement(cursor).get(LFO_TYPE)+TK.get(LFO_TYPE));
      TK.set(LFO_TYPE,0);
      if (debug)
	printf("[lfo_type:%d]\n",P[cty].getPatternElement(cursor).get(LFO_TYPE));
      
    }

  // Change pb speed
  if (TK.getAll(LFO_TYPE)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(LFO_TYPE,P[cty].getPatternElement(i).get(LFO_TYPE)+TK.getAll(LFO_TYPE));
      TK.setAll(LFO_TYPE,0);
      if (debug)
	printf("[lfo_type:%d]\n",P[cty].getPatternElement(cursor).get(LFO_TYPE));
      
    }

  
  
  // Change filter cutoff
  if (TK.get(FILTER1_CUTOFF)!=0)
    {
      P[cty].getPatternElement(cursor).set(FILTER1_CUTOFF,
					   P[cty].getPatternElement(cursor).get(FILTER1_CUTOFF)+TK.get(FILTER1_CUTOFF));
      TK.set(FILTER1_CUTOFF,0);
      if (debug) printf("[cutoff:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_CUTOFF));	  
    }
  
  // Change filter cutoff
  if (TK.getAll(FILTER1_CUTOFF)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(FILTER1_CUTOFF,
					P[cty].getPatternElement(i).get(FILTER1_CUTOFF)+TK.getAll(FILTER1_CUTOFF));
      TK.setAll(FILTER1_CUTOFF,0);
      if (debug) printf("[cutoff_all:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_CUTOFF));
    }
  
  // Change filter resonance
  if (TK.get(FILTER1_RESONANCE)!=0)
    {
      P[cty].getPatternElement(cursor).set(FILTER1_RESONANCE,
					   P[cty].getPatternElement(cursor).get(FILTER1_RESONANCE)+TK.get(FILTER1_RESONANCE));
      TK.set(FILTER1_RESONANCE,0);
      if (debug) printf("[resonance:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_RESONANCE));	  
    }
  
  // Change filter resonance
  if (TK.getAll(FILTER1_RESONANCE)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(FILTER1_RESONANCE,
					P[cty].getPatternElement(i).get(FILTER1_RESONANCE)+TK.getAll(FILTER1_RESONANCE));
      TK.setAll(FILTER1_RESONANCE,0);
      if (debug) printf("[resonance_all:%d]\n",P[cty].getPatternElement(cursor).get(FILTER1_RESONANCE));	  
    }
  
    // Change step divider
  if (TK.get(BPM_DIVIDER)!=0)
    {	  
      if (TK.get(BPM_DIVIDER)>0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()*2);
      if (TK.get(BPM_DIVIDER)<0)
	SEQ.getPatternSequencer(cty).setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider()/2);
      TK.set(BPM_DIVIDER,0);
      P[cty].setBPMDivider(SEQ.getPatternSequencer(cty).getBPMDivider());
    }
  
  // Change oscillator one
  if (TK.get(OSC1_TYPE)!=0)
    {
      update_SAMM(cty,cursor);
      P[cty].getPatternElement(cursor).set(OSC1_TYPE,
					   SAM->checkI(OSC1_TYPE,P[cty].getPatternElement(cursor).get(OSC1_TYPE)+TK.get(OSC1_TYPE)));
      TK.set(OSC1_TYPE,0);
      if (debug)
	printf("[osconetype:%d]\n",P[cty].getPatternElement(cursor).get(OSC1_TYPE));	  
    }
  
  // Change oscillator one
  if (TK.getAll(OSC1_TYPE)!=0)
    {
      for (i=0;i<16;i++)
	{
	  update_SAMM(cty,i);
	  P[cty].getPatternElement(i).set(OSC1_TYPE,
					  SAM->checkI(OSC1_TYPE,P[cty].getPatternElement(i).get(OSC1_TYPE)+TK.getAll(OSC1_TYPE)));
	}
      TK.setAll(OSC1_TYPE,0);
      if (debug)
	printf("[osconetype_all:%d]\n",P[cty].getPatternElement(cursor).get(OSC1_TYPE));	  
    }
  
  // Change oscillator two
  if (TK.get(OSC2_TYPE)!=0)
    {
      update_SAMM(cty,cursor);
      P[cty].getPatternElement(cursor).set(OSC2_TYPE,
					   SAM->checkI(OSC2_TYPE,P[cty].getPatternElement(cursor).get(OSC2_TYPE)+TK.get(OSC2_TYPE)));
      TK.set(OSC2_TYPE,0);
      if (debug)
	printf("[osctwotype:%d]\n",P[cty].getPatternElement(cursor).get(OSC2_TYPE));	  
    }
  
  // Change oscillator two
  if (TK.getAll(OSC2_TYPE)!=0)
    {
      for (i=0;i<16;i++)
	{
	  update_SAMM(cty,i);
	  P[cty].getPatternElement(i).set(OSC2_TYPE,
					  SAM->checkI(OSC2_TYPE,P[cty].getPatternElement(i).get(OSC2_TYPE)+TK.getAll(OSC2_TYPE)));
	}
      TK.setAll(OSC2_TYPE,0);
      if (debug)
	printf("[osctwotype:%d]\n",P[cty].getPatternElement(cursor).get(OSC2_TYPE));	  
    }
  


  // Change oscillator one
  if (TK.get(MACHINE_TYPE)!=0)
    {
      P[cty].getPatternElement(cursor).set(MACHINE_TYPE,P[cty].getPatternElement(cursor).get(MACHINE_TYPE)+TK.get(MACHINE_TYPE));
      TK.set(MACHINE_TYPE,0);
      if (debug)
	printf("[machine_type:%d]\n",P[cty].getPatternElement(cursor).get(MACHINE_TYPE));	  
    }
  
  // Change oscillator one
  if (TK.getAll(MACHINE_TYPE)!=0)
    {
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(MACHINE_TYPE,P[cty].getPatternElement(i).get(MACHINE_TYPE)+TK.getAll(MACHINE_TYPE));
      TK.setAll(MACHINE_TYPE,0);
      if (debug)
	printf("[machine_type_all:%d]\n",P[cty].getPatternElement(cursor).get(MACHINE_TYPE));	  
    }
  
  
  // Change Note
  if (TK.get(NOTE)!=0)
    { 
      P[cty].getPatternElement(cursor).set(NOTE,
					   P[cty].getPatternElement(cursor).get(NOTE)+TK.get(NOTE));
      TK.set(NOTE,0);
      printf("[note:%d]\n",P[cty].getPatternElement(cursor).get(NOTE));	  
    }
  
  // Change Note
  if (TK.getAll(NOTE)!=0)
    { 
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(NOTE,
					P[cty].getPatternElement(i).get(NOTE)+TK.getAll(NOTE));
      TK.setAll(NOTE,0);
      printf("[note_all:%d]\n",P[cty].getPatternElement(cursor).get(NOTE));	  
    }


  // Change Note
  if (TK.get(FM_TYPE)!=0)
    { 
      P[cty].getPatternElement(cursor).set(FM_TYPE,P[cty].getPatternElement(cursor).get(FM_TYPE)+TK.get(FM_TYPE));
      TK.set(FM_TYPE,0);
      printf("[fmtype:%d]\n",P[cty].getPatternElement(cursor).get(FM_TYPE));	  
    }
  
  // Change Note
  if (TK.getAll(FM_TYPE)!=0)
    { 
      for (i=0;i<16;i++)
	P[cty].getPatternElement(i).set(FM_TYPE,P[cty].getPatternElement(i).get(FM_TYPE)+TK.getAll(FM_TYPE));
      TK.setAll(FM_TYPE,0);
      printf("[fmtype_all:%d]\n",P[cty].getPatternElement(cursor).get(FM_TYPE));
    }



  // Change Note
  if (TK.getAll(PATTERN_LENGTH)!=0)
    { 
      SEQ.getPatternSequencer(cty).setPatternLenght(SEQ.getPatternSequencer(cty).getPatternLenght()+TK.getAll(PATTERN_LENGTH));
      TK.setAll(PATTERN_LENGTH,0);
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


  if (TK.get(BPM)!=0)
    {
      //save the bpm in the 
      //change the number of time AudioEngine need to be trigged
      //to effectively change pattern step
      
      bpm_current=P[cty].getBPM();
      for(t=0;t<TRACK_MAX;t++)	    
	P[t].setBPM(bpm_current+TK.get(BPM));
      
      bpm_current=bpm_current+TK.get(BPM);
      TK.set(BPM,0);
      refresh_bpm();
      
    }
  
  if (TK.get(SWING)!=0)
    {
      //save the swing in the Pattern
      //change the number of time AudioEngine need to be trigged
      //to effectively change pattern step
      
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
	  if (P[cty].getPatternElement(cursor).get(NOTE)==0)
	    {
	      //P[cty].getPatternElement(cursor).setAttack_amp(0);
	      //P[cty].getPatternElement(cursor).setRelease_amp(64);
	      //P[cty].getPatternElement(cursor).setNote(37);
	      P[cty].getPatternElement(cursor).set(NOTE,25);
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
	  M[t]->setI(OSC1_NOTE,P[t].getPatternElement(step).get(NOTE));

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

	  M[t]->setI(LFO1_DEPTH,P[t].getPatternElement(step).get(LFO1_DEPTH));
	  M[t]->setF(LFO1_FREQ,P[t].getPatternElement(step).get(LFO1_FREQ));

	  M[t]->setI(PITCHBEND_DEPTH,P[t].getPatternElement(step).get(PITCHBEND_DEPTH));
	  M[t]->setI(PITCHBEND_SPEED,P[t].getPatternElement(step).get(PITCHBEND_SPEED));
	  M[t]->setI(LFO_TYPE,       P[t].getPatternElement(step).get(LFO_TYPE));

	  //M[t]->set(OSC12_MIX,phase);

	  M[t]->setI(OSC1_TYPE,P[t].getPatternElement(step).get(OSC1_TYPE));
	  M[t]->setI(OSC2_TYPE,P[t].getPatternElement(step).get(OSC2_TYPE));

	  M[t]->setI(OSC1_AMP,P[t].getPatternElement(step).get(OSC1_AMP));
	  M[t]->setI(OSC2_AMP,P[t].getPatternElement(step).get(OSC2_AMP));


	  M[t]->setI(FILTER1_TYPE,P[t].getPatternElement(step).get(FILTER1_TYPE));
	  M[t]->setI(FILTER1_ALGO,P[t].getPatternElement(step).get(FILTER1_ALGO));;


	  M[t]->setI(FILTER1_CUTOFF,P[t].getPatternElement(step).get(FILTER1_CUTOFF));
	  M[t]->setI(FILTER1_RESONANCE,P[t].getPatternElement(step).get(FILTER1_RESONANCE));




	  M[t]->setI(NOTE_ON,1);

	}
      /*
      else
	{
	  if (noteOffTrigger[t]<0)
	    {
	      printf("$$$NOTEOFF\n");
	      //M[t]->getADSRAmp().setNoteOff();
	      //M[t]->getADSRFltr().setNoteOff();
	      //M[t]->reset();

	      //M[t]->setI(NOTE_ON,0);
	    }
	  else
	    {
	      noteOffTrigger[t]=noteOffTrigger[t]-1;
	      printf("noteOffTrigger[%d]=%d\n",noteOffTrigger[t],t);
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

  dirty_graphic=1;

  printf("Now in PatternPlayer::seq()\n");

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
      // if user want to quit via handle_key
      if (quit ||
	  running<=0)
	{
	  printf("user want to quit\n");
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
	  last_nbcb_ch_step=nbcb;
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
  int cpu_speed;
#ifdef PSP
  running = isRunning();
  setupExitCallback();
  cpu_speed=scePowerGetCpuClockFrequencyInt();
  printf("CURRENT PSP CPU SPEED:%d\n",cpu_speed);
  //cpu_speed=333;
  //scePowerSetCpuClockFrequency(cpu_speed);
  //scePowerSetClockFrequency(333, 333, 166);
  //scePowerSetClockFrequency(233, 233, 133);
  //scePowerSetClockFrequency(300, 300, 150);
  scePowerSetClockFrequency(266, 266, 133);
  cpu_speed=scePowerGetCpuClockFrequencyInt();
  printf("NEW PSP CPU SPEED:%d\n",cpu_speed);
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
  //exit(0);

  //AE.openAudioSdl();
  
  seq();

  printf("[closeAudio output]\n");
  //AE.stopAudioSdl();
  AE.stopAudio();
  AE.closeAudio();
  
  //AE.closeAudioSdl();

  printf("[closeVideo output]\n");
  SG.closeVideo();
  //sleep(10);
  //PE.print();
  printf("Exiting PatternPlayer\n");

#ifdef PSP
  sceKernelExitGame();	
#endif
}

