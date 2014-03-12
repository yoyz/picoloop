using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include "Note.h"
#include "PatternReader.h"
#include "PatternPlayer.h"
#include "AudioEngine.h"
#include "Wave.h"
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

vector <Pattern>            P(TRACK_MAX);  
vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);

Sequencer      SEQ;         // used to  store/get information about sequencer 
AudioEngine    AE;          // used to  init alsa/rtaudio
PatternReader  PR;          // used to  read data.pic file
PatternElement PE;          // used for copy paste PatternElement
InputManager   IE;          // used to  fetch key
SDL_GUI        SG;          // used to  open a gui and display stuff
Wave           cowbell;     // used ?
Instrument     inst;        // used ?


int save=false;
int load=false;

int saveall=false;
int loadall=false;


int tempo=60;
int nbcb=0;             // current nb audio callback 
int last_nbcb=0;
int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*tempo); // Weird ?
int last_nbcb_ch_step=0;// nb audio callback from last step
int debug=1;
int t=0;                // index of track



int repeat=0;
int quit=0;             // do we need to quit ?
int cursor=0;           // cursor position in a sequencer track
int note=0;
int attack=0;
int release=0;
int vcomix=0;
int osconetype=0;
int osctwotype=0;

int loadsave_cursor_x=0; // index in the load/save menu
int loadsave_cursor_y=0; // index in the load/save menu

int start_key=0;        // start key pressed ?
int step=0;             // current step in the sequencer
int menu=0;             // menu mode
int menu_note=0;
int menu_cursor=0;      // index int the menu
//int ct=0;               // current_track
int ct_x=0;             
int ct_y=0;
int invert_trig=0;

int dirty_graphic=1;

//menu_cursor
enum {
  AD,
  NOTE,
  LS,
  VCO,
  OSC,
  LFO,
  FLTR,
  FX
};
  
//menu
enum {
  MENU_OFF,
  MENU_ON_PAGE1,
  MENU_ON_PAGE2
};

#define BUTTON_B      SDLK_LALT
#define BUTTON_A      SDLK_LCTRL
#define BUTTON_X      SDLK_SPACE
#define BUTTON_Y      SDLK_LSHIFT

#define BUTTON_UP     SDLK_UP
#define BUTTON_DOWN   SDLK_DOWN
#define BUTTON_LEFT   SDLK_LEFT
#define BUTTON_RIGHT  SDLK_RIGHT

#define BUTTON_SELECT SDLK_ESCAPE
#define BUTTON_START  SDLK_RETURN


//char * tmp_str;

