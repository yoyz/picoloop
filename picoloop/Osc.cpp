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
      M[t]->reset();
      M[t]->getADSRAmp().init();
      M[t]->getADSRFltr().init();
      M[t]->getVCO().init();
      M[t]->getVCO().setSynthFreq(0);      
    }


  printf("openAudio start streaming\n");
  AE.startAudio();

  int i=0;
  for (i=0;i<10;i++)
    printf("[%d]\n",M[0]->tick());
}


void closeaudio()
{
  AE.closeAudio();
}





void func()
{
  int t;
  float f=PE.getNoteFreq();
  int   i=f;

  for (t=0;t<1;t++)
    {
      
      printf("[Freq:%d]\n",i);
      //M[t]->getVCO().setOscillator(0,0);
      //      M[t]->getVCO().setOscillator(1,2);

      //M[t]->getVCO().setSynthFreq(i);

      M[t]->getBiquad().reset();

      M[t]->getVCO().setSynthFreq(i);
      M[t]->getVCO().setOscillator(0,1);
      M[t]->getVCO().setOscillator(1,1);
      M[t]->getVCO().setVCOMix(vcomix);		  

      M[t]->getADSRAmp().setRelease(127);
      M[t]->getADSRAmp().setAttack(0);

      M[t]->getADSRFltr().setAttack(0);
      M[t]->getADSRFltr().setRelease(127);

      M[t]->getBiquad().setBiquad(0, 0.5+0.005, (0.5+0.005), 0.0);
      M[t]->getBiquad().calcBiquad();


      M[t]->getVCO().setVCOMix(64);
      M[t]->getADSRAmp().reset();;	  
      M[t]->getVCO().reset();
      M[t]->getVCO().getOscillatorOne();
      M[t]->getVCO().reset();
      

      M[t]->getVCO().setVCOMix(vcomix);		  
      M[t]->getADSRAmp().setRelease(release);
      M[t]->getADSRAmp().setAttack(attack);
      M[t]->getVCO().init();
      M[t]->getVCO().setSynthFreq(i);
    }     
  //sleep(4);
  usleep(1000);
  M[t]->getVCO().setSynthFreq(i*2);
  usleep(1000);
  M[t]->getVCO().setSynthFreq(i*4);
  sleep(3);
}


int main(int argc,char ** argv)
{	  
  //if (argc!=2) { printf("arg please\n"); exit(1); }
  //  file_buffer=(Sint16*)malloc(sizeof(short)*DEFAULTSAMPLES);
  //  vector_buffer=(Sint16*)malloc(sizeof(Sint16)*SCREEN_WIDTH);


  struct stat fileStat;

  openaudio();


  attack=0;
  release=128;
  vcomix=64;
  PE.setNote(12);
  func();


  closeaudio();
  return 0;
}

