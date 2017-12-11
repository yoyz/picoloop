using namespace std;

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "PatternElement.h"
#include "Machine.h"

PatternElement::PatternElement()
{
  //DPRINTF("PatternElement::PatternElement()");
  this->init();
}

PatternElement::~PatternElement()
{
  //printf("PatternElement::~PatternElement()\n");
}


void PatternElement::init()
{
  note1=0;
  note2=0;
  note3=0;
  note4=0;
  note_on=0;
  Channel=0;
  vcomix=64;

  amp=127;

  amp_attack=1;
  amp_decay=112;
  amp_sustain=8;
  amp_release=24;

  fltr_attack=8;
  fltr_decay=64;
  fltr_sustain=32;
  fltr_release=32;



  fltr_cutoff=112;
  fltr_resonance=16;

  lfo1_depth=0;
  lfo1_speed=0;

  lfo2_depth=0;
  lfo2_speed=0;

    
  lfo_type=0;

  oscOneType=PICO_WAVETABLE_SAW;
  oscTwoType=PICO_WAVETABLE_PULSE;
  oscThreeType=PICO_WAVETABLE_SINE;

  lfo1Waveform=PICO_CURSYNTH_SIN;   // used by lfo1 cursynth
  lfo2Waveform=PICO_CURSYNTH_SIN;   // used by lfo2 cursynth

  adsr_note=1;

  filterType=FILTER_TYPE_LP;
  filterAlgo=FILTER_ALGO_AMSYNTH;

  filter1_saturation=64;
  filter1_feedback=64;


  lfo1_env_amount=64;
  lfo2_env_amount=64;

  osc1_detune=64;
  osc2_detune=64;

  machine_type=SYNTH_PICOSYNTH;

  velocity=96;

  osc1_mod=0;
  osc2_mod=0;

  osc1_amp=32;
  osc2_amp=32;

  osc3_amp=0;
  osc4_amp=0;

  trig_time=1;
  osc1_unison=0;
  osc2_unison=0;

  osc1_unisondt=0;
  osc2_unisondt=0;

  pb_depth=64;
  pb_speed=0;

  polyphony=1;
  fx_depth=0;
  fx_speed=0;
  fm_type=0;
  
  osc1_scale=0;
  osc2_scale=0;
  osc3_scale=0;
  osc4_scale=0;

  env1_depth=64;
  env2_depth=64;
  env3_depth=64;
  env4_depth=64;

  keytrack=64;
  phaseOsc1=0;
}

void PatternElement::inc(int what)
{
  this->set(what,this->get(what)+1);
}

void PatternElement::dec(int what)
{
  this->set(what,this->get(what)-1);
}

