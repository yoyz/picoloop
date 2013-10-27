using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "AudioEngine.h"
#include "MonoMixer.h" 
#include "InputManager.h"

AudioEngine    AE;          // used to  init alsa/rtaudio
InputManager   IE;          // used to  fetch key
vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);


#define SCREEN_DEPTH	16
#define NCOEF	(32)

int y_m=SCREEN_HEIGHT/2;
int divide=128;
bool redraw=true;
int loop=0;
int offset=0;
float zoom=1.0;
int quit;

int attack=0;
int release=63;
int vcomix=63;

char * filename;

int len=0;
Sint16 * file_buffer=0;
Sint16 * vector_buffer=0;
int filesize_octet=0;

int octave=25;

SDL_Surface *screen=NULL;

bool left_key=false;
bool right_key=false;



PatternElement PE;

void openaudio()
{
  int t;

  AE.openAudio();
  for (t=0;t<TRACK_MAX;t++)
    {
      MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer();
      M[t]=MM[t]->getInput();
      M[t]->init();
      M[t]->getADSR().init();
      M[t]->getVCO().init();
      M[t]->getVCO().setSynthFreq(0);      
    }


  printf("openAudio start streaming\n");
  AE.startAudio();

  int i=0;
  for (i=0;i<10;i++)
    printf("[%d]\n",M[0]->tick());

}




void init_video()
{
  if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
    }
  
  printf("SDL initialized.\n");
 
  screen = SDL_SetVideoMode(SCREEN_WIDTH, 
			    SCREEN_HEIGHT, 
			    SCREEN_DEPTH,			
			    SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_HWACCEL);
  
  if (screen == NULL)
    {
      fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
    }

}



void handle_key()
{

  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;
  int    noteon=0;
  int t;

  IE.handleKey();

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  if (IE.shouldExit())
    quit=1;

  
  if(lastKey == SDLK_UP && lastEvent ==  SDL_KEYUP)
    {
    if (divide<=32) divide=32;
    octave=octave+12;
    redraw=true;
    printf("key up octave:%d\n",octave);
    }

  if(lastKey== SDLK_DOWN && lastEvent ==  SDL_KEYUP)
    {
      if (divide>=4096) divide=4096;
      octave=octave-12;
      redraw=true;
      printf("key down octave:%d\n",octave);
    }

  if(lastKey==SDLK_LEFT)
    {
      offset=offset--;
      if (!keyState[SDLK_LCTRL]) 
	offset=offset-(10*zoom);
      else
	offset=offset-SCREEN_WIDTH;

      if (offset <=0) offset=0;
      redraw=true;
      printf("key left\n");            
    }

  if (lastKey==SDLK_RIGHT)
    {
      offset=offset++;
      if (!keyState[SDLK_LCTRL]) 
	offset=offset+(10*zoom);
      else
	offset=offset+SCREEN_WIDTH;

      redraw=true;
      printf("key right\n");      
    }

  if(lastKey==SDLK_PAGEUP)
    {
      zoom=zoom*1.1;
      if (zoom>1024.0) zoom=1024;
      redraw=true;
      printf("key pgup %f\n",zoom);      
    }

  if(lastKey==SDLK_PAGEDOWN)
    {
      zoom=zoom/1.1;
      if (zoom<0.01) zoom=0.01;
      redraw=true;
      printf("key pgdown %f\n",zoom);
    }

  if (lastKey==SDLK_w) { attack--; printf("attack %d\n",attack); }
  if (lastKey==SDLK_x) { attack++; printf("attack %d\n",attack); }

  if (lastKey==SDLK_c) { release--; printf("release %d\n",release); }
  if (lastKey==SDLK_v) { release++; printf("release %d\n",release); }

  if (lastKey==SDLK_b) { vcomix--; printf("vco %d\n",vcomix); }
  if (lastKey==SDLK_n) { vcomix++; printf("vco %d\n",vcomix); }


  if (lastEvent ==  SDL_KEYUP)
    {
      if (lastKey==SDLK_q) { printf("key q\n"); PE.setNote(0+octave);  noteon=1; }
      if (lastKey==SDLK_z) { printf("key z\n"); PE.setNote(1+octave);  noteon=1; }
      if (lastKey==SDLK_s) { printf("key s\n"); PE.setNote(2+octave);  noteon=1; }
      if (lastKey==SDLK_e) { printf("key e\n"); PE.setNote(3+octave);  noteon=1; }
      if (lastKey==SDLK_d) { printf("key d\n"); PE.setNote(4+octave);  noteon=1; }
      if (lastKey==SDLK_f) { printf("key f\n"); PE.setNote(5+octave);  noteon=1; }
      if (lastKey==SDLK_t) { printf("key t\n"); PE.setNote(6+octave);  noteon=1; }
      if (lastKey==SDLK_g) { printf("key g\n"); PE.setNote(7+octave);  noteon=1; }
      if (lastKey==SDLK_y) { printf("key y\n"); PE.setNote(8+octave);  noteon=1; }
      if (lastKey==SDLK_h) { printf("key h\n"); PE.setNote(9+octave);  noteon=1; }
      if (lastKey==SDLK_u) { printf("key u\n"); PE.setNote(10+octave);  noteon=1; }
      if (lastKey==SDLK_j) { printf("key j\n"); PE.setNote(11+octave);  noteon=1; }
      
      if (lastKey==SDLK_k) { printf("key k\n"); PE.setNote(12+octave);  noteon=1; }
      if (lastKey==SDLK_o) { printf("key o\n"); PE.setNote(13+octave);  noteon=1; }
      if (lastKey==SDLK_l) { printf("key l\n"); PE.setNote(14+octave);  noteon=1; }
      if (lastKey==SDLK_p) { printf("key p\n"); PE.setNote(15+octave);  noteon=1; }
      if (lastKey==SDLK_m) { printf("key m\n"); PE.setNote(16+octave);  noteon=1; }

      IE.clearLastKeyEvent(); 
    }

  if (noteon)
    {
      for (t=0;t<1;t++)
	{
	  float f=PE.getNoteFreq();
	  int   i=f;
	  
	  printf("[Freq:%d]\n",i);
	  M[t]->getADSR().reset();;	  
	  M[t]->getVCO().reset();
	  M[t]->getVCO().getOscillatorOne();
	  M[t]->getVCO().reset();

	  M[t]->getVCO().setSynthFreq(i);
	  M[t]->getVCO().setVCOMix(vcomix);		  
	  M[t]->getADSR().setRelease(release);
	  M[t]->getADSR().setAttack(attack);
	  //M[t]->getADSR().setRelease(P[t].getPatternElement(step).getRelease());		  
	  //	  M[t]->getADSR().setAttack(P[t].getPatternElement(step).getAttack());		  
	  //M[t]->getVCO().setVCOMix(P[t].getPatternElement(step).getVCOMix());	
	}
    }  
  
  //printf("key pressed sleeping 10ms\n");
  SDL_Delay(10);

}

