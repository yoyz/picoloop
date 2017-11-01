#include "Master.h"
#include "AudioEngine.h"

AudioEngine    AE;          // used to  init alsa/rtaudio


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

void processBuffer_updateMidiSendClockCounter() {}
/* end external dependency not used in waveSynth */


void donothing() {}

void openaudio()
{
  AE.setupSequencerCallback(donothing);
  AE.setNbTickBeforeStepChange(1000);
  AE.setAudioOutput(0);
  AE.openAudio();
  printf("openAudio start streaming\n");
  AE.startAudio();
}

void closeaudio()
{
  AE.stopAudio();
}

int main(int argc,char ** argv)
{	  
  openaudio();
}