void PatternElement::set(int what,int val)
{
  switch (what)
    {
      // BEGIN NOT CATCHED BY PATTERN_ELEMENT
    case BPM:
      break;
    case BPM_DIVIDER:
      break;
    case PATTERN_LENGTH:
      break;
    case PATTERN_SHIFT:
      break;
    case SWING:
      break;
    case MIDI_SEND_DELTA:
      break;
      // END NOT CATCHED BY PATTERN_ELEMENT
      
    case ADSR_AMP_ATTACK:
      amp_attack=checkSevenBitBoundarie(val);
      break;

    case ADSR_AMP_DECAY:
      amp_decay=checkSevenBitBoundarie(val);
      break;      

    case ADSR_AMP_SUSTAIN:
      amp_sustain=checkSevenBitBoundarie(val);
      break;      

    case ADSR_AMP_RELEASE:
      amp_release=checkSevenBitBoundarie(val);
      break;



    case ADSR_FLTR_ATTACK:
      fltr_attack=checkSevenBitBoundarie(val);
      break;

    case ADSR_FLTR_DECAY:
      fltr_decay=checkSevenBitBoundarie(val);
      break;      

    case ADSR_FLTR_SUSTAIN:
      fltr_sustain=checkSevenBitBoundarie(val);
      break;      

    case ADSR_FLTR_RELEASE:
      fltr_release=checkSevenBitBoundarie(val);
      break;



    case FILTER1_CUTOFF:
      fltr_cutoff=checkSevenBitBoundarie(val);
      break;

    case FILTER1_RESONANCE:
      fltr_resonance=checkSevenBitBoundarie(val);
      break;      


    case LFO1_DEPTH:
      lfo1_depth=checkSevenBitBoundarie(val);
      break;
    case LFO1_ENV_AMOUNT:
      lfo1_env_amount=checkSevenBitBoundarie(val);
      break;      
    case LFO1_FREQ:
      lfo1_speed=checkSevenBitBoundarie(val);
      break;      

    case LFO2_DEPTH:
      lfo2_depth=checkSevenBitBoundarie(val);
      break;
    case LFO2_ENV_AMOUNT:
      lfo2_env_amount=checkSevenBitBoundarie(val);
      break;      
    case LFO2_FREQ:
      lfo2_speed=checkSevenBitBoundarie(val);
      break;      


    case FX1_DEPTH:
      fx_depth=checkSevenBitBoundarie(val);
      break;
    case FX1_SPEED:
      fx_speed=checkSevenBitBoundarie(val);
      break;      

    case PITCHBEND_DEPTH:
      pb_depth=checkSevenBitBoundarie(val);
      break;
    case PITCHBEND_SPEED:
      pb_speed=checkSevenBitBoundarie(val);
      break;      


    case OSC1_TYPE:
      oscOneType=checkSevenBitBoundarie(val);
      break;
    case OSC2_TYPE:
      oscTwoType=checkSevenBitBoundarie(val);
      break;      

    case OSC3_TYPE:
      oscThreeType=checkSevenBitBoundarie(val);
      break;      

    case NOTE1:
      note1=checkSevenBitBoundarie(val);
      break;      


    case FILTER1_TYPE:
      filterType=checkSevenBitBoundarie(val);
      break;
    case FILTER1_ALGO:
      filterAlgo=checkSevenBitBoundarie(val);
      break;      


    case FILTER1_SATURATION:
      filter1_saturation=checkSevenBitBoundarie(val); 
      break;
    case FILTER1_FEEDBACK:
      filter1_feedback=checkSevenBitBoundarie(val); 
      break;      


    case NOTE_ON:
      note_on=checkSevenBitBoundarie(val);
      break;      

    case AMP:
      amp=checkSevenBitBoundarie(val);
      break;      


    case OSC1_AMP:
      osc1_amp=checkSevenBitBoundarie(val);
      break;
    case OSC2_AMP:
      osc2_amp=checkSevenBitBoundarie(val);
      break;

    case OSC3_AMP:
      osc3_amp=checkSevenBitBoundarie(val);
      break;
    case OSC4_AMP:
      osc4_amp=checkSevenBitBoundarie(val);
      break;


    case OSC1_UNISON:
      osc1_unison=checkSevenBitBoundarie(val);
      break;
    case OSC2_UNISON:
      osc2_unison=checkSevenBitBoundarie(val);
      break;

    case OSC1_UNISONDT:
      osc1_unisondt=checkSevenBitBoundarie(val);
      break;
    case OSC2_UNISONDT:
      osc2_unisondt=checkSevenBitBoundarie(val);
      break;


    case VCO_MIX:
      vcomix=checkSevenBitBoundarie(val);
      break;

    case OSC1_PHASE:
      phaseOsc1=checkSevenBitBoundarie(val);
      break;

    case TRIG_TIME_DURATION:
      trig_time=checkSevenBitBoundarie(val);
      break;

      //NOT USED ANYMORE NEED TO BE REMOVED
    case NOTE_ADSR:
      adsr_note=checkSevenBitBoundarie(val);
      break;

    case MACHINE_TYPE:
      machine_type=checkSevenBitBoundarie(val);
      //machine_type=checkSevenBitBoundarie(val)%SYNTH_SIZE; // HACK TO make the DBOPL/PICOSYNTH/PICODRUM TO WORK
      break;


    case FM_TYPE:
      fm_type=checkSevenBitBoundarie(val)%2; // HACK TO make AM/FM
      break;

    case LFO_TYPE:
      lfo_type=checkSevenBitBoundarie(val)%2; // HACK TO make LFOPITCH/PB
      break;


    case OSC1_DETUNE:
      osc1_detune=checkSevenBitBoundarie(val);
      break;

    case OSC2_DETUNE:
      osc2_detune=checkSevenBitBoundarie(val);
      break;


    case LFO1_WAVEFORM:
      lfo1Waveform=checkSevenBitBoundarie(val);
      break;

    case LFO2_WAVEFORM:
      lfo2Waveform=checkSevenBitBoundarie(val);
      break;


    case VELOCITY:
      velocity=checkSevenBitBoundarie(val);
      break;

    case OSC1_MOD:
      osc1_mod=checkSevenBitBoundarie(val);
      break;
    case OSC2_MOD:
      osc2_mod=checkSevenBitBoundarie(val);
      break;


    case OSC1_SCALE:
      osc1_scale=checkThreeOctaveScale(val);
      break;

    case OSC2_SCALE:
      osc2_scale=checkThreeOctaveScale(val);
      break;

    case OSC3_SCALE:
      osc3_scale=checkThreeOctaveScale(val);
      break;

    case OSC4_SCALE:
      osc4_scale=checkThreeOctaveScale(val);
      break;

    case ENV1_DEPTH:
      env1_depth=checkSevenBitBoundarie(val);
      break;
    case ENV2_DEPTH:
      env2_depth=checkSevenBitBoundarie(val);
      break;
    case ENV3_DEPTH:
      env3_depth=checkSevenBitBoundarie(val);
      break;
    case ENV4_DEPTH:
      env4_depth=checkSevenBitBoundarie(val);
      break;

    case KEYTRACK:
      keytrack=checkSevenBitBoundarie(val);
      break;


      

      
    default:
      DPRINTF("PatternElement::set(%d,%d)",what,val);
      printf("==============================>case default => exit\n");
      exit(1);
      break;
    }
}


