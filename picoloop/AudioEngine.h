using namespace std;

#ifndef __AUDIOENGINE____
#define __AUDIOENGINE____
#include "Master.h"
#include "Instrument.h"
#include "Synth.h"
#include "Wave.h"
#include <SDL/SDL.h>
#include <iostream>
#include <map>
#include "RtAudio.h"

#include "Oscillator.h"
#include "SineOscillator.h"
#include "AudioMixer.h"
/*
  Purpose : 
  mix all channel
  implement the main callback which produce the stream 
 */

//#define BUFFER_FRAME 8192
//#define BUFFER_FRAME 512
#define BUFFER_FRAME DEFAULTSAMPLES
#define DEFAULT_FREQ 44100

class AudioEngine
{
 public:
  AudioEngine();
  void setDefault();
  void setEngineFreq(int frequency);

  int probeDevice();

  int  startAudio();
  int  openAudio();
  int  closeAudio();
  void setTick(int t);
  int  getTick();
  AudioMixer   & getAudioMixer();
  //void callback();
  //  void sdl_callback();
  //void  sdl_audio_callback(void *user_data, Uint8 *audio, int length);
  int callback(void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *data );
  void set_instrument(Instrument inst);
  void setSynthFreq(int sfreq);
  int  getNbCallback();
  
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
  Instrument   inst;
  //  SineOscillator S;
  AudioMixer   AM;
  int          nbCallback;
  FILE       * fd;
  int          debug_audio;

  RtAudio dac;
  RtAudio::StreamParameters rtAudioOutputParams;
  RtAudio::StreamOptions    rtAudioStreamOptions;

};

#endif
