using namespace std;

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Master.h"
//#include "AudioEngine.h"
#include "AudioMixer.h"
#include "MonoMixer.h" 
//#include "InputManager.h"
#include "SDL_GUI.h"

#include "Generator.h"
#include "WaveTableManager.h"
#include "WaveTable.h"
//#include "PatternElement.h"

#include "NoteFreq.h"

//SDL_GUI        SG;          // used to  initialize video
//AudioEngine    AE;          // used to  init alsa/rtaudio
//InputManager   IE;          // used to  fetch key
AudioMixer       AM;

vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);
vector <Effect    *>        FX(TRACK_MAX);

/* begin external dependency not used in waveSynth */
int   menu_config_bank=0;           // will choose the bank to load at startup
int   menu_config_audioOutput=0;    // audioOutputNumber define in the config menu
int   menu_config_midiOutput=0;     //  midiOutputNumber define in the config menu
int   menu_config_midiInput=0;      //  midiInputNumber  define in the config menu
int   menu_config_midiClockMode=0;  // 0 internal, 1 midi sync out, 2 midi sync in 
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
int    t=0;
//PatternElement PE;

void processBuffer_updateMidiSendClockCounter() {}
void donothing() {}

void openaudio()
{
  int t;
  //AE.setupSequencerCallback(donothing);
  //AE.setNbTickBeforeStepChange(1000);
  //AE.setAudioOutput(0);
  //AE.openAudio();
  for (t=0;t<TRACK_MAX;t++)
    {
      AM.setAudioVolume(128);
      MM[t]=AM.getMonoMixer(t);      
      //AE.getAudioMixer().setAudioVolume(128);
      //MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer();
      //MM[t]=AE.getAudioMixer().getMonoMixer(t);      
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
  //AE.startAudio();

  int i=0;
  for (i=0;i<10;i++)
    printf("[%d]\n",M[0]->tick());

}



void trigMachine()
{
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
//redraw=true;
//SDL_Delay(10);
}



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
  int nbcycle=0;
  Sint16 sample;
  int i;
  wtg();
  NoteFreq & NF = NoteFreq::getInstance();
  NF.init();
  openaudio();

  for (t=0;t<TRACK_MAX;t++)
    {
      MM[t]->setMachineType(3);
    }
  while(nbcycle<(60*60))
    {
      trigMachine();
      nbcycle++;
      for (i=0;i<44100;i++)
	sample=AM.tick();
    }  
  
  return 0;
}

