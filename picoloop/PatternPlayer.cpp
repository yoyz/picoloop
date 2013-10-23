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

#define KEY_REPEAT_INTERVAL 400

#define BOX_COLOR        0xAECD15
#define TRIG_COLOR       0x0E4C15
#define NOTE_COLOR       0x46DC65
#define CURSOR_COLOR     0x1515CD
#define STEP_COLOR       0x242C45

#define SMALLBOX_COLOR   0x442233

vector <Pattern>      P(TRACK_MAX);  
vector <Machine   *>  M(TRACK_MAX);
vector <MonoMixer *> MM(TRACK_MAX);


AudioEngine AE;
PatternReader PR;
PatternElement PE;
InputManager IE;
SDL_GUI SG;
Wave cowbell;
Instrument inst;


int save=false;
int load=false;

int tempo=60;
int nbcb=0;             // current nb audio callback 
int last_nbcb=0;
int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*tempo); // Weird ?
int last_nbcb_ch_step=0;// nb audio callback from last step
int debug=1;
int t=0;                // index of track



int repeat=0;
int quit=0;             // do we need to quit ?
int cursor=0;           // cursor position in the sequencer
int note=0;
int attack=0;
int release=0;


int start_key=0;        // start key pressed ?
int step=0;             // current step in the sequencer
int menu=0;             // menu mode
int menu_note=0;
int menu_cursor=0;      // index of menu
int ct=0;               // current_track
int invert_trig=0;

int dirty_graphic=1;

//char * tmp_str;

