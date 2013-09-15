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

AudioEngine ae;

#define TRACK_MAX 2
vector <Pattern> P(TRACK_MAX);  
vector <Machine> M(TRACK_MAX);

//Pattern P0;
//Pattern P1;

//PatternElement PE0;
//PatternElement PE1;

SDL_GUI sg;

//MonoMixer MM0;
//MonoMixer MM1;

Wave cowbell;
//Synth beeper;
Instrument inst;


bool left_key=false;
bool right_key=false;
bool up=false;
bool down=false;
bool pageup=false;
bool pagedown=false;
bool ctrl=false;

bool q_key=false; //C
bool z_key=false; //C+
bool s_key=false; //D
bool e_key=false; //D+
bool d_key=false; //E
bool r_key=false; //NULL
bool f_key=false; //F
bool t_key=false; //F+
bool g_key=false; //G
bool y_key=false; //G+
bool h_key=false; //A
bool u_key=false; //A+
bool j_key=false; //B

bool k_key=false; //C
bool o_key=false; //C+
bool l_key=false; //D
bool p_key=false; //D+
bool m_key=false; //E

int quit=0;                // do we need to quit ?
int cursor=0;              // cursor position in the sequencer
int note=0;
int attack=0;
int release=0;

int start_key=0;        // start key pressed ?
int step=0;             // current step in the sequencer
int menu=0;             // menu mode
int menu_cursor=0;      // index of menu
int ct=0;               // current_track


void display_board()
{
  int i;
  char str[8];
   
  for (i=0;i<16;i++)
    { sg.drawBoxNumber(i,0xAECD15); }
    

  // Attack/Release
  if (menu_cursor==0)
    {
      for (i=0;i<16;i++)
	{
	  if (P[ct].getPatternElement(i).getTrig())
	    sg.drawBoxNumber(i,0x0EDC15);
	  sg.smallBoxNumber(i,P[ct].getPatternElement(i).getRelease(),0,0x442233);
	  sg.smallBoxNumber(i,0,P[ct].getPatternElement(i).getAttack(),0x662233);      
	}
      sg.drawBoxNumber(cursor,0x1545CD);
      sg.smallBoxNumber(cursor,P[ct].getPatternElement(cursor).getRelease(),0,0x442233);
      sg.smallBoxNumber(cursor,0,P[ct].getPatternElement(cursor).getAttack(),0x662233); 
      
      sg.drawBoxNumber(step,0x045c15);  
      sg.smallBoxNumber(step,P[ct].getPatternElement(step).getRelease(),0,0x442233);
      sg.smallBoxNumber(step,0,P[ct].getPatternElement(step).getAttack(),0x662233); 
    }


  // Note
  if (menu_cursor==1)
    {

      for (i=0;i<16;i++)
	{
	  if (P[ct].getPatternElement(i).getTrig())
	    sg.drawBoxNumber(i,0x0EDC15);
	  sg.smallBoxNumber(i,
			    (P[ct].getPatternElement(i).getNote()%12)*10,
			    (P[ct].getPatternElement(i).getNote()/12)*10,
			    0x442233);
	}      

      sg.drawBoxNumber(cursor,0x1545CD);
      sg.drawBoxNumber(step,0x045c15);  
      
      sg.smallBoxNumber(cursor,
			(P[ct].getPatternElement(cursor).getNote()%12)*10,
			(P[ct].getPatternElement(cursor).getNote()/12)*10,
			0x442233);
      sg.smallBoxNumber(step,  
			(P[ct].getPatternElement(step).getNote()%12)*10,
			(P[ct].getPatternElement(step).getNote()/12)*10,
			0x442233);
    }

  sg.refresh();
}


void openaudio()
{
  //beeper.initialize(1000,440);
  //inst.setSynth();
  //inst.assignSynth(beeper);
  ae.set_instrument(inst);
  printf("Before\n");
  ae.openAudio();
  printf("After\n");
}


