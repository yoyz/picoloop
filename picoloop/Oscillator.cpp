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
  ph=0;
  DPRINTF("Oscillator::Oscillator() %d",index);
}

Oscillator::~Oscillator()
{
  DPRINTF("Oscillator::~Oscillator() %d",index);
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
  int shift=8;
  int coeff=table_size/128;
  index=(coeff*phase)<<shift;
  ph=phase;
}



int Oscillator::getFreq()
{
  return frequency;
}

void Oscillator::setNoteDetune(int note,int detune)
{
  NoteFreq & NF = NoteFreq::getInstance();
  offset_next_index=NF.getWTJumpDetune(note,detune);
  //DPRINTF("note:%d detune:%d offset_next_index:%d\n",note,detune,offset_next_index);
}


void Oscillator::setNoteDetune(int notedetune)
{
  NoteFreq & NF = NoteFreq::getInstance();
  offset_next_index=NF.getWTJumpDetune(notedetune);
  //DPRINTF("note:%d detune:%d offset_next_index:%d\n",note,detune,offset_next_index);
}


void Oscillator::setFreq(int freq)
{
  Sint32 tmp;
  int    shift=8;
  int    wtshift=1<<shift;

  //return;

  if (freq==frequency)
    return;

  if (freq <= 0)          
    {
      freq=1;
      offset_next_index=(table_size)/44100;
    }
  else 
    if (freq > 20000) freq=20000;
  frequency=freq;

  if (freq>0)
    {
      //tmp=freq*table_size*65535;
      //tmp=((freq*(wtshift))/44100)*(16384);
      //tmp=(freq*((wtshift))*WAVETABLE_SIZE)/(44100);
      //tmp=(freq*((wtshift))*WAVETABLE_SIZE)/(44100);
      //tmp=((freq*WAVETABLE_SIZE)/44100)*wtshift;
      tmp=(WAVETABLE_SIZE*wtshift/44100)*freq;
      //tmp=tmp/44100;
      //offset_next_index=(freq*table_size*65535)/44100;
      offset_next_index=tmp;
      // offset_next_index=tmp+ph*wtshift;
      // if ((offset_next_index>>shift)>=table_size)
      // 	offset_next_index=offset_next_index-(table_size<<shift);


      //DPRINTF("freq:%d table_size:%d offset_next_index:%d tmp:%d\n",freq,table_size,offset_next_index,tmp);
      //printf("freq:%d table_size:%d offset_next_index:%d tmp:%d\n",freq,table_size,offset_next_index,tmp);
    }

  //DPRINTF("offset_next_index:%d\n",offset_next_index);
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

  index=index+offset_next_index;
  /*
  // Need to fix this one day
  // The index at start could go outside of the table[XXX], so I need a way to prevent it
  if (index<0) index=0;
  */
  if (index<0) index=0;
  if ((index>>shift)>=table_size)
    {
      index=index-(table_size<<shift);

      /*
	Should be useless now
       */
      //if ((index>>shift)>=table_size)
      //{
      //index=0;
      //}

      //      if ((index>>16)>table_size)
      //index=0;
      //DPRINTF("offset_next_index.tick.offset_next_index:%d\n",offset_next_index);
    }
  return table[index>>shift];
}

