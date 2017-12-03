using namespace std;

//#include <SDL/SDL.h>
//#include <SDL/SDL_audio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Master.h"
#include "AudioEngine.h"
#include "MonoMixer.h" 
#include "InputManager.h"
#include "SDL_GUI.h"

#include "Generator.h"
#include "WaveTableManager.h"
#include "WaveTable.h"
//#include "PatternElement.h"

#include "NoteFreq.h"

SDL_GUI        SG;          // used to  initialize video
AudioEngine    AE;          // used to  init alsa/rtaudio
AudioMixer   * AM;
InputManager   IE;          // used to  fetch key

vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);
vector <Effect    *>        FX(TRACK_MAX);

/* begin external dependency not used in waveSynth */
int   menu_config_bank=0;           // will choose the bank to load at startup
int   menu_config_audioOutput=0;    // audioOutputNumber define in the config menu
int   menu_config_midiOutput=0;     //  midiOutputNumber define in the config menu
int   menu_config_midiInput=0;      //  midiInputNumber  define in the config menu
//int   menu_config_midiClockMode=0;  // 0 internal, 1 midi sync out, 2 midi sync in 
int   menu_config_audiopulseclock_out=0; // 0 no sync, 1 sync left, 2 sync right
int   menu_config_y=0;              // current selected item in menu_config

int counter_send_midi_clock=0;      // send n clock and decrement the counter each time 
int counter_send_midi_clock_six=0;  // send n clock and decrement the counter each time

int counter_recv_midi_clock=0;      // recv n clock and decrement the counter each time 
int counter_recv_midi_clock_six=0;  // recv n clock and decrement the counter each time

int counter_delta_midi_clock=0;
int delta_midi_clock=0;

int mmc_start=1;                    // we have received a mmc stop
int mmc_stop=0;


int32_t pal0[MAXCOLOR]={0x0,      0xC8A0A8, 0x59AADA, 0xFFA11C, 0x39CE90, 0x36B82B, 0x3E789A, 0xFFFFFF, 0x0 };
int32_t * pal=pal0;

/* end external dependency not used in waveSynth */

#define SCREEN_DEPTH	16
#define NCOEF	(32)

int y_m=SCREEN_HEIGHT/2;
int divide=128;
bool redraw=true;
int loop=0;
int offset=0;
float zoom=1.0;
int quit;


int current_op=1;
int current_machine=1;

int waveform_op1=0;
int waveform_op2=0;

int attack_amp=0;
int decay_amp=96;
int sustain_amp=96;
int release_amp=96;

int attack_fltr=0;
int decay_fltr=96;
int sustain_fltr=96;
int release_fltr=96;

int osc1_detune=64;
int osc2_detune=64;


int vcomix=63;
int phase=63;

int cutoff=112;
int resonance=16;

char * filename;

int len=0;
Sint16 * file_buffer=0;
Sint16 * vector_buffer=0;
int filesize_octet=0;

int octave=25;

SDL_Surface *screen=NULL;

bool left_key=false;
bool right_key=false;

int    noteon=0;
int    noteValue=0;

//PatternElement PE;

void processBuffer_updateMidiSendClockCounter() {}
void donothing() {}

void openaudio()
{
  int t;
  int i=0;
  AM=new AudioMixer();
  AM->setAudioVolume(128);
  AE.setAudioMixer(AM);
  AE.setupSequencerCallback(donothing);
  AE.setNbTickBeforeStepChange(1000);
  AE.setAudioOutput(0);
  AE.openAudio();
  //AE.getAudioMixer().setAudioVolume(128);
  for (t=0;t<TRACK_MAX;t++)
    {

      //MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer();
      MM[t]=AE.getAudioMixer()->getMonoMixer(t);      
      MM[t]->init();
      MM[t]->setAmplitude(128);
      //MM[t]->setMachineType(0);
      M[t]=MM[t]->getInput();
      M[t]->init();
      FX[t] = MM[t]->getEffect();                             
      //M[t]->reset();      
      //M[t]->getADSRAmp().init();
      //M[t]->getADSRFltr().init();
      //M[t]->getVCO().init();
      //M[t]->getVCO().setSynthFreq(0);      
    }


  printf("openAudio start streaming\n");
  AE.startAudio();


  for (i=0;i<10;i++)
    printf("[%d]\n",M[0]->tick());

}




