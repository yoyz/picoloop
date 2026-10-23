#include "../../Master.h"
#include "ER1Machine.h"

/*
  ---------------------------------------------------------------------------
  PatternElement field reuse
  ---------------------------------------------------------------------------

  ER-1 knob          picoloop field              transform
  ---------------------------------------------------------------------------
  Wave               OSC1_TYPE                   0..6
  Pitch              NOTE1 (+ OSC1_SCALE)        note + scale
  Detune             OSC1_DETUNE                 64 = 0, +-1 semitone
  Mod Type           LFO1_WAVEFORM               0..7
  Mod Depth          PITCHBEND_DEPTH             64 = 0  (default 64!)
  Mod Speed          LFO1_FREQ                   0..127
  Sweeps             LFO1_ENV_AMOUNT             0..64
  Phase              OSC1_PHASE                  0..127 -> 0..360 deg
  Span               OSC2_PHASE                  0 -> 360 deg
  Wave Warp          ENV1_DEPTH                  64 = 0  (default 64!)
  Bit Depth          OSC1_UNISON                 0 = full/17 bit, 127 = 1 bit
  Decay              ADSR_AMP_DECAY              127 = infinite
  Noise              OSC3_AMP                    default 0 = off
  Noise Decay        OSC4_AMP
  Level              OSC1_AMP                    (AMP is the track volume)
  Low Boost          VCO_MIX (= OSC12_MIX)       64 = off
  Filter             FILTER1_CUTOFF
  Filter Pos         FILTER1_ALGO                0 pre, 1 post VCA, 2 post crush
  Reso               FILTER1_RESONANCE
  Key Track          FILTER1_SATURATION          64 = 0
  Filter Type        FILTER1_TYPE                0 off, 1 LP, 2 BP, 3 HP

  All of those IDs are already pushed to the machine by seq_update_track().
  FILTER1_SATURATION is used for Key Track because KEYTRACK is the only knob
  not forwarded there; this avoids touching PatternPlayer.cpp for playback.
  ---------------------------------------------------------------------------
*/

static const WaveTables & er1WaveTables()
{
  // One table set for every voice/mixer.
  static const WaveTables tables;
  return tables;
}

static inline int er1Clamp(int v,int lo,int hi)
{
  if (v<lo) return lo;
  if (v>hi) return hi;
  return v;
}

static inline double er1Percent(int v)
{
  return double(er1Clamp(v,0,127)) * 100.0 / 127.0;
}

// 0..127 centered on 64 -> -100..0..+100
static inline double er1Signed100(int v)
{
  v=er1Clamp(v,0,127);
  if (v==64) return 0.0;
  if (v<64)  return double(v-64) * 100.0 / 64.0;
  return double(v-64) * 100.0 / 63.0;
}

// 0..127 centered on 64 -> 0..+100 (for knobs that have no negative side)
static inline double er1From64(int v)
{
  if (v<=64) return 0.0;
  return double(er1Clamp(v,64,127)-64) * 100.0 / 63.0;
}

// Bit depth curve for the BIT knob (OSC1_UNISON, 0..127).
//
//   0 ..  25 (20 % of the travel) : 17 .. 6 bit  (subtle, barely audible)
//  25 .. 127 (80 % of the travel) :  6 .. 1 bit  (the useful bitcrush range)
//
// 0 is "no crush" (17 bit) so the PatternElement default sounds clean, and
// 127 is the maximum crush (1 bit). Fractional depths are supported by the
// voice (quantisation step = 2^(1-bits)), so the sweep is continuous.
static inline double er1BitDepth(int v)
{
  const int split=25;

  v=er1Clamp(v,0,127);
  if (v<=split)
    return 17.0 - 11.0 * double(v) / double(split);
  return 6.0 - 5.0 * double(v-split) / double(127-split);
}

ER1Machine::ER1Machine()
  : voice(er1WaveTables(),DEFAULTFREQ)
{
  DPRINTF("ER1Machine::ER1Machine()");
  note=60;
  scale=0;
  velocity=127;
  level=80.0;
  index=ER1_SAM;
}

ER1Machine::~ER1Machine()
{
  DPRINTF("ER1Machine::~ER1Machine()");
}

void ER1Machine::init()
{
  DPRINTF("ER1Machine::init()");
  voice.setSampleRate(DEFAULTFREQ);
  this->reset();
}

void ER1Machine::reset()
{
  voice.reset();
  index=ER1_SAM;
}

void ER1Machine::applyPitch()
{
  int n=note+scale;
  if (n>126) n=126;
  if (n<-8)  n=-8;
  voice.setParam(P_PITCH,n);
}

void ER1Machine::applyLevel()
{
  voice.setParam(P_LEVEL,level*double(velocity)/127.0);
}

Sint32 ER1Machine::tick()
{
  if (index>=ER1_SAM)
    {
      voice.render(stereo,ER1_SAM);
      for (int i=0;i<ER1_SAM;i++)
        {
          // Pan is not used by picoloop: fold the stereo voice to mono.
          float s=(stereo[i*2+0]+stereo[i*2+1])*0.70710678f;
          s*=4000.0f;
          if (s> 32767.0f) s= 32767.0f;
          if (s<-32768.0f) s=-32768.0f;
          buffer[i]=(Sint32)s;
        }
      index=0;
    }

  // The ER-1 voice is one-shot (decay only). TRIG_TIME_MODE/DURATION are
  // deliberately ignored: the sequencer always enables them with the
  // PatternElement default (1 -> 512 samples), which would cut every drum
  // after ~12 ms. NOTE_ON=0 still performs a short release if it is sent.
  Sint32 out=buffer[index];
  index++;
  return out;
}