void handle_key()
{
  SDL_Event event;
  if (SDL_PollEvent(&event))
    {
      // printf("loop %d\n",loop);
      //loop++;
      
      //printf("[%d %d %d %d]\n",SDL_KEYUP,SDL_KEYDOWN,event.type,event.key.keysym.sym);
      switch (event.type)
	{
	case SDL_QUIT:
	  printf("Exiting...\n");
	  quit = 1;	  
	  exit(0);
	  break; 	  
	  
	case SDL_KEYUP:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_LCTRL:       ctrl=false;      break;
	    case SDLK_RCTRL:       ctrl=false;      break;
	    case SDLK_LEFT:        left_key=false;  break;	     
	    case SDLK_RIGHT:       right_key=false; break;	      
	    case SDLK_UP:          up=false;        break;	      
	    case SDLK_DOWN:        down=false;      break;
	    case SDLK_PAGEUP:      pageup=false;    break;	      
	    case SDLK_PAGEDOWN:    pagedown=false;  break;
	      
	    case SDLK_q:           q_key=false;     break;
	    case SDLK_z:           z_key=false;     break;
	    case SDLK_s:           s_key=false;     break;
	    case SDLK_e:           e_key=false;     break;
	    case SDLK_d:           d_key=false;     break;
	    case SDLK_r:           r_key=false;     break;
	    case SDLK_f:           f_key=false;     break;
	    case SDLK_t:           t_key=false;     break;
	    case SDLK_g:           g_key=false;     break;
	    case SDLK_y:           y_key=false;     break;
	    case SDLK_h:           h_key=false;     break;
	    case SDLK_u:           u_key=false;     break;
	    case SDLK_j:           j_key=false;     break;
	    case SDLK_k:           k_key=false;     break;
	    case SDLK_o:           o_key=false;     break;
	    case SDLK_l:           l_key=false;     break;
	    case SDLK_p:           m_key=false;     break;
	    case SDLK_m:           m_key=false;     break;
	      
	    case SDLK_RETURN:     start_key=2; break;
	      
	    default:
	      break;
	    }
	  break;
	  
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_ESCAPE:
	      printf("Exiting...\n");
	      quit = 1;
	      exit(0);
	      break;
	      
	    case SDLK_LCTRL:       ctrl=true;      break;
	    case SDLK_RCTRL:       ctrl=true;      break;
	    case SDLK_LEFT:        left_key=true;  break;	      
	    case SDLK_RIGHT:       right_key=true; break;	      
	    case SDLK_UP:          up=true;        break;	      
	    case SDLK_DOWN:        down=true;      break;
	    case SDLK_PAGEUP:      pageup=true;    break;	      
	    case SDLK_PAGEDOWN:    pagedown=true;  break;
	      
	    case SDLK_q:           q_key=true;     break;
	    case SDLK_z:           z_key=true;     break;
	    case SDLK_s:           s_key=true;     break;
	    case SDLK_e:           e_key=true;     break;
	    case SDLK_d:           d_key=true;     break;
	    case SDLK_r:           r_key=true;     break;
	    case SDLK_f:           f_key=true;     break;
	    case SDLK_t:           t_key=true;     break;
	    case SDLK_g:           g_key=true;     break;
	    case SDLK_y:           y_key=true;     break;
	    case SDLK_h:           h_key=true;     break;
	    case SDLK_u:           u_key=true;     break;
	    case SDLK_j:           j_key=true;     break;
	    case SDLK_k:           k_key=true;     break;
	    case SDLK_o:           o_key=true;     break;
	    case SDLK_l:           l_key=true;     break;
	    case SDLK_p:           m_key=true;     break;
	    case SDLK_m:           m_key=true;     break;
	      
	    case SDLK_RETURN:      start_key=1; break;

	    default:
	      break;
	      
	    }
	  
	  break;
	  
	}
      printf("new event:%d %s\n",event.type,SDL_GetKeyName(event.key.keysym.sym));
    }
  
  //printf("No new event\n");

  if (start_key==2) 
    {
      if (menu==0)        { menu=1;  start_key=0; }
	else if (menu==1) { menu=0;  start_key=0; }
    }


  if (menu==1)
    {
      if(left_key)
	{
	  menu_cursor--;
	  if (menu_cursor<0) menu_cursor=4;
	  printf("[menu_cursor:%d]\n",menu_cursor);
	  printf("key left\n");            
	}      

      if(right_key)
	{
	  menu_cursor++;
	  if (menu_cursor>4) menu_cursor=0;
	  printf("[menu_cursor:%d]\n",menu_cursor);
	  printf("key right\n");            
	}

      if(up)
	{
	  ct++;
	  if (ct >= TRACK_MAX) ct=0;
	  printf("key up\n");
	}

      if(down)
	{
	  ct--;
	  if (ct<0) ct=TRACK_MAX-1;
	  printf("key down\n");
	}
    }

  
  if (menu==0)
    {
      if(up)
	{
	  cursor=cursor-4;
	  if (cursor < 0) cursor=cursor +16;
	  printf("key down\n");
	}

      if(down)
	{
	  cursor=( cursor+4 ) %16;
	  //if (cursor > 15) cursor=cursor-12;
	  printf("[cursor:%d]\n",cursor);
	  printf("key down\n");
	}
      
      if(left_key)
	{
	  cursor--;
	  if (cursor<0) cursor=15;
	  printf("[cursor:%d]\n",cursor);
	  printf("key left\n");            
	}
      
      if (right_key)
	{
	  cursor++;
	  if (cursor>15) cursor=0;
	  printf("key right\n");      
	}
    }
  if(pageup) { printf("key pgup \n");        }

  if(pagedown) { printf("key pgdown \n");    }



  if (s_key) { attack=-4;  }
  if (z_key) { attack=4;   }

  if (e_key) { release=4; }
  if (d_key) { release=-4;  }

  if (r_key) { note=1;   }
  if (f_key) { note=-1;  }




  //No user activity => sleeping a while to keep cpu cool
  //it keep cpu under 100% usage
  if ((up       || 
       down     || 
       left     || 
       right    || 
       pagedown || 
       pageup   ||
       q_key    ||
       z_key    ||
       k_key    
       )==false)
  {
    int delay=40;
    //    printf("sleeping %dms\n",delay);
    SDL_Delay(delay);
  }
  else
    {
      int delay=10;
      //printf("sleeping %dms\n",delay);
      SDL_Delay(delay);
    }
}