void display_board()
{
  int i;
  char str_up[8];
  char str_down[24];
  int  cty=SEQ.getCurrentTrackY();
  dirty_graphic=0;

  SG.clearScreen();
  //  sprintf(str,"Track %d ",ct);
  sprintf(str_up,"Track %d ",cty);
  SG.guiTTFText(200,20,str_up);
  //  sprintf(str_down,"[A/D] Note L/S",cty);
  
  //  printf("           AD:%d FX:%d\n",AD,FX);
  //  exit(0);

  if (menu==MENU_ON_PAGE1 && menu_cursor==AD)    sprintf(str_down,"[A/R] Note  L/S   VCO ",cty);  
  if (menu==MENU_ON_PAGE1 && menu_cursor==NOTE)  sprintf(str_down," A/R [Note] L/S   VCO ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==LS)    sprintf(str_down," A/R  Note [L/S]  VCO ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==VCO)   sprintf(str_down," A/R  Note  L/S  [VCO]",cty);

  if (menu==MENU_ON_PAGE2 && menu_cursor==OSC)   sprintf(str_down,"[OSC] LFO   FLTR  FX ",cty);  
  if (menu==MENU_ON_PAGE2 && menu_cursor==LFO)   sprintf(str_down," OSC [LFO]  FLTR  FX ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==FLTR)  sprintf(str_down," OSC  LFO  [FLTR] FX ",cty);
  if (menu==MENU_ON_PAGE2 && menu_cursor==FX)    sprintf(str_down," OSC  LFO   FLTR [FX]",cty);

  if (menu==0)                       sprintf(str_down,"                     ",cty);

  if (menu_cursor==AD)               sprintf(str_up,"A/R ");
  if (menu_cursor==NOTE)             sprintf(str_up,"Note");
  if (menu_cursor==LS)               sprintf(str_up,"L/S ");
  if (menu_cursor==VCO)              sprintf(str_up,"VCO ");

  if (menu_cursor==OSC)              sprintf(str_up,"OSC ");
  if (menu_cursor==LFO)              sprintf(str_up,"LFO");
  if (menu_cursor==FLTR)             sprintf(str_up,"FLTR ");
  if (menu_cursor==FX)               sprintf(str_up,"FX ");


  SG.guiTTFText(200,40,str_up);
  SG.guiTTFText(10,200,str_down);

  // Draw all box default color   
  for (i=0;i<16;i++)
    { SG.drawBoxNumber(i,BOX_COLOR); }
     

  // Attack/Release
  if (menu_cursor==AD)
    {
      // Cursor & step postion      
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      //SG.smallBoxNumber(cursor,P[cty].getPatternElement(cursor).getRelease(),0,SMALLBOX_COLOR);
      //SG.smallBoxNumber(cursor,0,P[cty].getPatternElement(cursor).getAttack(),SMALLBOX_COLOR); 
      
      SG.drawBoxNumber(step,STEP_COLOR);  
      //SG.smallBoxNumber(step,P[cty].getPatternElement(step).getRelease(),0,SMALLBOX_COLOR);
      //SG.smallBoxNumber(step,0,P[cty].getPatternElement(step).getAttack(),SMALLBOX_COLOR); 

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
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getRelease(),0,SMALLBOX_COLOR);
	      SG.smallBoxNumber(i,0,P[cty].getPatternElement(i).getAttack(),SMALLBOX_COLOR);      
	    }
	}
    }



  // Note
  if (menu_cursor==NOTE)
    {
      if (menu_note==0)      
	for (i=0;i<16;i++)
	  {
	    // Draw trig note color
	    if (P[cty].getPatternElement(i).getTrig())
	      SG.drawBoxNumber(i,NOTE_COLOR);
	    
	    SG.smallBoxNumber(i,
			      (P[cty].getPatternElement(i).getNote()%12)*10,
			      (P[cty].getPatternElement(i).getNote()/12)*10,
			      SMALLBOX_COLOR);
	  }
      // Note C3 
      if (menu_note==1)
	{	  
	  for (i=0;i<16;i++)
	    {
	      // Draw trig note color
	      if (P[cty].getPatternElement(i).getTrig())
		SG.drawBoxNumber(i,NOTE_COLOR);
	      
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  

	      if (P[cty].getPatternElement(i).getTrig())
		SG.drawTTFTextNumberFirstLine(i,P[cty].getPatternElement(i).getNoteCharStar());
	    }
	}
      // Note Cursor
      if (menu_note==0)
	{

	  SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  SG.drawBoxNumber(step,STEP_COLOR);  
    
	  SG.smallBoxNumber(cursor,
			    (P[cty].getPatternElement(cursor).getNote()%12)*10,
			    (P[cty].getPatternElement(cursor).getNote()/12)*10,
			    SMALLBOX_COLOR);
	  SG.smallBoxNumber(step,  
			    (P[cty].getPatternElement(step).getNote()%12)*10,
			    (P[cty].getPatternElement(step).getNote()/12)*10,
			    SMALLBOX_COLOR);
	}
    }

  if (menu==MENU_OFF && 
      menu_cursor==LS)
    {
      printf("HIT\n");
      int x,y;
      const char * txt="0";
      const char * tmp_txt;
      tmp_txt="0";

      SG.clearScreen();
      for (x=0;x<16;x++)
	for (y=0;y<4;y++)
	  {
	    if (PR.PatternDataExist(x,y))
	      SG.middleBoxNumber(x,y,NOTE_COLOR);
	    else
	      SG.middleBoxNumber(x,y,STEP_COLOR);
	  }
      
      SG.middleBoxNumber(loadsave_cursor_x,loadsave_cursor_y,TRIG_COLOR);
      
      for (x=0;x<16;x++)
	for (y=0;y<4;y++)
	  SG.drawTTFTextLoadSaveBoxNumer(x,y,tmp_txt);
    }


  // VCO
  if (menu_cursor==VCO)
    {
      for (i=0;i<16;i++)
	{
	  // Draw trigged box trig color   
	  //	  if (P[cty].getPatternElement(i).getTrig())
	  //	    SG.drawBoxNumber(i,TRIG_COLOR);
	  // AdsR
	  if (P[cty].getPatternElement(i).getTrig())
	    {	      
	      //if (P[cty].getPatternElement(i).getTrig())
	      SG.drawBoxNumber(i,NOTE_COLOR);
	      SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),0,SMALLBOX_COLOR);

	      if (i==step)   SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),0,STEP_COLOR);
	      if (i==cursor) SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),0,CURSOR_COLOR);
	    }
	  //	  SG.smallBoxNumber(i,0,P[cty].getPatternElement(i).getAttack(),SMALLBOX_COLOR);      
	  else
	    {

	      if (i==step)    SG.drawBoxNumber(step,  STEP_COLOR);  
	      if (i==cursor) SG.drawBoxNumber(cursor,CURSOR_COLOR);

	      //	      if (i==cursor) SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),0,CURSOR_COLOR);
	      //	      if (i==step)   SG.smallBoxNumber(i,P[cty].getPatternElement(i).getVCOMix(),0,STEP_COLOR);

	    }
	}
      // Cursor & step postion
      //SG.drawBoxNumber(cursor,CURSOR_COLOR);
      //      SG.smallBoxNumber(cursor,P[cty].getPatternElement(cursor).getVCOMix(),0,SMALLBOX_COLOR);
      //      SG.smallBoxNumber(cursor,0,P[cty].getPatternElement(cursor).getAttack(),SMALLBOX_COLOR); 
      
      //      SG.drawBoxNumber(step,STEP_COLOR);  
      //SG.smallBoxNumber(step,P[cty].getPatternElement(step).getVCOMix(),0,SMALLBOX_COLOR);
      //SG.smallBoxNumber(step,0,P[cty].getPatternElement(step).getAttack(),SMALLBOX_COLOR); 
    }

  if (menu_cursor==OSC)
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


  SG.refresh();
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

  //Enable Menu navigation
  if (lastKey   ==  BUTTON_SELECT && 
      lastEvent ==  SDL_KEYUP)
    {
      switch (menu)
	{
	case MENU_OFF:      
	  menu=MENU_ON_PAGE1;                 
	  break;

	case MENU_ON_PAGE1: 
	  menu=MENU_ON_PAGE2; 
	  menu_cursor+=4; 
	  break;

	case MENU_ON_PAGE2: 
	  menu=MENU_ON_PAGE1; 
	  menu_cursor-=4; 
	  break;

	  //	case MENU_ON_PAGE1: menu=MENU_OFF;      break;
	  //case MENU_ON_PAGE1: menu=MENU_ON_PAGE2; menu_cursor=+4; break;
	  //case MENU_ON_PAGE2: menu=MENU_ON_PAGE1; menu_cursor=-4; break;
	}
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      printf("[global menu : %d]\n",menu);
    }

  //leave menu mode
  if (lastKey      ==  BUTTON_B       && 
      lastEvent    ==  SDL_KEYUP      &&
      (menu        ==  MENU_ON_PAGE1  ||
       menu        ==  MENU_ON_PAGE2))
    menu=MENU_OFF;
  
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  SDL_KEYUP     && 
      menu_cursor ==  NOTE)
    {
      if      (menu_note==0)        { menu_note=1;  }
      else if (menu_note==1)        { menu_note=0;  }   
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      printf("[sub menu note : %d]\n",menu_note);
    }
  
  //Menu Mode 
  //Move MENU_CURSOR
  if (menu==MENU_ON_PAGE1 ||
      menu==MENU_ON_PAGE2)
    {
      if(keyState[BUTTON_LEFT]) 
	{
	  if (keyRepeat[BUTTON_LEFT]    == 1 || 
	      keyRepeat[BUTTON_LEFT]%64 == 0)
	    menu_cursor--;
	  if (menu_cursor<AD  && menu==MENU_ON_PAGE1) menu_cursor=VCO;
	  if (menu_cursor<OSC && menu==MENU_ON_PAGE2) menu_cursor=FX;
	  dirty_graphic=1;
	  printf("\t\t[menu_cursor:%d]\n",menu_cursor);
	  printf("key left\n");            
	}      
      
      if(keyState[BUTTON_RIGHT])
	{
	  if (keyRepeat[BUTTON_RIGHT]    == 1 || 
	      keyRepeat[BUTTON_RIGHT]%64 == 0)
	    menu_cursor++;
	  if (menu_cursor>VCO && menu==MENU_ON_PAGE1) menu_cursor=AD;
	  if (menu_cursor>FX  && menu==MENU_ON_PAGE2) menu_cursor=OSC;
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
  
  
  
  
  
  
  //MOVE the cursor : LEFT UP DOWN RIGHT   
  if ((menu==MENU_OFF && menu_cursor==AD    ||
       menu==MENU_OFF && menu_cursor==NOTE  ||
       menu==MENU_OFF && menu_cursor==VCO   ||
       menu==MENU_OFF && menu_cursor==OSC
       ) &&
      !keyState[BUTTON_B]                   &&
      !keyState[BUTTON_A]
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
  
  
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == AD)
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
	  { release=-1;   dirty_graphic=1; }
      
      if (keyState[BUTTON_RIGHT] && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]>4) 
	  { release=1; 	  dirty_graphic=1; }
      
      if (keyState[BUTTON_UP]    && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_UP]==1 ||    keyRepeat[BUTTON_UP]>4) 
	  { attack=1;  	  dirty_graphic=1; }
      
      if (keyState[BUTTON_DOWN]  && keyState[BUTTON_B]) 
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]>4) 
	  { attack=-1; 	  dirty_graphic=1; }
    }  
  
  // change note
  // copy/paste
  if (menu        == MENU_OFF && 
      menu_cursor == NOTE)
    {
      // copy/paste/insert/delete trig 
      if (lastKey   == BUTTON_A && 
	  lastEvent ==  SDL_KEYUP)
	{
	  invert_trig=1;
	  printf("key lalt\n");      
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	}
      
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
  

  // in the load/save view 
  // move loasavecursor position 
  // Save/load Pattern
  if (menu        == MENU_OFF && 
      menu_cursor == LS       && 
      keyState[BUTTON_B])
    {
      if (keyState[BUTTON_DOWN])
	save=true;
      if (keyState[BUTTON_UP])	
	load=true;
    }

  // in the load/save view 
  // move loasavecursor position 
  // Save/load bund of Pattern

  if (menu        == MENU_OFF && 
      menu_cursor == LS       && 
      keyState[BUTTON_A] )
    {
      if (keyState[BUTTON_DOWN])
	saveall=true;
      if (keyState[BUTTON_UP])	
	loadall=true;
    }


  // in the load/save view 
  // move load/save cursor position 
  if (menu        == MENU_OFF && 
      menu_cursor == LS &&
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
	  
      if (loadsave_cursor_x>15)          { loadsave_cursor_x=0;           }
      if (loadsave_cursor_x<0)           { loadsave_cursor_x=15;          }
      if (loadsave_cursor_y>TRACK_MAX-1) { loadsave_cursor_y=0;           }
      if (loadsave_cursor_y<0)           { loadsave_cursor_y=TRACK_MAX-1; }  
      
      SEQ.setCurrentTrackY(loadsave_cursor_y);

    }


  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == VCO      &&
      keyState[BUTTON_B]
      )
    {
      if (keyState[BUTTON_LEFT]) 
	if (keyRepeat[BUTTON_LEFT]==1 || keyRepeat[BUTTON_LEFT]%4==0) 
	  { vcomix=-1; 	  dirty_graphic=1;}
      
      if (keyState[BUTTON_RIGHT]) 
	if (keyRepeat[BUTTON_RIGHT]==1 || keyRepeat[BUTTON_RIGHT]%4==0) 
	  { vcomix=1;  	  dirty_graphic=1;}
      
      if (keyState[BUTTON_UP]) 
	if (keyRepeat[BUTTON_UP]==1 || keyRepeat[BUTTON_UP]%64==0) 
	  { attack=1;   	  dirty_graphic=1;}
      
      if (keyState[BUTTON_DOWN])
	if (keyRepeat[BUTTON_DOWN]==1 || keyRepeat[BUTTON_DOWN]%64==0) 
	  { attack=-1;  	  dirty_graphic=1;}
    }
  

  // change oscilltor one and two type
  if (menu        == MENU_OFF && 
      menu_cursor == OSC )
    {
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


  int delay=1;
  //printf("sleeping %dms\n",delay);
  SDL_Delay(delay);  
}



