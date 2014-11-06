using namespace std;
#include "Oscillator.h"
//#include "NoteFreq.h"
//#include "WaveTableManager.h"

Oscillator::Oscillator()
{
  frequency=0;
  amplitude=0;
  sample_num=0;
  sample_num_index=0;
  last_tick=0;
  table_size=0;
  offset_next_index=0;
  table_size=WAVETABLE_SIZE;
  index=0;
}

Oscillator::~Oscillator()
{
  printf("Oscillator::~Oscillator()\n");
}

void Oscillator::reset()
{
  if (sample_num>0)
    {
      sample_num=0;
    }
  index=0;
}

// phase is from 0 to 127
void Oscillator::setPhase(int phase)
{
  int coeff=table_size/128;
  index=coeff*phase;
}



int Oscillator::getFreq()
{
  return frequency;
}

void Oscillator::setNoteDetune(int note,int detune)
{
  NoteFreq & NF = NoteFreq::getInstance();
  offset_next_index=NF.getWTJumpDetune(note,detune);
  //printf("note:%d detune:%d offset_next_index:%d\n",note,detune,offset_next_index);
}

void Oscillator::setFreq(int freq)
{
  //return;

  if (freq==frequency)
    return;

  if (freq <= 0)          
    {
      freq=0;
      offset_next_index=(table_size)/44100;
    }
  else 
    if (freq > 20000) freq=20000;
  frequency=freq;

  if (freq>0)
    {
      offset_next_index=(freq*table_size*65535)/44100;
    }
}



void Oscillator::setAmplitude(int amp)
{
  int debug=1;
  if (amp>=127          ){ amplitude=127; }
  if (amp<=0            ){ amplitude=0;   }
  if (amp>0   && amp<127){ amplitude=amp; }
  printf("Oscillator::setAmplitude(amplitude=%d)\n",amplitude);
}


void   Oscillator::setWaveForm(int waveform)
{
  WaveTable * WT;
  WaveTableManager & WTM = WaveTableManager::getInstance();
  WT=WTM.get(waveform);
  table=WT->getBuffer();
}


Sint16 Oscillator::tick()
{ 
  index=index+offset_next_index;
  if (index>=table_size)
    {
      index=index-(table_size>>16);
      if ((index>>16)>table_size)
	index=0;
      //printf("offset_next_index.tick.offset_next_index:%d\n",offset_next_index);
    }
  return table[index>>16];
}