void init_video()
{
  SG.initVideo();
}



void handle_key()
{

  //bool * keyState;
  //int  * keyRepeat;
  int    lastEvent;
  int    lastKey;
  int t;

  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();

  IE.handleKey();

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  if (IE.shouldExit())
    quit=1;

  /*
  if(lastKey == SDLK_UP && lastEvent ==  SDL_KEYUP)
    {
    if (divide<=32) divide=32;
    octave=octave+12;
    redraw=true;
    printf("key up octave:%d\n",octave);
    }
  */


  if (keyRepeat[SDLK_F1]%16
      )
    {
      current_op=1;
      printf("current_op:%d\n",current_op);
      redraw=true;

    }


  if (keyRepeat[SDLK_F2]%16

      )
    {
      current_op=2;
      printf("current_op:%d\n",current_op);
      redraw=true;

    }


  if (keyRepeat[SDLK_F3]%16
      && 
      lastEvent ==  SDL_KEYDOWN

      )
    {
      current_machine=current_machine-1;
      if (current_machine<0)
	current_machine=SYNTH_SIZE-1;
      printf("current_machine:%d\n",current_machine);
      redraw=true;

    }


  if (keyRepeat[SDLK_F4]%16
      && 
      lastEvent ==  SDL_KEYDOWN

      )
    {
      current_machine=current_machine+1;
      if (current_machine>SYNTH_SIZE)
	current_machine=0;
      printf("current_machine:%d\n",current_machine);
      redraw=true;

    }





  // OCTAVE
  if (keyRepeat[SDLK_w]%64    && 
      keyRepeat[SDLK_DOWN]%64 
      && 
      lastEvent ==  SDL_KEYDOWN
      )
    {
      octave=octave-12;
      if (octave <=1) octave=1;
      redraw=true;
      printf("[w]+[down] => octave:%d\n",octave);
    }


  if (keyRepeat[SDLK_w]%64    && 
      keyRepeat[SDLK_UP]%64   
      && 
      lastEvent ==  SDL_KEYDOWN)
      
    {
      octave=octave+12;
      if (octave >=73) octave=73;
      redraw=true;
      printf("[w]+[up] => octave:%d\n",octave);
    }


  // WaveForm
  if (keyRepeat[SDLK_w]%64    && 
      keyRepeat[SDLK_LEFT]%64 
      && 
      lastEvent ==  SDL_KEYDOWN
      )
    {
      if (current_op==1)
	{
	  waveform_op1=waveform_op1-1;
	  if (waveform_op1 <0) waveform_op1=3;
	  printf("[w]+[left] => waveform_op1:%d\n",waveform_op1);
	}
      if (current_op==2)
	{
	  waveform_op2=waveform_op2-1;
	  if (waveform_op2 <0) waveform_op2=3;
	  printf("[w]+[left] => waveform_op2:%d\n",waveform_op2);
	}

      redraw=true;
    }


  // WaveForm
  if (keyRepeat[SDLK_w]%64    && 
      keyRepeat[SDLK_RIGHT]%64 
      && 
      lastEvent ==  SDL_KEYDOWN
      )
    {
      if (current_op==1)
	{
	  waveform_op1=waveform_op1+1;
	  if (waveform_op1 >3) waveform_op1=0;
	  printf("[w]+[right] => waveform_op1:%d\n",waveform_op1);
	}
      if (current_op==2)
	{
	  waveform_op2=waveform_op2+1;
	  if (waveform_op2 >3) waveform_op2=0;
	  printf("[w]+[right] => waveform_op2:%d\n",waveform_op2);
	}

      redraw=true;
    }








  // AMP ENV ATTACK_AMP
  if (keyRepeat[SDLK_x]%16   && 
      keyRepeat[SDLK_DOWN]%16 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      if (current_op==1)
	{
	  attack_amp=attack_amp-1;
	  if (attack_amp <=1) attack_amp=0;
	  printf("[x]+[down] => attack_amp:%d\n",attack_amp);
	}
      if (current_op==2)
	{
	  attack_fltr=attack_fltr-1;
	  if (attack_fltr <=1) attack_fltr=0;
	  printf("[x]+[down] => attack_fltr:%d\n",attack_fltr);
	}

      redraw=true;

    }


  if (keyRepeat[SDLK_x]%16   && 
      keyRepeat[SDLK_UP]%16   
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      if (current_op==1)
	{
	  attack_amp=attack_amp+1;
	  if (attack_amp >=127) attack_amp=127;
	  printf("[x]+[up] => attack_amp:%d\n",attack_amp);
	}
      if (current_op==2)
	{
	  attack_fltr=attack_fltr+1;
	  if (attack_fltr >=127) attack_fltr=127;
	  printf("[x]+[up] => attack_fltr:%d\n",attack_fltr);
	}
      redraw=true;
    }

  // AMP ENV RELEASE_AMP
  if (keyRepeat[SDLK_x]%16    && 
      keyRepeat[SDLK_RIGHT]%16 
      )
    {
      if (current_op==1)
	{
	  release_amp=release_amp+1;
	  if (release_amp >=127) release_amp=127;
	  printf("[x]+[right] => release_amp:%d\n",release_amp);
	}
      if (current_op==2)
	{
	  release_fltr=release_fltr+1;
	  if (release_fltr >=127) release_fltr=127;
	  printf("[x]+[right] => release_amp:%d\n",release_fltr);
	}

	  redraw=true;
    }


  if (keyRepeat[SDLK_x]%16    && 
      keyRepeat[SDLK_LEFT]%16   //&& 
      )
    {
      if (current_op==1)
	{
	  release_amp=release_amp-1;
	  if (release_amp <=1) release_amp=1;
	  printf("[x]+[left] => release_amp:%d\n",release_amp);
	}
      if (current_op==2)
	{
	  release_fltr=release_fltr-1;
	  if (release_fltr <=1) release_fltr=1;
	  printf("[x]+[left] => release_fltr:%d\n",release_fltr);
	}
      redraw=true;

    }


  // AMP ENV 
  if (keyRepeat[SDLK_c]%16   && 
      keyRepeat[SDLK_DOWN]%16 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      if (current_op==1)
	{
	  decay_amp=decay_amp-1;
	  if (decay_amp <=1) decay_amp=0;
	  printf("[x]+[down] => decay_amp:%d\n",decay_amp);
	}
      if (current_op==2)
	{
	  decay_fltr=decay_fltr-1;
	  if (decay_fltr <=1) decay_fltr=0;
	  printf("[x]+[down] => decay_fltr:%d\n",decay_fltr);
	}
      redraw=true;

    }


  if (keyRepeat[SDLK_c]%16   && 
      keyRepeat[SDLK_UP]%16   
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      if (current_op==1)
	{
	  decay_amp=decay_amp+1;
	  if (decay_amp >=127) decay_amp=127;
	  printf("[x]+[up] => decay_amp:%d\n",decay_amp);
	}
      if (current_op==2)
	{
	  decay_fltr=decay_fltr+1;
	  if (decay_fltr >=127) decay_fltr=127;
	  printf("[x]+[up] => decay_fltr:%d\n",decay_fltr);
	}

      redraw=true;

    }

  // AMP ENV 
  if (keyRepeat[SDLK_c]%16    && 
      keyRepeat[SDLK_RIGHT]%16 
      )
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
    {
      if (current_op==1)
	{
	  sustain_amp=sustain_amp+1;
	  if (sustain_amp >=127) sustain_amp=127;
	  printf("[x]+[right] => sustain_amp:%d\n",sustain_amp);
	}
      if (current_op==2)
	{
	  sustain_fltr=sustain_fltr+1;
	  if (sustain_fltr >=127) sustain_fltr=127;
	  printf("[x]+[right] => sustain_fltr:%d\n",sustain_fltr);
	}

      redraw=true;

    }


  if (keyRepeat[SDLK_c]%16    && 
      keyRepeat[SDLK_LEFT]%16   //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      if (current_op==1)
	{
	  sustain_amp=sustain_amp-1;
	  if (sustain_amp <=1) sustain_amp=1;
	  printf("[x]+[left] => sustain_amp:%d\n",sustain_amp);
	}
      if (current_op==2)
	{
	  sustain_fltr=sustain_fltr-1;
	  if (sustain_fltr <=1) sustain_fltr=1;
	  printf("[x]+[left] => sustain_fltr:%d\n",sustain_fltr);
	}

      redraw=true;

    }





  // VCO MIX
  if (keyRepeat[SDLK_b]%64    && 
      keyRepeat[SDLK_DOWN]%64 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      vcomix=vcomix-1;
      if (vcomix <=1) vcomix=1;
      redraw=true;
      printf("[v]+[down] => vcomix|FreqMultOp1:%d\n",vcomix);
    }


  if (keyRepeat[SDLK_b]%64    && 
      keyRepeat[SDLK_UP]%64   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      vcomix=vcomix+1;
      if (vcomix >=127) vcomix=127;
      redraw=true;
      printf("[v]+[up] => vcomix|FreqMultOp1:%d\n",vcomix);
    }


  if (keyRepeat[SDLK_b]%64    && 
      keyRepeat[SDLK_LEFT]%64 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      phase=phase-1;
      if (phase <=1) phase=1;
      redraw=true;
      printf("[v]+[left] => phase|FreqMultOp2:%d\n",phase);
    }


  if (keyRepeat[SDLK_b]%64    && 
      keyRepeat[SDLK_RIGHT]%64   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      phase=phase+1;
      if (phase >=127) phase=127;
      redraw=true;
      printf("[v]+[right] => phase|FreqMultOp2:%d\n",phase);
    }










  // FILTER Cutoff/Resonance

  if (keyRepeat[SDLK_v]%16    && 
      keyRepeat[SDLK_DOWN]%16 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      cutoff=cutoff-1;
      if (cutoff <=1) cutoff=1;
      redraw=true;
      printf("[c]+[down] => cutoff:%d\n",cutoff);
    }


  if (keyRepeat[SDLK_v]%16    && 
      keyRepeat[SDLK_UP]%16   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      cutoff=cutoff+1;
      if (cutoff >=126) cutoff=126;
      redraw=true;
      printf("[c]+[up] => cutoff:%d\n",cutoff);
    }


  if (keyRepeat[SDLK_v]%64    && 
      keyRepeat[SDLK_RIGHT]%64 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      resonance=resonance+1;
      if (resonance >=127) resonance=127;
      redraw=true;
      printf("[c]+[right] => resonance:%d\n",resonance);
    }


  if (keyRepeat[SDLK_v]%64    && 
      keyRepeat[SDLK_LEFT]%64   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      resonance=resonance-1;
      if (resonance <=1) resonance=1;
      redraw=true;
      printf("[c]+[left] => resonance:%d\n",resonance);
    }




  //
  // osc1_detune osc2_detune
  //
  if (keyRepeat[SDLK_n]%16    && 
      keyRepeat[SDLK_DOWN]%16 
      )
    {
      osc1_detune=osc1_detune-1;
      if (osc1_detune <=0) osc1_detune=0;
      redraw=true;
      printf("[n]+[down] => osc1_detune:%d\n",osc1_detune);
    }


  if (keyRepeat[SDLK_n]%16    && 
      keyRepeat[SDLK_UP]%16   
      )
    {
      osc1_detune=osc1_detune+1;
      if (osc1_detune >=126) osc1_detune=126;
      redraw=true;
      printf("[n]+[up] => osc1_detune:%d\n",osc1_detune);
    }


  if (keyRepeat[SDLK_n]%64    && 
      keyRepeat[SDLK_RIGHT]%64 
      )
    {
      osc2_detune=osc2_detune+1;
      if (osc2_detune >=127) osc2_detune=127;
      redraw=true;
      printf("[n]+[right] => osc2_detune:%d\n",osc2_detune);
    }


  if (keyRepeat[SDLK_n]%64    && 
      keyRepeat[SDLK_LEFT]%64   
      )
    {
      osc2_detune=osc2_detune-1;
      if (osc2_detune <=0) osc2_detune=1;
      redraw=true;
      printf("[n]+[left] => osc2_detune:%d\n",osc2_detune);
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

  //  if (lastKey==SDLK_w) { attack--; printf("attack %d\n",attack); }
  //  if (lastKey==SDLK_x) { attack++; printf("attack %d\n",attack); }

  //  if (lastKey==SDLK_c) { release--; printf("release %d\n",release); }
  //  if (lastKey==SDLK_v) { release++; printf("release %d\n",release); }

  //  if (lastKey==SDLK_b) { vcomix--; printf("vco %d\n",vcomix); if (vcomix<=0  ) vcomix=0;   }
  //  if (lastKey==SDLK_n) { vcomix++; printf("vco %d\n",vcomix); if (vcomix>=127) vcomix=127; }


  if (lastEvent ==  SDL_KEYDOWN)
    {
      if (lastKey==SDLK_q) { printf("key q\n"); noteValue=0+octave;  noteon=1; }
      if (lastKey==SDLK_z) { printf("key z\n"); noteValue=1+octave;  noteon=1; }
      if (lastKey==SDLK_s) { printf("key s\n"); noteValue=2+octave;  noteon=1; }
      if (lastKey==SDLK_e) { printf("key e\n"); noteValue=3+octave;  noteon=1; }
      if (lastKey==SDLK_d) { printf("key d\n"); noteValue=4+octave;  noteon=1; }
      if (lastKey==SDLK_f) { printf("key f\n"); noteValue=5+octave;  noteon=1; }
      if (lastKey==SDLK_t) { printf("key t\n"); noteValue=6+octave;  noteon=1; }
      if (lastKey==SDLK_g) { printf("key g\n"); noteValue=7+octave;  noteon=1; }
      if (lastKey==SDLK_y) { printf("key y\n"); noteValue=8+octave;  noteon=1; }
      if (lastKey==SDLK_h) { printf("key h\n"); noteValue=9+octave;  noteon=1; }
      if (lastKey==SDLK_u) { printf("key u\n"); noteValue=10+octave; noteon=1; }
      if (lastKey==SDLK_j) { printf("key j\n"); noteValue=11+octave; noteon=1; }
      
      if (lastKey==SDLK_k) { printf("key k\n"); noteValue=12+octave; noteon=1; }
      if (lastKey==SDLK_o) { printf("key o\n"); noteValue=13+octave; noteon=1; }
      if (lastKey==SDLK_l) { printf("key l\n"); noteValue=14+octave; noteon=1; }
      if (lastKey==SDLK_p) { printf("key p\n"); noteValue=15+octave; noteon=1; }
      if (lastKey==SDLK_m) { printf("key m\n"); noteValue=16+octave; noteon=1; }
    }

  if (lastEvent ==  SDL_KEYUP)
    {
      if (lastKey==SDLK_q) { printf("key q\n"); noteValue=0+octave;  noteon=0; }
      if (lastKey==SDLK_z) { printf("key z\n"); noteValue=1+octave;  noteon=0; }
      if (lastKey==SDLK_s) { printf("key s\n"); noteValue=2+octave;  noteon=0; }
      if (lastKey==SDLK_e) { printf("key e\n"); noteValue=3+octave;  noteon=0; }
      if (lastKey==SDLK_d) { printf("key d\n"); noteValue=4+octave;  noteon=0; }
      if (lastKey==SDLK_f) { printf("key f\n"); noteValue=5+octave;  noteon=0; }
      if (lastKey==SDLK_t) { printf("key t\n"); noteValue=6+octave;  noteon=0; }
      if (lastKey==SDLK_g) { printf("key g\n"); noteValue=7+octave;  noteon=0; }
      if (lastKey==SDLK_y) { printf("key y\n"); noteValue=8+octave;  noteon=0; }
      if (lastKey==SDLK_h) { printf("key h\n"); noteValue=9+octave;  noteon=0; }
      if (lastKey==SDLK_u) { printf("key u\n"); noteValue=10+octave; noteon=0; }
      if (lastKey==SDLK_j) { printf("key j\n"); noteValue=11+octave; noteon=0; }
     
      if (lastKey==SDLK_k) { printf("key k\n"); noteValue=12+octave; noteon=0; }
      if (lastKey==SDLK_o) { printf("key o\n"); noteValue=13+octave; noteon=0; }
      if (lastKey==SDLK_l) { printf("key l\n"); noteValue=14+octave; noteon=0; }
      if (lastKey==SDLK_p) { printf("key p\n"); noteValue=15+octave; noteon=0; }
      if (lastKey==SDLK_m) { printf("key m\n"); noteValue=16+octave; noteon=0; }
    }
  IE.clearLastKeyEvent(); 

  //  printf("%%%%%%%%%%%%%%%%%%%%%%%% noteon:%d\n",noteon);
  
  //noteon=0;

  if (noteon==1 &&
    //M[t]->getADSRAmp().getNoteOn()==0)
      M[t]->getI(NOTE_ON)==0)
    {
      printf("Trig note on\n");

      for (t=0;t<1;t++)
	{
	  //float   f=PE.getNoteFreq();
	  //int     i=f;
	  //float   f_c;
	  //float   f_r;

	  //f_c=cutoff;
	  //f_r=resonance;


	  //f_c=f_c/256;
	  //f_r=f_r/8;
	  
	  //printf("[Freq:%f]\n",f);

	  MM[t]->setAmplitude(64);
	  MM[t]->setMachineType(current_machine);
	  //M[t]  = MM[t]->getInput();                             
	  //FX[t] = MM[t]->getEffect();                             

	  M[t]  = MM[t]->getInput();                             
	  FX[t] = MM[t]->getEffect();                             

	  FX[t]->setDepth(0);
	  FX[t]->setSpeed(0);

	  //MM[t]->setMachineType(1);
	  //MM[t]->setAmplitude(64);
	  //M[t]->setF(OSC1_FREQ,f);

	  M[t]->reset();
	  M[t]->setI(NOTE1,noteValue);

	  M[t]->setI(OSC1_TYPE,2);
	  M[t]->setI(OSC2_TYPE,2);
	
	  M[t]->setI(OSC12_MIX,vcomix);
	  M[t]->setI(OSC1_PHASE,phase);

	  M[t]->setI(OSC1_TYPE,waveform_op1);
	  M[t]->setI(OSC2_TYPE,waveform_op2);

	  M[t]->setI(OSC1_DETUNE, osc1_detune);
	  M[t]->setI(OSC2_DETUNE, osc2_detune);


	  M[t]->setI(ADSR_ENV0_ATTACK,attack_amp);
	  M[t]->setI(ADSR_ENV0_DECAY,decay_amp);
	  M[t]->setI(ADSR_ENV0_SUSTAIN,sustain_amp);
	  M[t]->setI(ADSR_ENV0_RELEASE,release_amp);

	  M[t]->setI(ADSR_ENV1_ATTACK,attack_fltr);
	  M[t]->setI(ADSR_ENV1_ATTACK,decay_fltr);
	  M[t]->setI(ADSR_ENV1_ATTACK,sustain_fltr);
	  M[t]->setI(ADSR_ENV1_RELEASE,release_fltr);

	  M[t]->setI(LFO1_DEPTH,     0);
	  M[t]->setI(LFO1_ENV_AMOUNT,0);
	  M[t]->setF(LFO1_FREQ,      0);

	  M[t]->setI(LFO2_DEPTH,     0);
	  M[t]->setI(LFO2_ENV_AMOUNT,0);
	  M[t]->setF(LFO2_FREQ,      0);


	  M[t]->setI(FILTER1_CUTOFF,cutoff);
	  M[t]->setI(FILTER1_RESONANCE,resonance);

	  M[t]->setI(NOTE_ON,1);


	}
    }

  if (noteon==0)      
      
      //&& 
      //M[t]->getADSR().getNoteOn()==1)
      //M[t]->set(NOTE_ON,0);
    {
      //exit(0);
      for (t=0;t<1;t++)
	{
	  if (M[t]->getI(NOTE_ON)==1)
	    {
	      printf("Trig note off\n");
	      M[t]->setI(NOTE_ON,0);
	    }
	}
    }

  
  //printf("key pressed sleeping 10ms\n");
  redraw=true;
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

  file_buffer=AE.getBufferOutLeft();

  for (int i = 0; i < SCREEN_WIDTH-1 ; i++)
    {
      last_j=j;
      j=offset+i*zoom;
      //      if (j<=filesize_octet*2)
      if (j<=DEFAULTSAMPLES)
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

#ifdef __SDL20__
void draw_screen()
{
  SDL_Rect rect;
  short color;
  //return;
  // rect.x rect.y rect.w rect.h
  // int x the x location of the rectangle's upper left corner
  // int y the y location of the rectangle's upper left corner
  // int w the width of the rectangle
  // int h the height of the rectangle

  //Blank screen
  //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));               

  screen=SG.screen;
  
  SDL_FillRect(screen,NULL, 0x000000);

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
  
  //SDL_Flip(screen);
  SG.refresh();
  SDL_Delay(1);
  redraw=false;  

}
#endif

#ifdef __SDL12__
void draw_screen()
{
  SDL_Rect rect;
  short color;
//return;
  // rect.x rect.y rect.w rect.h
  // int x the x location of the rectangle's upper left corner
  // int y the y location of the rectangle's upper left corner
  // int w the width of the rectangle
  // int h the height of the rectangle

  //Blank screen
  //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));               

if (screen==NULL)
  screen=SDL_GetVideoSurface();
  
  SDL_FillRect(screen,NULL, 0x000000);

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
#endif



void wtg()
{
  
  Generator G;
  WaveTableManager & WTM = WaveTableManager::getInstance();
  WaveTable* WT;


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

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothSine();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMSINE);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothSaw();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMSAW);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothPulse();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMPULSE);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.smoothTriangle();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_SMTRGL);



  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.noise();
  //memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_NOISE);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.LFSRNoise();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE*DEFAULTBITRATE/8);
  WTM.insert(WT,PICO_WAVETABLE_LFSRNOISE);

  

  
  // WT = new WaveTable();
  // WT->setSize(WAVETABLE_SIZE);
  // G.noise();
  // memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  // WTM.insert(WT,PICO_WAVETABLE_NOISE);
  
  printf("wavetablemanager.getSize : %d\n",WTM.getSize());


}


int main(int argc,char ** argv)
{	  
  //if (argc!=2) { printf("arg please\n"); exit(1); }

  wtg();
  NoteFreq & NF = NoteFreq::getInstance();
  NF.init();


  file_buffer=(Sint16*)malloc(sizeof(short)*DEFAULTSAMPLES);
  vector_buffer=(Sint16*)malloc(sizeof(Sint16)*SCREEN_WIDTH);
  

  //PE.setNote(32);

  struct stat fileStat;
  //  if(stat(argv[1],&fileStat) < 0)    
  //return 1;

  //filename=argv[1];
  //load_raw(argv[1]);

  openaudio();
  //init_video();
  IE.init();
  IE.printState();
  SG.initVideo();
  
  /* Main loop */
  quit = 0;

  while(!quit)
    {
      prepare_vector_buffer();
      draw_screen();
      handle_key();
      //quit=1;
    }  
  DPRINTF("[closeVideo output]");
  //SG.closeVideo();
  
  return 0;
}