// 
int seq_update()
{
  int  cty=SEQ.getCurrentTrackY();
  int  ctx=SEQ.getCurrentTrackX();
  int  t;

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
	PR.readPatternData(loadsave_cursor_x,loadsave_cursor_y,P[cty]);
      else
	P[cty].init();
      load=false;
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
  if (loadall)
    {
      printf("<==[LOAD_ALL]==>\n");
      for (t=0;t<TRACK_MAX;t++)
	if (PR.PatternDataExist(loadsave_cursor_x,t)==true)
	  PR.readPatternData(loadsave_cursor_x,t,P[t]);
	else
	  P[t].init();
      loadall=false;
    }

    
  if (step==16) 
    step=0; 

  if (debug)
    printf("STEP:%d\n",step);	  
  
  
  // Handle only modification of on next step value 
  // for each Machine 
  // of each track 
  for (t=0;t<TRACK_MAX;t++)
    {
      if (P[t].getPatternElement(step).getTrig()==true)
	{
	  float f=P[t].getPatternElement(step).getNoteFreq();
	  int   i=f;
	  	 
	  M[t]->getADSR().reset();;	  
	  M[t]->getVCO().reset();
	  M[t]->getVCO().getOscillatorOne();

	  M[t]->getVCO().setSynthFreq(i);
	  M[t]->getADSR().setRelease(P[t].getPatternElement(step).getRelease());		  
	  M[t]->getADSR().setAttack(P[t].getPatternElement(step).getAttack());		  
	  M[t]->getVCO().setVCOMix(P[t].getPatternElement(step).getVCOMix());		  
	  M[t]->getVCO().setOscillator(0,P[t].getPatternElement(step).getOscillatorOneType());
	  M[t]->getVCO().setOscillator(1,P[t].getPatternElement(step).getOscillatorTwoType());
	}
      else
	{
	  //m.setSynthFreq(800);
	  //printf("m.setSynthFreq(0);\n");
	  //m0.setSynthFreq(0);
	}
    }		  
}