void ER1Machine::setF(int what,float val)
{
  // seq_update_track() sends LFO1_FREQ through setF(), not setI().
  if (what==LFO1_FREQ) voice.setParam(P_MODSPEED,double(er1Clamp((int)(val+0.5f),0,127)));
}

void ER1Machine::setI(int what,int val)
{
  int v=this->checkI(what,val);

  switch (what)
    {
    case OSC1_TYPE:
      voice.setParam(P_WAVE,er1Clamp(v,0,6));
      break;

    case NOTE1:
      note=v;
      this->applyPitch();
      break;

    case OSC1_SCALE:
      scale=v;
      this->applyPitch();
      break;

    case OSC1_DETUNE:
      voice.setParam(P_DETUNE,double(v-64)/64.0);
      break;

    case LFO1_WAVEFORM:
      voice.setParam(P_MODTYPE,er1Clamp(v,0,7));
      break;

    case PITCHBEND_DEPTH:
      // 64 = no modulation; signed -64..+63
      voice.setParam(P_MODDEPTH,double(v-64));
      break;

    case LFO1_FREQ:
      voice.setParam(P_MODSPEED,v);
      break;

    case LFO1_ENV_AMOUNT:
      voice.setParam(P_SWEEPS,er1Clamp(v,0,64));
      break;

    case OSC1_PHASE:
      voice.setParam(P_PHASE,double(v)*360.0/127.0);
      break;

    case OSC2_PHASE:
      // PatternElement default is 0, ER-1 default span is a full cycle.
      voice.setParam(P_SPAN,(v==0)?360.0:double(v)*360.0/127.0);
      break;

    case ENV1_DEPTH:
      // 64 = no wave warp
      voice.setParam(P_WAVEWARP,er1Signed100(v));
      break;

    case OSC1_UNISON:
      // BIT knob: 0 = no crush (17 bit), 127 = maximum crush (1 bit).
      voice.setParam(P_BITDEPTH,er1BitDepth(v));
      break;

    case ADSR_AMP_DECAY:
      voice.setParam(P_DECAY,(v>=127)?128.0:double(v));
      break;

    case OSC3_AMP:
      voice.setParam(P_NOISE,er1Percent(v));
      break;

    case OSC4_AMP:
      voice.setParam(P_NOISEDECAY,v);
      break;

    case OSC1_AMP:
      level=er1Percent(v);
      this->applyLevel();
      break;

    case VCO_MIX:
      // 64 is the PatternElement default -> no low boost
      voice.setParam(P_LOWBOOST,(v>64)?double(v-64)*100.0/63.0:0.0);
      break;

    case FILTER1_CUTOFF:
      voice.setParam(P_FILTER,v);
      break;

    case FILTER1_ALGO:
      voice.setParam(P_FILTERPOS,er1Clamp(v,0,2));
      break;

    case FILTER1_RESONANCE:
      voice.setParam(P_RESO,er1Percent(v));
      break;

    case FILTER1_SATURATION:
      // Key Track, biased so the default (64) means 0%
      voice.setParam(P_KEYTRACK,er1From64(v));
      break;

    case FILTER1_TYPE:
      voice.setParam(P_FILTERTYPE,er1Clamp(v,0,3));
      break;

    case VELOCITY:
      velocity=v;
      this->applyLevel();
      break;

    case NOTE_ON:
      if (v==1)
        {
          this->applyPitch();
          voice.trigger();
        }
      else
        {
          voice.release();
        }
      break;

    default:
      break;
    }
}

int ER1Machine::getI(int what)
{
  if (what==NOTE_ON) return voice.active()?1:0;
  return -1;
}

int ER1Machine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:         return er1Clamp(val,0,6);
    case LFO1_WAVEFORM:     return er1Clamp(val,0,7);
    case FILTER1_ALGO:      return er1Clamp(val,0,2);
    case FILTER1_TYPE:      return er1Clamp(val,0,3);
    case ADSR_AMP_DECAY:    return er1Clamp(val,0,127);
    default:
      return er1Clamp(val,0,127);
    }
}

const char * ER1Machine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_null  = "     ";

  static const char * str_wave[7] =
    { "SINE ","TRI  ","SAW  ","SQR  ","PUL25","PUL12","NOISE" };

  static const char * str_mod[8] =
    { "SAWDN","SQUAR","TRI  ","S&H  ","NOISE","ENV  ","PUL25","PUL12" };

  static const char * str_pos[3] = { "PRE  ","POST ","CRSH " };
  static const char * str_flt[4] = { "OFF  ","LP   ","BP   ","HP   " };

  // Indexed by rounded bit depth, 0 = "no crush".
  static const char * str_bits[18] =
    { "FULL ",
      " 1bit"," 2bit"," 3bit"," 4bit"," 5bit"," 6bit"," 7bit"," 8bit"," 9bit",
      "10bit","11bit","12bit","13bit","14bit","15bit","16bit","17bit" };

  switch (machineParam)
    {
    case OSC1_TYPE:
      return str_wave[this->checkI(OSC1_TYPE,paramValue)];

    case LFO1_WAVEFORM:
      return str_mod[this->checkI(LFO1_WAVEFORM,paramValue)];

    case FILTER1_ALGO:
      return str_pos[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_flt[this->checkI(FILTER1_TYPE,paramValue)];

    case OSC1_UNISON:
      {
        if (paramValue<=0) return str_bits[0];
        int b=(int)(er1BitDepth(paramValue)+0.5);
        if (b<1)  b=1;
        if (b>17) b=17;
        return str_bits[b];
      }

    case LFO1_ENV_AMOUNT:
      if (paramValue==0) return "FREE ";
      return str_null;

    default:
      return str_null;
    }
}