void prepare_vector_buffer()
{
  Sint16 y_value;
  Sint32 j;
  Sint32 last_j;
  Sint32 idx=0;
  Sint64 average=0;
  Sint64 counter=0;
  for (int i = 0; i < SCREEN_WIDTH-1 ; i++)
    {
      last_j=j;
      j=offset+i*zoom;
      if (j<=filesize_octet*2)
	{
	    y_value=file_buffer[j]/divide;
	    /*
	  else
	    {
	      average=0;
	      counter=0;
	      for (idx=j;idx<offset+(i+1)*zoom;idx++)
		{
		  average=average+file_buffer[idx];
		  counter++;
		  idx++;

		  }
	      printf("%d %d\n",counter,average);
	      vector_buffer[i]=(average/counter)/divide;
	      continue;
	    }
	    */
 	}
      else
	{
	  y_value=0;
	}
      vector_buffer[i]=y_value;
    }
}

void draw_screen()
{
  SDL_Rect rect;
  short color;

  // rect.x rect.y rect.w rect.h
  // int x the x location of the rectangle's upper left corner
  // int y the y location of the rectangle's upper left corner
  // int w the width of the rectangle
  // int h the height of the rectangle

  //Blank screen
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));               

  for (int i = 0; i < SCREEN_WIDTH-1 ; i++)
    {
      rect.x = i;
      rect.w = 1;

      if (vector_buffer[i]>0)
	{
	  rect.y=((SCREEN_HEIGHT/2)-vector_buffer[i]);
	  rect.h=vector_buffer[i];
	}
      else
	{
	  rect.y=((SCREEN_HEIGHT/2));
	  rect.h=vector_buffer[i]*-1;	  
	}

      // Create a vertical colored line
      color=vector_buffer[i];
      SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, color, 0, 128));      
    }
  
  SDL_Flip(screen);
  SDL_Delay(1);
  redraw=false;  
}



int main(int argc,char ** argv)
{	  
  //if (argc!=2) { printf("arg please\n"); exit(1); }
  file_buffer=(Sint16*)malloc(sizeof(short)*DEFAULTSAMPLES);
  vector_buffer=(Sint16*)malloc(sizeof(Sint16)*SCREEN_WIDTH);

  PE.setNote(32);

  struct stat fileStat;
  //  if(stat(argv[1],&fileStat) < 0)    
  //return 1;

  //filename=argv[1];
  //load_raw(argv[1]);

  openaudio();
  init_video();

  
  /* Main loop */
  quit = 0;
  while(!quit)
    {
      if (redraw==true)
	{	  
	  prepare_vector_buffer();
	  draw_screen();
	  quit=0;
	  //SDL_Delay(100);
	}
      else 
	{ 
	  handle_key();
	}
    }

  return 0;
}

