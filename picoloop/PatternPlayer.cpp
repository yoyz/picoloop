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

Pattern P0;
Pattern P1;

PatternElement PE0;
PatternElement PE1;

SDL_GUI sg;

//MonoMixer MM0;
//MonoMixer MM1;

Wave cowbell;
//Synth beeper;
Instrument inst;

int quit;
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


int cursor=0;

int attack=0;
int release=0;

void display_board()
{
  int i;
  for (i=0;i<16;i++)
    {
      sg.drawBoxNumber(i,0xAECD15);
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
	      
	      
	    default:
	      break;
	      
	    }
	  
	  break;
	  
	}
      printf("new event:%d %s\n",event.type,SDL_GetKeyName(event.key.keysym.sym));
    }
  
  //printf("No new event\n");
  
  
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
      bool moved=false;
      /*
      if (cursor=0)     { cursor=3;  moved=true;   }
      if (cursor=4)     { cursor=7;  moved=true;   }
      if (cursor=8)     { cursor=11;  moved=true;   }
      if (cursor=12)    { cursor=15; moved=true;   }
      if (moved==false) { cursor++;  }
      */
      cursor--;
      if (cursor<0) cursor=15;
      printf("[cursor:%d]\n",cursor);
      printf("key left\n");            
    }

  if (right_key)
    {
      bool moved=false;
      /*
      if (cursor=3)     { cursor=0;  moved=true;   }
      if (cursor=7)     { cursor=4;  moved=true;   }
      if (cursor=11)    { cursor=8;  moved=true;   }
      if (cursor=15)    { cursor=12; moved=true;   }
      if (moved==false) { cursor++;  }
      */
      cursor++;
      printf("key right\n");      
    }

  if(pageup)
    {
      printf("key pgup \n");      
    }

  if(pagedown)
    {
      printf("key pgdown \n");
    }

  if (s_key) { attack=-1;  }
  if (z_key) { attack=1;   }
  if (e_key) { release=-1; }
  if (d_key) { release=1;  }



  /*
  if (k_key)
    {
            printf("key z\n");
	    //Machine    & m0=ae.getAudioMixer().getTrack(0).getMachine();
	    m0.setSynthFreq(880);
    }
  */

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
    //      printf("sleeping 100ms");
    //      SDL_Delay(100);
  }
  else
    {
      //printf("sleeping 10ms");
      //SDL_Delay(120);
    }
}



