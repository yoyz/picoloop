using namespace std;

#include <string>
#include "Wave.h"
#include "Synth.h"

#ifndef __INSTRUMENT__
#define __INSTRUMENT__


class Instrument
{
public:
  Instrument();
  void setWave();
  void setSynth();
  void assignWave(Wave waveInstrument);
  void assignSynth(Synth synthInstrument);
  
  void setPlaying();
  void unsetPlaying();
  int getPlaying();
  
  void generateSamples(Uint8 *audio, int length);

 private:
  int wave;
  int synth;
  Wave w;
  Synth s;
  int playing;
};
  
#endif
