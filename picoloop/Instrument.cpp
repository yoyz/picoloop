using namespace std;

#include "Instrument.h"

Instrument::Instrument()
{
  wave=1;
  synth=0;
  playing=0;
}

void Instrument::setWave()
{
  wave=1;
  synth=0;
}

void Instrument::setSynth()
{
  wave=0;
  synth=1;
}


void Instrument::assignWave(Wave waveInstrument)
{
  w=waveInstrument;
}

void Instrument::assignSynth(Synth synthInstrument)
{
  s=synthInstrument;
}

void Instrument::setPlaying()
{
  playing=1;
}

void Instrument::unsetPlaying()
{
  playing=0;
}

int Instrument::getPlaying()
{
  return playing;
}

void Instrument::generateSamples(Uint8 *audio, int length)
{
  printf("Instrument::generateSamples\n");
  //s.generateSamples(audio,length);
  //w.generateSamples(audio,length);
  //w.generateSamples(audio,length);
}