int seq()
{
  Machine m0;
  Machine m1;

  int tempo=60;
  int step=0;      // current step
  int nbcb=0;      // current nb audio callback 
  int last_nbcb=0; // nb audio callback from last step
  int nb_cb_ch_step=60*DEFAULT_FREQ/(BUFFER_FRAME*4*tempo); // Weird ?




  AudioMixer & am=ae.getAudioMixer();
  //Track & t0=ae.getAudioMixer().getTrack(0);

  //  m0.setSawOsc();
  //m0.setSawOsc();
  m0.setFuzzyPulseOsc();
  //  m1.setSawOsc();

  //  m0.setSineOsc();
  //m1.setSawOsc();
    m1.setSineOsc();

  m0.setSynthFreq(0);
  m1.setSynthFreq(0);

  //Track     & t0=ae.getAudioMixer().getTrack(0);
  MonoMixer & mm0=ae.getAudioMixer().getTrack(0).getMonoMixer();
  MonoMixer & mm1=ae.getAudioMixer().getTrack(1).getMonoMixer();

  mm0.setInput(&m0);
  mm1.setInput(&m1);

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
      if (nbcb-last_nbcb>nb_cb_ch_step)
	{
	  //printf("[cursor:%d]\n",cursor);
	  handle_key();

	  if (attack!=0)
	    {
	      //m0.getADSR().setAttack(m0.getADSR().getAttack()+attack);
	      P0.getPatternElement(cursor).setAttack(P0.getPatternElement(cursor).getAttack()+attack);
	      attack=0;
	      printf("[attack:%d]\n",m0.getADSR().getAttack());
	    }

	  if (release!=0)
	    {
	      //	      m0.getADSR().setRelease(m0.getADSR().getRelease()+release);
	      P0.getPatternElement(cursor).setRelease(P0.getPatternElement(cursor).getRelease()+release);
	      release=0;
	      printf("[release:%d %d]\n",m0.getADSR().getRelease(),P0.getPatternElement(cursor).getRelease()+release);

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
	  printf("STEP:%d\n",step);	  

	  display_board();
	  //GRAPHICS
	  sg.drawBoxNumber(step,0x045c15);
	  //if (step > 0)  sg.drawBoxNumber(step-1,0xAECD15);
	  //	  if (step == 0) sg.drawBoxNumber(15,0xAECD15);

	  sg.drawBoxNumber(cursor,0x1545CD);
	  sg.refresh();
	  //GRAPHICS
	  

	  //printf("loop\n");    
	  last_nbcb=nbcb;
	  if (P0.getPatternElement(step).getTrig()==true)
	    {
	      float f=P0.getPatternElement(step).getNoteFreq();
	      int   i=f;
	      
	      //printf("%f %d \n",P0.getPatternElement(step).getNoteFreq(),i);
	      m0.setSynthFreq(i);
	      m0.getADSR().reset();
	      //              m0.getADSR().setAttack(P0.getPatternElement(step).getAttack());
	      //	      m0.getADSR().setDecay(P0.getPatternElement(step).getDecay());
	      //	      m0.getADSR().setSustain(P0.getPatternElement(step).getSustain());
	      m0.getADSR().setRelease(P0.getPatternElement(step).getRelease());


	      //m0.getADSR().get();
	      m0.getOscillator()->reset();
	      //m.setSynthFreq(1200);
	    }
	  else
	    {
	      //m.setSynthFreq(800);
	      //printf("m.setSynthFreq(0);\n");
	      //m0.setSynthFreq(0);
	    }

	  if (P1.getPatternElement(step).getTrig()==true)
	    {
	      float f=P1.getPatternElement(step).getNoteFreq();
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
	      printf("m.setSynthFreq(0);\n");
	      m1.setSynthFreq(0);
	    }

	  step++;  
	}

      //Keyboard
      
      //Keyboard

      //am.setSynthFreq(880);
      //am.setSynthFreq(880);
      //ae.getAudioMixer().setSynthFreq(440);
      //t.setSynthFreq(880);
      //m.setSynthFreq(880);
      //printf("loop\n");
      usleep(10);
      //t.setSynthFreq(1660);
      //am.setSynthFreq(1660);
      //ae.getAudioMixer().setSynthFreq(880);
      //sleep(2);
      /*
	ae.setSynthFreq(880);      
	sleep(1);
	ae.setSynthFreq(660);      
	sleep(1); 
	ae.setSynthFreq(440);
	sleep(1);
	ae.setSynthFreq(220);      
	sleep(1);
      */
    }
}

void setPattern()
{
  PatternReader PR;
  string fileName="data.pic";
  //PR.setFileName("data.pic");
  PR.setFileName(fileName);
  PR.readPatternData(1,1,P0);
  //  PR.readPatternData(1,2,P1);

  /*
  P0.setPatternSize(16);
  //PE = P.getPatternElement(0);
  P0.getPatternElement(0).setNote(C3);
  P0.getPatternElement(0).setTrig(true);

  P0.getPatternElement(1).setNote(F3);
  P0.getPatternElement(1).setTrig(true);

  P0.getPatternElement(3).setNote(A3P);
  P0.getPatternElement(3).setTrig(true);

  P0.getPatternElement(5).setNote(D3P);
  P0.getPatternElement(5).setTrig(true);

  P0.getPatternElement(8).setTrig(true);
  P0.getPatternElement(8).setNote(G3);

  P0.getPatternElement(9).setTrig(true);
  P0.getPatternElement(9).setNote(C3);

  P0.getPatternElement(13).setTrig(true);
  P0.getPatternElement(13).setNote(D4P);

  P0.print();
  printf("\n");

  P1.setPatternSize(16);
  //PE = P.getPatternElement(0);
  P1.getPatternElement(0).setNote(C4);
  P1.getPatternElement(0).setTrig(true);

  P1.getPatternElement(1).setNote(G4P);
  P1.getPatternElement(1).setTrig(true);

  P1.getPatternElement(3).setNote(D5P);
  P1.getPatternElement(3).setTrig(true);

  P1.getPatternElement(5).setNote(C4);
  P1.getPatternElement(5).setTrig(true);

  P1.getPatternElement(8).setNote(D4P);
  P1.getPatternElement(8).setTrig(true);

  P1.getPatternElement(9).setNote(C4);
  P1.getPatternElement(9).setTrig(true);

  P1.getPatternElement(13).setNote(D3P);
  P1.getPatternElement(13).setTrig(true);

  P1.print();
  */
}


int main()
{
  //string wave="808-cowbell.wav";
  char * str="808-cowbell.wav";
  cowbell.loadWave(str);
  setPattern();
  sg.initVideo();
  //SDL_EnableKeyRepeat(25, );
  sg.openFont();
  display_board();


  openaudio();
  seq();
  //sleep(10);
  //PE.print();
}
