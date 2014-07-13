using namespace std;

#ifndef __AUDIOENGINE____
#define __AUDIOENGINE____
#include "Master.h"
//#include "Instrument.h"
//#include "Synth.h"
//#include "Wave.h"
#include <SDL/SDL.h>
#include <iostream>
#include <map>
#include "RtAudio.h"

#include "Oscillator.h"
#include "SineOscillator.h"
#include "AudioMixer.h"

#ifdef   __SDL_AUDIO__
#include "AudioDriverSDL.h"
#endif

#ifdef   __RTAUDIO__
#include "AudioDriverRTAudio.h"
#endif


/*
  Purpose : 
  mix all channel
  implement the main callback which produce the stream 
 */

//#define BUFFER_FRAME 8192
//#define BUFFER_FRAME 512

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
  int  closeAudioSdl();
  void setTick(int t);
  int  getTick();
  AudioMixer   & getAudioMixer();
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
  void processBuffer();
  Sint16 * getBufferOut();

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
  int          tick;
  int          nb_tick;
  int          nb_tick_before_step_change;
  //Instrument   inst;
  //  SineOscillator S;
  AudioMixer   AM;
  int          nbCallback;
  FILE       * fd;
  int          debug_audio;
  Sint16     * buffer_out;
  //RtAudio dac;
  int          bufferGenerated;

  void       (*seqCallback)(void);
  //RtAudio::StreamParameters rtAudioOutputParams;
  //RtAudio::StreamOptions    rtAudioStreamOptions;
  AudioDriver  AD;
  //SDL_AudioSpec * sdlAudioSpecWanted;
  //  SDL_AudioSpec * sdlAudioSpecObtained;
};

#endif
