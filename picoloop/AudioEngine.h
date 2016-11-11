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
//#include "SineOscillator.h"
#include "AudioMixer.h"

#ifdef   __SDL_AUDIO__
#include "AudioDriverSDL.h"
#endif

#ifdef   __RTAUDIO__
#include "AudioDriverRTAudio.h"
#endif



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


typedef struct WAVEHEADER_AUDIOENGINE
{  
  char   riff[4];        // (4 bytes): FileTypeBlocID  «RIFF»  (0x52,0x49,0x46,0x46)
  Sint32 riffLength;     // (4 bytes): FileSize        file size minus 8 bytes
  char   wave[4];        // (4 bytes): FileFormatID    Format = «WAVE»  (0x57,0x41,0x56,0x45)
  
  char   fmt[4];         // (4 bytes): FormatBlocID    «fmt »  id (0x66,0x6D, 0x74,0x20)
  Sint32 waveLength;     // (4 bytes): Length of format data.  Always 16
  Sint16 wFormatTag;     // (2 bytes): storage format : 1==PCM
  Sint16 nChannels;      // (2 bytes): 1==mono, 2==stereo
  Sint32 frequency;      // (4 bytes): Frequency: 44 100 Hz Sample Rate (CD-Quality)
  Sint32 bytePerSec;     // (4 bytes): Frequency(44100) * bytePerBloc
  Sint16 bytePerBloc;    // (2 bytes): nChannels * (BitsPerSample/8)
  Sint16 wBitsPerSample; // (2 bytes): bit used per sample (8, 16, 24)
  char   data[4];        // (4 bytes): «data»  (0x64,0x61,0x74,0x61)
  Sint32 dataLength;     // (4 bytes): fileSize - sizeof(WAVEHEADER)
} WHAE;



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
  int          nb_tick_before_midi_send_clock;     // number of audio sample between each midi sync clock
  int          nb_tick_before_six_midi_send_clock; // number of audio sample between six  midi sync clock
  int          nb_tick_midi_send_clock;            // counter between each     clock increment by audio sample
  int          nb_tick_midi_send_clock_mulsix;     // counter between each six clock increment by audio sample
  int          midi_tick_number;
  //Instrument   inst;
  //  SineOscillator S;
  AudioMixer   AM;
  int          nbCallback;
  FILE       * fd;
  int          dump_audio;
  Sint16     * buffer_out_left;
  Sint16     * buffer_out_right;
  //RtAudio dac;
  int          bufferGenerated;

  void       (*seqCallback)(void);
  //RtAudio::StreamParameters rtAudioOutputParams;
  //RtAudio::StreamOptions    rtAudioStreamOptions;
  AudioDriver  AD;
  PulseSync    PS;

  int          callback_called;
  //SDL_AudioSpec * sdlAudioSpecWanted;
  //  SDL_AudioSpec * sdlAudioSpecObtained;

  WHAE         waveHeader;
  Sint32       fwrite_byte_counter;
};

#endif
