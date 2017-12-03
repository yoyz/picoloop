

#ifndef __AUDIOENGINE____
#define __AUDIOENGINE____
#include "Master.h"

#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              

//#include <SDL/SDL.h>
#include <iostream>
#include <map>
#include "RtAudio.h"

#include "Oscillator.h"
#include "AudioMixer.h"
#include "WaveFileWriter.h"

#ifdef   __SDL_AUDIO__
#include "AudioDriverSDL.h"
#endif

#ifdef   __PSVITA_AUDIO__
#include "AudioDriverPSVita.h"
#endif


#ifdef   __RTAUDIO__
#include "AudioDriverRTAudio.h"
#endif



void processBuffer_updateMidiSendClockCounter();
extern int counter_send_midi_clock;     // send n clock and decrement the counter each time
extern int counter_send_midi_clock_six; // send n clock and decrement the counter each time
extern int counter_recv_midi_clock;     // send n clock and decrement the counter each time
extern int counter_recv_midi_clock_six; // send n clock and decrement the counter each time
extern int counter_delta_midi_clock;    // delta in sample between picoloop(master) and midi slave
extern int   menu_config_midiClockMode;  // 0 internal, 1 midi sync out, 2 midi sync in 
extern float bpm_current;
                                    // define in PatternPlayer.cpp

/*
  Purpose : 
  mix all channel
  implement the main callback which produce the stream 
 */

//#define BUFFER_FRAME 8192
//#define BUFFER_FRAME 512




class PulseSync
{
public:
  PulseSync();

  int setNbTickBeforeStepChange(int val);
  int tick();

  int nb_tick_before_step_change;
  int nb_tick;
  int nb_tick_before_step_change_real;
  int tick_length_high;
  int tick_length_low;
  int tick_height_high;
  int tick_height_low;
  int tick_height_std;
  int step;

  int out;
  int last_out;
  int stepdec1;
  int stepdec2;


};


class AudioEngine
{
 public:
  AudioEngine();
  void setDefault();
  void setEngineFreq(int frequency);

  //int probeDevice();

  int  startAudio();
  int  startAudioSdl();
  int  stopAudio();
  int  stopAudioSdl();
  int  openAudio();
  int  openAudioSdl();
  int  closeAudio();
  int  closeDumpAudioFile();
  int  closeAudioSdl();
  int  getNumberOfAudioOutputDevice();
  void setAudioOutput(int deviceNumber);
  char *  getAudioOutputName(int deviceNumber);
  //void setTick(int t);
  int  getTickLeft();
  int  getTickRight();
  AudioMixer   * getAudioMixer();
  void setAudioMixer(AudioMixer * aumix);
  //void callback();
  //  void sdl_callback();
  //void  sdl_audio_callback(void *user_data, Uint8 *audio, int length);
  void  callback(void *user_data, Uint8 *audio, int length);
  /*
  int callback(void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *data );
  */
  
  //void sdlcallback(void *unused, Uint8 *stream, int len);

  //void set_instrument(Instrument inst);
  int setNbTickBeforeStepChange(int val);
  //void setSynthFreq(int sfreq);
  int  getNbCallback();

  int bufferIsGenerated();
  void processBuffer(int len);
  Sint16 * getBufferOutLeft();
  Sint16 * getBufferOutRight();

  void setupSequencerCallback(void (*ptrfunc)(void));

 private:
  int          freq;
  int          samples;
  int          channels;
  int          polyphony;

  int          defaultFreq;
  int          defaultSamples;
  int          defaultChannels;
  int          defaultPolyphony;
  unsigned int bufferFrames;

  int          FORMAT;
  int          tick_left;
  int          tick_right;
  int          nb_tick;
  int          nb_tick_before_step_change;
  AudioMixer * AM;
  int          nbCallback;
  FILE       * fd;
  int          dump_audio;
  Sint16     * buffer_out_left;
  Sint16     * buffer_out_right;
  int          bufferGenerated;

  void       (*seqCallback)(void);
  AudioDriver  AD;
  PulseSync    PS;

  int          callback_called;
  Sint32       fwrite_byte_counter;

  char      *  dump_audio_env;
  WaveFileWriter WFW;          
};

#endif