void display_board()
{
  int i;
  char str[8];
  dirty_graphic=0;

  SG.clearScreen();
  sprintf(str,"Track %d ",ct);
  SG.guiTTFText(200,20,str);
  
  if (menu_cursor==0)
    sprintf(str,"A/D");
  if (menu_cursor==1)
    sprintf(str,"Note");
  if (menu_cursor==2)
    sprintf(str,"L/S");

  SG.guiTTFText(200,40,str);

  // Draw all box default color   
  for (i=0;i<16;i++)
    { SG.drawBoxNumber(i,BOX_COLOR); }
     

  // Attack/Release
  if (menu_cursor==0)
    {
      for (i=0;i<16;i++)
	{
	  // Draw trigged box trig color   
	  if (P[ct].getPatternElement(i).getTrig())
	    SG.drawBoxNumber(i,TRIG_COLOR);
	  // AdsR
	  SG.smallBoxNumber(i,P[ct].getPatternElement(i).getRelease(),0,SMALLBOX_COLOR);
	  SG.smallBoxNumber(i,0,P[ct].getPatternElement(i).getAttack(),SMALLBOX_COLOR);      
	}
      // Cursor & step postion
      SG.drawBoxNumber(cursor,CURSOR_COLOR);
      SG.smallBoxNumber(cursor,P[ct].getPatternElement(cursor).getRelease(),0,SMALLBOX_COLOR);
      SG.smallBoxNumber(cursor,0,P[ct].getPatternElement(cursor).getAttack(),SMALLBOX_COLOR); 
      
      SG.drawBoxNumber(step,STEP_COLOR);  
      SG.smallBoxNumber(step,P[ct].getPatternElement(step).getRelease(),0,SMALLBOX_COLOR);
      SG.smallBoxNumber(step,0,P[ct].getPatternElement(step).getAttack(),SMALLBOX_COLOR); 
    }


  // Note
  if (menu_cursor==1)
    {
      if (menu_note==0)      
	for (i=0;i<16;i++)
	  {
	    // Draw trig note color
	    if (P[ct].getPatternElement(i).getTrig())
	      SG.drawBoxNumber(i,NOTE_COLOR);
	    
	    SG.smallBoxNumber(i,
			      (P[ct].getPatternElement(i).getNote()%12)*10,
			      (P[ct].getPatternElement(i).getNote()/12)*10,
			      SMALLBOX_COLOR);
	  }
      // Note C3 
      if (menu_note==1)
	{	  
	  for (i=0;i<16;i++)
	    {
	      // Draw trig note color
	      if (P[ct].getPatternElement(i).getTrig())
		SG.drawBoxNumber(i,NOTE_COLOR);
	      
	      if (i==cursor)
		SG.drawBoxNumber(cursor,CURSOR_COLOR);
	      if (i==step)
		SG.drawBoxNumber(step,STEP_COLOR);  

	      if (P[ct].getPatternElement(i).getTrig())
		SG.drawTTFTextNumber(i,P[ct].getPatternElement(i).getNoteCharStar());
	    }
	}
      // Note Cursor
      if (menu_note==0)
	{
	  SG.drawBoxNumber(cursor,CURSOR_COLOR);
	  SG.drawBoxNumber(step,STEP_COLOR);  
    
	  SG.smallBoxNumber(cursor,
			    (P[ct].getPatternElement(cursor).getNote()%12)*10,
			    (P[ct].getPatternElement(cursor).getNote()/12)*10,
			    SMALLBOX_COLOR);
	  SG.smallBoxNumber(step,  
			    (P[ct].getPatternElement(step).getNote()%12)*10,
			    (P[ct].getPatternElement(step).getNote()/12)*10,
			    SMALLBOX_COLOR);
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
  //printf("%d %d %d\n",lastKey,lastEvent,lastKey==&& SDLK_RETURN && lastEvent==SDL_KEYUP);
  //  printf("lastevent=%d\n",lastEvent);
  if (lastKey   == SDLK_ESCAPE && lastEvent ==  SDL_KEYUP)
    {
      printf("Entering menu\n");
      if      (menu==0)        { menu=1;  start_key=0; }
      else if (menu==1)        { menu=0;  start_key=0; }   
      IE.clearLastKeyEvent();
    }

  if (lastKey   == SDLK_RETURN && lastEvent ==  SDL_KEYUP)
    {
      printf("Entering menu note\n");
      if      (menu_note==0)        { menu_note=1;  }
      else if (menu_note==1)        { menu_note=0;  }   
      IE.clearLastKeyEvent();
    }



  //Move MENU_CURSOR
  if (menu==1)
    {
      if(keyState[SDLK_LEFT]) 
	{
	  if (keyRepeat[SDLK_LEFT]==1 || keyRepeat[SDLK_LEFT]%64==0)
	    menu_cursor--;
	  if (menu_cursor<0) menu_cursor=4;
	  printf("[menu_cursor:%d]\n",menu_cursor);
	  printf("key left\n");            
	  dirty_graphic=1;
	}      

      if(keyState[SDLK_RIGHT])
	{
	  if (keyRepeat[SDLK_RIGHT]==1 || keyRepeat[SDLK_RIGHT]%64==0)
	    menu_cursor++;
	  if (menu_cursor>4) menu_cursor=0;
	  printf("[menu_cursor:%d]\n",menu_cursor);
	  printf("key right\n");            
	  dirty_graphic=1;
	}

      if(keyState[SDLK_UP])
	{
	  if (keyRepeat[SDLK_UP]==1 || keyRepeat[SDLK_UP]%64==0)
	    ct++;
	  if (ct >= TRACK_MAX) ct=0;
	  printf("key up\n");
	  dirty_graphic=1;
	}

      if(keyState[SDLK_DOWN])
	{
	  if (keyRepeat[SDLK_DOWN]==1 || keyRepeat[SDLK_DOWN]%64==0)
	    ct--;
	  if (ct<0) ct=TRACK_MAX-1;
	  printf("key down\n");
	  dirty_graphic=1;
	}
    }






  //MOVE the cursor : LEFT UP DOWN RIGHT   
  if (menu==0)
    {



      if(keyState[SDLK_UP] && ! keyState[SDLK_LCTRL])
	{
	  if (keyRepeat[SDLK_UP]==1 || keyRepeat[SDLK_UP]%64==0)
	    cursor=cursor-4;
	  if (cursor < 0) cursor=cursor +16;
	  printf("key down : up \n");
	  dirty_graphic=1;
	}

      if(keyState[SDLK_DOWN] && ! keyState[SDLK_LCTRL])
	{
	  if (keyRepeat[SDLK_DOWN]==1 || keyRepeat[SDLK_DOWN]%64==0)
	    cursor=( cursor+4 ) %16;
	  printf("key down : down\n");
	  dirty_graphic=1;
	}
      
      if(keyState[SDLK_LEFT] && ! keyState[SDLK_LCTRL])
	{
	  if (keyRepeat[SDLK_LEFT]==1 || keyRepeat[SDLK_LEFT]%64==0)
	      cursor--;
	  
	  if (cursor<0) cursor=15;
	  printf("key left\n");            
	  dirty_graphic=1;
	}
      
      if (keyState[SDLK_RIGHT] && ! keyState[SDLK_LCTRL])
	{
	  if (keyRepeat[SDLK_RIGHT]==1 || keyRepeat[SDLK_RIGHT]%64==0)
	  cursor++;
	  if (cursor>15) cursor=0;
	  printf("key right\n");      
	  dirty_graphic=1;
	}
    }

  // Move Attack Release 
  // Insert/Remove Trig
  if (menu==0 && menu_cursor==0)
    {
      if (lastKey== SDLK_LALT && lastEvent == SDL_KEYDOWN)
	{
	  invert_trig=1;
	  printf("key lalt\n");      
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	}

      if (keyState[SDLK_LEFT]  && keyState[SDLK_LCTRL])
	if (keyRepeat[SDLK_LEFT]==1 ||  keyRepeat[SDLK_LEFT]>16) 
	  { release=-4;   dirty_graphic=1; }

      if (keyState[SDLK_RIGHT] && keyState[SDLK_LCTRL]) 
	if (keyRepeat[SDLK_RIGHT]==1 || keyRepeat[SDLK_RIGHT]>16) 
	  { release=4; 	  dirty_graphic=1; }

      if (keyState[SDLK_UP]    && keyState[SDLK_LCTRL]) 
	if (keyRepeat[SDLK_UP]==1 ||    keyRepeat[SDLK_UP]>16) 
	  { attack=4;  	  dirty_graphic=1; }

      if (keyState[SDLK_DOWN]  && keyState[SDLK_LCTRL]) 
	if (keyRepeat[SDLK_DOWN]==1 || keyRepeat[SDLK_DOWN]>16) 
	  { attack=-4; 	  dirty_graphic=1; }
    }  

  if (menu==0 && menu_cursor==1)
    {
      //if (keyState[SDLK_LALT])
      if (lastKey== SDLK_LALT && lastEvent ==  SDL_KEYUP)
	{
	  invert_trig=1;
	  printf("key lalt\n");      
	  dirty_graphic=1;
	  IE.clearLastKeyEvent();
	}
      
      if (keyState[SDLK_LEFT]  && keyState[SDLK_LCTRL]) { note=-1; 	  dirty_graphic=1;}
      if (keyState[SDLK_RIGHT] && keyState[SDLK_LCTRL]) { note=1;  	  dirty_graphic=1;}
      if (keyState[SDLK_UP]    && keyState[SDLK_LCTRL]) { note=12;   	  dirty_graphic=1;}
      if (keyState[SDLK_DOWN]  && keyState[SDLK_LCTRL]) { note=-12;  	  dirty_graphic=1;}
    }  

  if (menu==0 && menu_cursor==2)
    {
      if (keyState[SDLK_DOWN]  && keyState[SDLK_LALT])
	save=true;
      if (keyState[SDLK_UP]    && keyState[SDLK_LALT])
	load=true;
    }

  int delay=10;
  //printf("sleeping %dms\n",delay);
  SDL_Delay(delay);


}


int seq_update()
{
  
  if (save)
    {
      printf("[SAVE]\n");
      PR.writePattern(1,ct+1,P[ct]);
      save=false;
    }
  
  if (load)
    {
      printf("[LOAD]\n");
      PR.readPatternData(1,ct+1,P[ct]);
      load=false;
    }
  
  if (note!=0)
    { 
      P[ct].getPatternElement(cursor).setNote(P[ct].getPatternElement(cursor).getNote()+note);
      note=0;
    }
  
  
  if (attack!=0)
    {
      //m0.getADSR().setAttack(m0.getADSR().getAttack()+attack);
      P[ct].getPatternElement(cursor).setAttack(P[ct].getPatternElement(cursor).getAttack()+attack);
      attack=0;
      if (debug)
	printf("[attack:%d]\n",M[ct]->getADSR().getAttack());
    }
  
  if (release!=0)
    {
      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
      P[ct].getPatternElement(cursor).setRelease(P[ct].getPatternElement(cursor).getRelease()+release);
      release=0;
      if (debug)
	printf("[release:%d]\n",P[ct].getPatternElement(cursor).getRelease()+release);
    }
  
  if (invert_trig)
    {
      P[ct].getPatternElement(cursor).setTrig(! P[ct].getPatternElement(cursor).getTrig());
      PE=P[ct].getPatternElement(cursor);
      invert_trig=0;
    }	  
  
  
  
  if (step==16) { step=0; }
  if (debug)
    printf("STEP:%d\n",step);	  
  
  
  
  for (t=0;t<TRACK_MAX;t++)
    {
      if (P[t].getPatternElement(step).getTrig()==true)
	{
	  float f=P[t].getPatternElement(step).getNoteFreq();
	  int   i=f;
	  
	  
	  //M[t].getVCO()->setSynthFreq(i);

	  //m0.getADSR().get();
	  //M[t].getOscillator()->reset();
	  M[t]->getADSR().reset();;	  
	  M[t]->getVCO().reset();
	  M[t]->getVCO().getOscillatorOne();

	  M[t]->getVCO().setSynthFreq(i);
	  M[t]->getADSR().setRelease(P[t].getPatternElement(step).getRelease());		  
	  //m.setSynthFreq(1200);
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

  for (t=0;t<TRACK_MAX;t++)
    {

      MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer();
      //MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer().getInput().init();
      
      M[t]=MM[t]->getInput();
      M[t]->init();
      M[t]->getVCO().init();
      //M[t]->getVCO().init();
      M[t]->getVCO().tick();
      M[t]->getVCO().setSynthFreq(0);
      
      //M[t].getVCO().setSineOsc();
    }
  //  AE.processBuffer();
  //  for (t=0;t<TRACK_MAX;t++)
  //    {
      //MM[t]->setInput(&M[t]);
      //      MM[t]->setAmplitude(32);
      //    }

  printf("openAudio start streaming\n");
  //  sleep(2);
  AE.startAudio();

  seq_update();  
  while (true)
    {
      nbcb=AE.getNbCallback();

      if (nbcb>last_nbcb)
	{
	  last_nbcb=nbcb;
	}
      handle_key();

      // if user want to quit via handle_key
      if (quit)
	return(0);

      //GRAPHICS
      if (dirty_graphic)
	display_board();

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

void loadPattern()
{
  int i;

  string fileName="data.pic";
  //PR.setFileName("data.pic");
  PR.setFileName(fileName);
  for (i=0;i<TRACK_MAX;i++)
    PR.readPatternData(1,i+1,P[i]);

}



int main()
{
  //  exit(0);
  //string wave="808-cowbell.wav";
  //  char * str="808-cowbell.wav";
  //tmp_str=(char*)malloc(sizeof(char)*4);
  //cowbell.loadWave(str);
  loadPattern();
  SG.initVideo();
  //handle_key();
  //  SDL_EnableKeyRepeat(500,500);
  SG.openBMPFont();
  if (SG.openTTFFont()==false) { printf("ttf font error\n"); exit(1); }
  display_board();

  printf("openAudio output\n");
  AE.openAudio();

  seq();

  SG.closeVideo();
  AE.closeAudio();
  //sleep(10);
  //PE.print();
}
