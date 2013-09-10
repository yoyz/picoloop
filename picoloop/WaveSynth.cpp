using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "AudioEngine.h"
#include "MonoMixer.h" 
AudioEngine ae;


//#define SCREEN_WIDTH	640
//#define SCREEN_HEIGHT	480
#define SCREEN_DEPTH	16
#define NCOEF	(32)

int y_m=SCREEN_HEIGHT/2;
int divide=128;
bool redraw=true;
int loop=0;
int offset=0;
float zoom=1.0;
int quit;

char * filename;

int len=0;
Sint16 * file_buffer=0;
Sint16 * vector_buffer=0;
int filesize_octet=0;

SDL_Surface *screen=NULL;

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

Machine m0;
Machine m1;


//printf("Before\n");
//ae.openAudio();

//AudioMixer am;
//Track      t0;
//Machine    m0;
//AudioMixer & am=ae.getAudioMixer();
//Track      & t0=ae.getAudioMixer().getTrack(0);
//Machine    & m0=ae.getAudioMixer().getTrack(0).getMachine();

//Track      & t1=ae.getAudioMixer().getTrack(1);
//Machine    & m1=ae.getAudioMixer().getTrack(1).getMachine();

//AudioMixer & am;
//Track      & t0;
//Machine    & m0;

void openaudio()
{
  //beeper.initialize(1000,440);
  //inst.setSynth();
  //inst.assignSynth(beeper);
  //ae.set_instrument(inst);
//  printf("Before\n");

  ae.openAudio();
  ae.startAudio();

  m0.setSineOsc();
  m1.setSineOsc();
  //  m0.setSawOsc();
  //  m1.setSawOsc();

  m0.setSynthFreq(0);
  m1.setSynthFreq(0);

  MonoMixer & mm0=ae.getAudioMixer().getTrack(0).getMonoMixer();
  MonoMixer & mm1=ae.getAudioMixer().getTrack(1).getMonoMixer();

  mm0.setInput(&m0);
  mm1.setInput(&m1);
  /*
  ae.openAudio();
  am=ae.getAudioMixer();
  m0=ae.getAudioMixer().getTrack(0).getMachine();
  m0.setFuzzyPulseOsc();
  m0.setSynthFreq(440);
  */
  /*
  AudioMixer & am=ae.getAudioMixer();
  Track      & t0=ae.getAudioMixer().getTrack(0);
  Machine    & m0=ae.getAudioMixer().getTrack(0).getMachine();

  Track      & t1=ae.getAudioMixer().getTrack(1);
  Machine    & m1=ae.getAudioMixer().getTrack(1).getMachine();
  m0.setSynthFreq(440);
  m1.setSynthFreq(880);
  */
//  am=ae.getAudioMixer();
//  t0=ae.getAudioMixer().getTrack(0);
//  m0=ae.getAudioMixer().getTrack(0).getMachine();

//  printf("After\n");
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
  SDL_Event event;
  if (SDL_PollEvent(&event))
    {
      printf("loop %d\n",loop);
      loop++;

      //printf("[%d %d %d %d]\n",SDL_KEYUP,SDL_KEYDOWN,event.type,event.key.keysym.sym);
      switch (event.type)
	{
	case SDL_QUIT:
	  printf("Exiting...\n");
	  quit = 1;
	  free(file_buffer);
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

  printf("No new event\n");

  
  if(up)
    {
    if (divide<=32) divide=32;
    divide=divide/1.1;
    redraw=true;
    printf("key up\n");
    }

  if(down)
    {
      if (divide>=4096) divide=4096;
      divide=divide*1.1;
      redraw=true;
      printf("key down\n");
    }

  if(left_key)
    {
      offset=offset--;
      if (!ctrl) 
	offset=offset-(10*zoom);
      else
	offset=offset-SCREEN_WIDTH;

      if (offset <=0) offset=0;
      redraw=true;
      printf("key left\n");            
    }

  if (right_key)
    {
      offset=offset++;
      if (!ctrl) 
	offset=offset+(10*zoom);
      else
	offset=offset+SCREEN_WIDTH;

      redraw=true;
      printf("key right\n");      
    }

  if(pageup)
    {
      zoom=zoom*1.1;
      if (zoom>1024.0) zoom=1024;
      redraw=true;
      printf("key pgup %f\n",zoom);      
    }

  if(pagedown)
    {
      zoom=zoom/1.1;
      if (zoom<0.01) zoom=0.01;
      redraw=true;
      printf("key pgdown %f\n",zoom);
    }

  if (q_key) { printf("key q\n"); m0.setSynthFreq(110); m0.getADSR().reset(); }
  if (z_key) { printf("key z\n"); m0.setSynthFreq(116); m0.getADSR().reset(); }
  if (s_key) { printf("key s\n"); m0.setSynthFreq(123); m0.getADSR().reset(); }
  if (e_key) { printf("key e\n"); m0.setSynthFreq(130); m0.getADSR().reset(); }
  if (d_key) { printf("key d\n"); m0.setSynthFreq(138); m0.getADSR().reset(); }
  if (f_key) { printf("key f\n"); m0.setSynthFreq(146); m0.getADSR().reset(); }
  if (t_key) { printf("key t\n"); m0.setSynthFreq(155); m0.getADSR().reset(); }
  if (g_key) { printf("key g\n"); m0.setSynthFreq(164); m0.getADSR().reset(); }
  if (y_key) { printf("key y\n"); m0.setSynthFreq(174); m0.getADSR().reset(); }
  if (h_key) { printf("key h\n"); m0.setSynthFreq(184); m0.getADSR().reset(); }
  if (u_key) { printf("key u\n"); m0.setSynthFreq(195); m0.getADSR().reset(); }
  if (j_key) { printf("key j\n"); m0.setSynthFreq(207); m0.getADSR().reset(); }


  if (k_key) { printf("key k\n"); m0.setSynthFreq(220); m0.getADSR().reset(); }
  if (o_key) { printf("key o\n"); m0.setSynthFreq(233); m0.getADSR().reset(); }
  if (l_key) { printf("key l\n"); m0.setSynthFreq(246); m0.getADSR().reset(); }
  if (p_key) { printf("key p\n"); m0.setSynthFreq(261); m0.getADSR().reset(); }
  if (m_key) { printf("key m\n"); m0.setSynthFreq(277); m0.getADSR().reset(); }

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
      printf("sleeping 100ms");
      //      SDL_Delay(100);
    }
  else
    {
      printf("sleeping 10ms");
      SDL_Delay(10);
    }
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
	  //	  if (zoom<=1.0)
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
	y_value=0;

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