int seq()
{
  //  Machine m0;
  //  Machine m1;

  int tempo=60;

  int nbcb=0;      // current nb audio callback 
  int last_nbcb=0; // nb audio callback from last step
  int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*tempo); // Weird ?
  int debug=1;
  int t=0; // index of track


  AudioMixer & am=ae.getAudioMixer();
  //Track & t0=ae.getAudioMixer().getTrack(0);

  //  m0.setSawOsc();
  //m0.setSawOsc();
  M[0].setFuzzyPulseOsc();
  M[1].setSineOsc();
  //  m1.setSawOsc();

  //  m0.setSineOsc();
  //m1.setSawOsc();


  M[0].setSynthFreq(0);
  M[1].setSynthFreq(0);

  //Track     & t0=ae.getAudioMixer().getTrack(0);
  MonoMixer & mm0=ae.getAudioMixer().getTrack(0).getMonoMixer();
  MonoMixer & mm1=ae.getAudioMixer().getTrack(1).getMonoMixer();

  mm0.setInput(&M[0]);
  mm1.setInput(&M[1]);

  ae.startAudio();
  //Machine & m0=ae.getAudioMixer().getTrack(0).getMachine();

  //  Track & t1=ae.getAudioMixer().getTrack(1);
  //  Machine & m1=ae.getAudioMixer().getTrack(1).getMachine();


  
  //m0.setSawOsc();
  //  m1.setSineOsc();
  //m0.setSawOsc();
  //  m0.setFuzzyPulseOsc();
  //  m1.setSawOsc();
  //m1.setSineOsc();
  //m0.setSawOsc();
  //m1.setSawOsc();

  //  am=ae.getAudioMixer();
  //  return 0;
  while (true)
    {
      nbcb=ae.getNbCallback();

      //if (nbcb>last_nbcb)
      //{
	  handle_key();
	  SDL_Delay(40);
	  //GRAPHICS
	  display_board();
	  
	  //if (step > 0)  sg.drawBoxNumber(step-1,0xAECD15);
	  //	  if (step == 0) sg.drawBoxNumber(15,0xAECD15);
	  //sg.drawBoxNumber(cursor,0x1545CD);
	  //sg.refresh();
	  //GRAPHICS

	  //	}

      if (nbcb-last_nbcb>nb_cb_ch_step)
	{
	  //printf("[cursor:%d]\n",cursor);

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
		printf("[attack:%d]\n",M[ct].getADSR().getAttack());
	    }

	  if (release!=0)
	    {
	      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
	      P[ct].getPatternElement(cursor).setRelease(P[ct].getPatternElement(cursor).getRelease()+release);
	      release=0;
	      if (debug)
		printf("[release:%d]\n",P[ct].getPatternElement(cursor).getRelease()+release);

	    }

	  //if (step%3==0) { m.setSineOsc(); }
	  //if (step%3==1) { m.setSawOsc(); }
	  //if (step%3==2) { m.setFuzzyPulseOsc(); }
	  
	  //m0.setSawOsc(); 
	  //m1.setSawOsc(); 
	  //m.setFuzzyPulseOsc();
	  //m0.setSineOsc();
	  //m1.setSineOsc();
	  



	  if (step==16) { step=0; }
	  if (debug)
	    printf("STEP:%d\n",step);	  


	  

	  //printf("loop\n");    
	  last_nbcb=nbcb;
	  
	  for (t=0;t<TRACK_MAX;t++)
	    {
	      if (P[0].getPatternElement(step).getTrig()==true)
		{
		  float f=P[t].getPatternElement(step).getNoteFreq();
		  int   i=f;
		  
		  //printf("%f %d \n",P0.getPatternElement(step).getNoteFreq(),i);
		  M[t].setSynthFreq(i);
		  M[t].getADSR().reset();
		  //              m0.getADSR().setAttack(P0.getPatternElement(step).getAttack());
		  //	      m0.getADSR().setDecay(P0.getPatternElement(step).getDecay());
		  //	      m0.getADSR().setSustain(P0.getPatternElement(step).getSustain());
		  M[t].getADSR().setRelease(P[t].getPatternElement(step).getRelease());
		  
		  
		  //m0.getADSR().get();
		  M[t].getOscillator()->reset();
		  //m.setSynthFreq(1200);
		}
	      else
		{
		  //m.setSynthFreq(800);
		  //printf("m.setSynthFreq(0);\n");
		  //m0.setSynthFreq(0);
		}

	      /*
		if (P[ct].getPatternElement(step).getTrig()==true)
		{
		float f=P[1].getPatternElement(step).getNoteFreq();
		int   i=f;
		
		//printf("%f %d \n",P1.getPatternElement(step).getNoteFreq(),i);
		m1.setSynthFreq(i);
		m1.getADSR().reset();
		m1.getOscillator()->reset();
		//m.setSynthFreq(1200);
		}
		else
		{
		//m.setSynthFreq(800);
		//printf("m.setSynthFreq(0);\n");
		//m1.setSynthFreq(0);
		}
	      */
	    }	
	  step++;  
	}


    }
}

void loadPattern()
{
  PatternReader PR;
  string fileName="data.pic";
  //PR.setFileName("data.pic");
  PR.setFileName(fileName);
  PR.readPatternData(1,1,P[0]);
  PR.readPatternData(1,2,P[1]);

  //  P[1].print();
  //exit(0);
}



int main()
{
  //string wave="808-cowbell.wav";
  char * str="808-cowbell.wav";
  cowbell.loadWave(str);
  loadPattern();
  sg.initVideo();
  //handle_key();
  SDL_EnableKeyRepeat(500,500);
  sg.openFont();
  display_board();


  openaudio();
  seq();
  //sleep(10);
  //PE.print();
}
