using namespace std;

#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              

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
vector <Effect    *>        FX(TRACK_MAX);



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
      MM[t]->init();
      MM[t]->setAmplitude(0);

      M[t]=MM[t]->getInput();
      M[t]->init();
      FX[t] = MM[t]->getEffect();                             
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
  
  t=0;
      
  printf("[Freq:%d]\n",i);
  
  MM[t]->setAmplitude(64);
  MM[t]->setMachineType(1);
  
  M[t]  = MM[t]->getInput();                             
  FX[t] = MM[t]->getEffect();                             
  
  FX[t]->setDepth(0);
  FX[t]->setSpeed(0);
  
  M[t]->setF(OSC1_FREQ,440);
  
  M[t]->setI(OSC12_MIX,16);
  M[t]->setI(OSC1_PHASE,16);
  
  M[t]->setI(OSC1_TYPE,1);
  M[t]->setI(OSC2_TYPE,1);
  
  
  M[t]->setI(ADSR_ENV0_ATTACK, 64);
  M[t]->setI(ADSR_ENV0_DECAY,  64);
  M[t]->setI(ADSR_ENV0_SUSTAIN,64);
  M[t]->setI(ADSR_ENV0_RELEASE,64);
  
  M[t]->setI(ADSR_ENV1_ATTACK, 64);
  M[t]->setI(ADSR_ENV1_ATTACK, 64);
  M[t]->setI(ADSR_ENV1_ATTACK, 64);
  M[t]->setI(ADSR_ENV1_RELEASE,64);
  
  
  
  M[t]->setI(FILTER1_CUTOFF,64);
  M[t]->setI(FILTER1_RES,64);
  
  // play the attack of the note for 1 second
  M[t]->setI(NOTE_ON,1);
  sleep(1);

  // Release the note
  M[t]->setI(NOTE_ON,0);
  sleep(2);

  // play a note 2* Higher
  M[t]->setF(OSC1_FREQ,880);
  M[t]->setI(NOTE_ON,1);
  sleep(2);

  // Release the note
  M[t]->setI(NOTE_ON,0);
  sleep(2);

  // play a note 4* lower
  M[t]->setF(OSC1_FREQ,220);
  M[t]->setI(NOTE_ON,1);
  sleep(3);

  // Release the note
  M[t]->setI(NOTE_ON,0);
  sleep(1);

}

void wtg()
{
  Generator G;
  WaveTableManager & WTM = WaveTableManager::getInstance();
  WaveTable* WT;

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.sine();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_SINE);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.saw();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_SAW);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.pulse();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_PULSE);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.triangle();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_TRGL);


  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.noise();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_NOISE);

  printf("wavetablemanager.getSize : %d\n",WTM.getSize());

}



int main(int argc,char ** argv)
{	  
  //if (argc!=2) { printf("arg please\n"); exit(1); }
  //  file_buffer=(Sint16*)malloc(sizeof(short)*DEFAULTSAMPLES);
  //  vector_buffer=(Sint16*)malloc(sizeof(Sint16)*SCREEN_WIDTH);


  struct stat fileStat;
  wtg();
  openaudio();


  attack=0;
  release=128;
  vcomix=64;
  PE.setNote(12);
  func();


  closeaudio();
  return 0;
}