int PatternElement::get(int what)
{
  switch (what)
    {
      // BEGIN NOT CATCHED BY PATTERN_ELEMENT
    case BPM:
      break;
    case BPM_DIVIDER:
      break;
    case PATTERN_LENGTH:
      break;
    case PATTERN_SHIFT:
      break;
    case SWING:
      break;
    case MIDI_SEND_DELTA:
      break;
      // END NOT CATCHED BY PATTERN_ELEMENT
      
    case ADSR_AMP_ATTACK:
      return amp_attack;
      break;
    case ADSR_AMP_DECAY:
      return amp_decay;
      break;      
    case ADSR_AMP_SUSTAIN:
      return amp_sustain;
      break;      
    case ADSR_AMP_RELEASE:
      return amp_release;
      break;


    case ADSR_FLTR_ATTACK:
      return fltr_attack;
      break;
    case ADSR_FLTR_DECAY:
      return fltr_decay;
      break;      
    case ADSR_FLTR_SUSTAIN:
      return fltr_sustain;
      break;      
    case ADSR_FLTR_RELEASE:
      return fltr_release;
      break;


    case FILTER1_CUTOFF:
      return fltr_cutoff;
      break;      
    case FILTER1_RESONANCE:
      return fltr_resonance;
      break;

    case LFO1_DEPTH:
      return lfo1_depth;
      break;      
    case LFO1_ENV_AMOUNT:
      return lfo1_env_amount;
      break;      
    case LFO1_FREQ:
      return lfo1_speed;
      break;


    case LFO2_DEPTH:
      return lfo2_depth;
      break;      
    case LFO2_ENV_AMOUNT:
      return lfo2_env_amount;
      break;      
    case LFO2_FREQ:
      return lfo2_speed;
      break;


    case FX1_DEPTH:
      return fx_depth;
      break;      
    case FX1_SPEED:
      return fx_speed;
      break;

    case PITCHBEND_DEPTH:
      return pb_depth;
      break;      
    case PITCHBEND_SPEED:
      return pb_speed;
      break;


    case OSC1_TYPE:
      return oscOneType;
      break;      
    case OSC2_TYPE:
      return oscTwoType;
      break;

    case OSC3_TYPE:
      return oscThreeType;
      break;

    case NOTE1:
      return note1;
      break;

    case FILTER1_TYPE:
      return filterType;
      break;      
    case FILTER1_ALGO:
      return filterAlgo;
      break;

    case FILTER1_SATURATION:
      return filter1_saturation;
      break;      
    case FILTER1_FEEDBACK:
      return filter1_feedback;
      break;


    case NOTE_ON:
      return note_on;
      break;

    case AMP:
      return amp;
      break;

   case OSC1_AMP:
      return osc1_amp;
      break;      
    case OSC2_AMP:
      return osc2_amp;
      break;

   case OSC3_AMP:
      return osc3_amp;
      break;      
    case OSC4_AMP:
      return osc4_amp;
      break;


   case OSC1_UNISON:
      return osc1_unison;
      break;      
    case OSC2_UNISON:
      return osc2_unison;
      break;

   case OSC1_UNISONDT:
      return osc1_unisondt;
      break;      
    case OSC2_UNISONDT:
      return osc2_unisondt;
      break;



    case VCO_MIX:
      return vcomix;
      break;


    case OSC1_PHASE:
      return phaseOsc1;
      break;

    case TRIG_TIME_DURATION:
      return trig_time;
      break;




    case NOTE_ADSR:
      return adsr_note;       //NOT USED ANYMORE NEED TO BE REMOVED
      break;

    case MACHINE_TYPE:
      return machine_type;  // HACK TO make the DBOPL/PICOSYNTH/PICODRUM TO WORK
      break;

    case FM_TYPE:
      return fm_type;       // HACK TO make AM/FM
      break;

    case LFO_TYPE:
      return lfo_type;      // HACK TO make LFOPITCH/PB
      break;


    case OSC1_DETUNE:
      return osc1_detune;
      break;

    case OSC2_DETUNE:
      return osc2_detune;
      break;

    case LFO1_WAVEFORM:
      return lfo1Waveform;
      break;

    case LFO2_WAVEFORM:
      return lfo2Waveform;
      break;


    case VELOCITY:
      return velocity;
      break;

    case OSC1_MOD:
      return osc1_mod;
      break;
    case OSC2_MOD:
      return osc2_mod;
      break;


    case OSC1_SCALE:
      return osc1_scale;
      break;

    case OSC2_SCALE:
      return osc2_scale;
      break;

    case OSC3_SCALE:
      return osc3_scale;
      break;

    case OSC4_SCALE:
      return osc4_scale;
      break;

    case ENV1_DEPTH:
      return env1_depth;
      break;
    case ENV2_DEPTH:
      return env2_depth;
      break;
    case ENV3_DEPTH:
      return env3_depth;
      break;
    case ENV4_DEPTH:
      return env4_depth;
      break;

    case KEYTRACK:
      return keytrack;
      break;


    default:
      DPRINTF("PatternElement::get(%d)",what);
      DPRINTF("==============================>case default : exit\n");
      exit(1);

    }
  
  return 0;
}


int PatternElement::checkSevenBitBoundarie(int val)
{
  if (val <= 0)
    return 0;
  if (val >= 127)
    return 127;
  return val;
}

int PatternElement::checkThreeOctaveScale(int val)
{
  if (val <= 0)
    return 0;
  if (val >= 12*3)
    return 12*3;
  return val;
}



void PatternElement::print()
{
  cout << getStr() << "\n";
}

string PatternElement::getStr()
{
  stringstream ss;
  string s;
  ss << "note1:" << note1 << "\ttrig:" << note_on ;
  s=ss.str();
  return s;
}

