using namespace std;
#include "Oscillator.h"
//#include "NoteFreq.h"
//#include "WaveTableManager.h"

Oscillator::Oscillator()
{
  frequency=0;
  freq=0;
  amplitude=0;
  sample_num=0;
  offset_next_index=0;
  table_size=WAVETABLE_SIZE;
  index=0;
  ph=0;
  table=NULL;
  note=0;
  detune=0;
  DPRINTF("Oscillator::Oscillator() %d",index);
}

Oscillator::~Oscillator()
{
  DPRINTF("Oscillator::~Oscillator() %d",index);
}

void Oscillator::init()
{
  frequency=0;
  freq=0;
  amplitude=0;
  sample_num=0;
  offset_next_index=0;
  table_size=WAVETABLE_SIZE;
  index=0;
  ph=0;
  table=NULL;
  note=0;
  detune=0;

}

void Oscillator::reset()
{
  index=0;
  offset_next_index=0;
  ph=0;
}

// phase is from 0 to 127
void Oscillator::setPhase(int phase)
{
  int shift=8;
  int coeff=table_size/128;
  index=(coeff*phase)<<shift;
  ph=phase;
}



int Oscillator::getFreq()
{
  return frequency;
}

void Oscillator::setNoteDetune(int nt,int dt)
{
  note=nt;
  detune=dt;
  NoteFreq & NF = NoteFreq::getInstance();
  offset_next_index=NF.getWTJumpDetune(note,detune);
  //DPRINTF("note:%d detune:%d offset_next_index:%d\n",note,detune,offset_next_index);
}


void Oscillator::setNoteDetune(int ntdt)
{
  notedetune=ntdt;
  NoteFreq & NF = NoteFreq::getInstance();
  offset_next_index=NF.getWTJumpDetune(notedetune);
  //DPRINTF("note:%d detune:%d offset_next_index:%d\n",note,detune,offset_next_index);
}


void Oscillator::setFreq(int frq)
{
  Sint32 tmp;
  int    shift=8;
  int    wtshift=1<<shift;

  freq=frq;
  //return;

  if (freq==frequency)
    return;

  if (freq <= 0)          
    {
      freq=1;
      offset_next_index=(table_size)/DEFAULTFREQ;
    }
  else 
    if (freq > 20000) freq=20000;
  frequency=freq;

  if (freq>0)
    {
      tmp=(WAVETABLE_SIZE*wtshift/DEFAULTFREQ)*freq;
      offset_next_index=tmp;
    }
}



void Oscillator::setAmplitude(int amp)
{
  int debug=1;
  if (amp>=127          ){ amplitude=127; }
  if (amp<=0            ){ amplitude=0;   }
  if (amp>0   && amp<127){ amplitude=amp; }
  DPRINTF("Oscillator::setAmplitude(amplitude=%d)",amplitude);
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
  const int   shift=8;
  const int   wtshift=1<<shift;
  assert(index>=0);
  index=index+offset_next_index;
  //printf("%d %d \n",index,offset_next_index);
  /*
  // Need to fix this one day
  // The index at start could go outside of the table[XXX], so I need a way to prevent it
  if (index<0) index=0;
  */
  //if (index<0) index=0;
  if ((index>>shift)>=table_size)
    {
      index=index-(table_size<<shift);
      // seem to be the case at picoloop at start
      // so I bugfix it here, not the right way it's not the root cause
      // if ((index>>shift)>=table_size)
      // 	index=0;
    }
  return table[index>>shift];
}