int seq()
{
  AudioMixer & am=AE.getAudioMixer();
  int  cty=SEQ.getCurrentTrackY();
  int  ctx=SEQ.getCurrentTrackX();

  // Initialize 
  for (t=0;t<TRACK_MAX;t++)
    {

      MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer();
      //MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer().getInput().init();
      
      M[t]=MM[t]->getInput();
      M[t]->init();
      M[t]->getADSR().init();
      M[t]->getVCO().init();
      //M[t]->getVCO().init();
      //M[t]->getVCO().tick();
      M[t]->getVCO().setSynthFreq(0);      
      //M[t].getVCO().setSineOsc();
    }

  printf("openAudio start streaming\n");
  //  sleep(2);
  AE.startAudio();

  seq_update();  
  while (true)
    {
      cty=SEQ.getCurrentTrackY();
      ctx=SEQ.getCurrentTrackX();

      nbcb=AE.getNbCallback();

      if (nbcb>last_nbcb)
	{
	  last_nbcb=nbcb;
	}
      handle_key();


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
	    }
	  invert_trig=0;
	}
	  
      // Change Attack
      if (attack!=0)
	{
	  //m0.getADSR().setAttack(m0.getADSR().getAttack()+attack);
	  P[cty].getPatternElement(cursor).setAttack(P[cty].getPatternElement(cursor).getAttack()+attack);
	  attack=0;
	  if (debug)
	    printf("[attack:%d]\n",P[cty].getPatternElement(cursor).getAttack());
	  //printf("[attack:%d]\n",P[cty].getPatternElement(cursor).getAttack()+attack);
	//printf("[attack:%d]\n",M[cty]->getADSR().getAttack());
	}
      
      // Change Release
      if (release!=0)
	{
	  //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
	  P[cty].getPatternElement(cursor).setRelease(P[cty].getPatternElement(cursor).getRelease()+release);
	  release=0;
	  if (debug)
	    printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease());
	  //	printf("[release:%d]\n",P[cty].getPatternElement(cursor).getRelease()+release);
	  
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
        
      if (osconetype!=0)
	{
	  P[cty].getPatternElement(cursor).setOscillatorOneType(P[cty].getPatternElement(cursor).getOscillatorOneType()+osconetype);
	  osconetype=0;
	  if (debug)
	    printf("[osconetype:%d]\n",P[cty].getPatternElement(cursor).getOscillatorOneType());	  
	}

      if (osctwotype!=0)
	{
	  P[cty].getPatternElement(cursor).setOscillatorTwoType(P[cty].getPatternElement(cursor).getOscillatorTwoType()+osctwotype);
	  osctwotype=0;
	  if (debug)
	    printf("[osctwotype:%d]\n",P[cty].getPatternElement(cursor).getOscillatorTwoType());	  
	}



      // Change Note
      if (note!=0)
	{ 
	  P[cty].getPatternElement(cursor).setNote(P[cty].getPatternElement(cursor).getNote()+note);
	  note=0;
	}
            
      // if user want to quit via handle_key
      if (quit)
	return(0);
      
      //display graphic if something has change : handle_key 
      if (dirty_graphic)
	display_board();

      // change step in the pattern
      if (nbcb-last_nbcb_ch_step>nb_cb_ch_step)
	{
	  dirty_graphic=1;
	  display_board();
	  //printf("[cursor:%d]\n",cursor);
	  
	  //printf("loop\n");    
	  last_nbcb_ch_step=nbcb;
	  step++;  
	  seq_update();
	}
      if (AE.bufferIsGenerated()==0)
	AE.processBuffer();
    }
}

void load_pattern()
{
  int i;

  string fileName="data.pic";
  //PR.setFileName("data.pic");
  PR.init();
  PR.setFileName(fileName);
  for (i=0;i<TRACK_MAX;i++)
    PR.readPatternData(0,i,P[i]);
    //PR.readPatternData(1,i+1,P[i]);

}



int main()
{
  //  exit(0);
  //string wave="808-cowbell.wav";
  //  char * str="808-cowbell.wav";
  //tmp_str=(char*)malloc(sizeof(char)*4);
  //cowbell.loadWave(str);
  load_pattern();
  printf("[openVideo output]\n");
  SG.initVideo();
  //handle_key();
  //  SDL_EnableKeyRepeat(500,500);
  SG.openBMPFont();
  if (SG.openTTFFont()==false) { printf("ttf font error\n"); exit(1); }
  display_board();

  printf("[openAudio output]\n");
  AE.openAudio();

  seq();

  printf("[closeVideo output]\n");
  SG.closeVideo();
  printf("[closeAudio output]\n");
  AE.closeAudio();
  //sleep(10);
  //PE.print();
}
