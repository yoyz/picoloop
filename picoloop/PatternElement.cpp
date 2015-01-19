using namespace std;

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "PatternElement.h"
#include "Machine.h"

PatternElement::PatternElement()
{
  printf("PatternElement::PatternElement()\n");
  note=0;
  note_on=0;
  Channel=0;
  vcomix=64;

  amp=127;

  amp_attack=64;
  amp_decay=64;
  amp_sustain=64;
  amp_release=64;

  fltr_attack=64;
  fltr_decay=64;
  fltr_sustain=64;
  fltr_release=64;

  osc1_amp=64;
  osc2_amp=64;


  fltr_cutoff=64;
  fltr_resonance=30;

  lfo_depth=0;
  lfo_speed=0;


  pb_depth=64; // middle so no pitch bend
  pb_speed=0;

  lfo_type=0;  // 0 lfo pitch, 1 pitchbend
    

  oscOneType=SAW;
  oscTwoType=PULSE;

  adsr_note=1;

  machine_type=0;

  trig_time=0;

  filterType=FILTER_TYPE_LOWPASS;
  filterAlgo=FILTER_ALGO_AMSYNTH;

  fx_depth=0;
  fx_speed=0;

  fm_type=0;
}

PatternElement::~PatternElement()
{
  //printf("PatternElement::~PatternElement()\n");
}


void PatternElement::init()
{
  note=0;
  note_on=0;
  Channel=0;
  vcomix=64;

  amp=127;

  amp_attack=64;
  amp_decay=64;
  amp_sustain=64;
  amp_release=64;

  fltr_attack=64;
  fltr_decay=64;
  fltr_sustain=64;
  fltr_release=64;



  fltr_cutoff=64;
  fltr_resonance=80;

  lfo_depth=0;
  lfo_speed=0;
    
  lfo_type=0;

  oscOneType=SAW;
  oscTwoType=PULSE;

  adsr_note=1;

  filterType=FILTER_TYPE_LOWPASS;
  filterAlgo=FILTER_ALGO_AMSYNTH;

}

void PatternElement::set(int what,int val)
{
  switch (what)
    {
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
      lfo_depth=checkSevenBitBoundarie(val);
      break;

    case LFO1_FREQ:
      lfo_speed=checkSevenBitBoundarie(val);
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

    case NOTE:
      note=checkSevenBitBoundarie(val);
      break;      


    case FILTER1_TYPE:
      filterType=checkSevenBitBoundarie(val);
      break;
    case FILTER1_ALGO:
      filterAlgo=checkSevenBitBoundarie(val);
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

    case VCO_MIX:
      vcomix=checkSevenBitBoundarie(val);
      break;

    case OSC1_PHASE:
      phaseOsc1=checkSevenBitBoundarie(val);
      break;

    case TRIG_TIME_DURATION:
      trig_time=checkSevenBitBoundarie(val);
      break;
      

      
    default:
	printf("PatternElement::set(%d,%d)\n",what,val);
	printf("case default => exit\n");
	exit(1);
	break;
    }
}


int PatternElement::get(int what)
{
  switch (what)
    {
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
      return lfo_depth;
      break;      
    case LFO1_FREQ:
      return lfo_speed;
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

    case NOTE:
      return note;
      break;

    case FILTER1_TYPE:
      return filterType;
      break;      
    case FILTER1_ALGO:
      return filterAlgo;
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

    case VCO_MIX:
      return vcomix;
      break;


    case OSC1_PHASE:
      return phaseOsc1;
      break;

    case TRIG_TIME_DURATION:
      return trig_time;
      break;


    // case VCO:
    //   return osc2_amp;
    //   break;


    default:
      printf("PatternElement::get(%d,%d)\n",what);
      printf("case default : exit");
      exit(1);

    }
  
  //return 1;
}


int PatternElement::checkSevenBitBoundarie(int val)
{
  if (val < 0)
    return 0;
  if (val > 127)
    return 127;
  return val;
}

// void PatternElement::setVCOMix(int mix)
// {
//   vcomix=this->checkSevenBitBoundarie(mix);  
//   printf("[vcomix:%d]\n",vcomix);
// }

// int PatternElement::getVCOMix()
// {
//   return vcomix;
// }


int PatternElement::getFmType()
{
  return fm_type;
}

void PatternElement::setFmType(int fm)
{
  if (fm>1)
    fm=0;
  if (fm<0)
    fm=1;
  fm_type=this->checkSevenBitBoundarie(fm);  
}



int PatternElement::getMachineType()
{
  return machine_type;
}

void PatternElement::setMachineType(int val)
{
  if      (val>2) val=0;
  else if (val<0) val=2;
  machine_type=val;
}



// void PatternElement::setPhaseOsc1(int val)
// {
//   phaseOsc1=this->checkSevenBitBoundarie(val);
// }

// int PatternElement::getPhaseOsc1()
// {
//   return phaseOsc1;
// }





const char * PatternElement::getFilterTypeCharStar()
{
  static const char * str_filter_type_unknown  = "UKNW";
  static const char * str_filter_type_lowpass  = "LP";
  static const char * str_filter_type_bandpass = "BP";
  static const char * str_filter_type_hipass   = "HP";

  if (filterType==FILTER_ALGO_NOFILTER) return str_filter_type_lowpass;
  if (filterType==FILTER_ALGO_BIQUAD)   return str_filter_type_bandpass;
  if (filterType==FILTER_ALGO_AMSYNTH)  return str_filter_type_hipass;
  return str_filter_type_unknown;
}


const char * PatternElement::getLFOTypeCharStar()
{
  static const char * str_lfo_type_unknown  = "UKNW";
  static const char * str_lfo_type_lfopitch = "PLFO";
  static const char * str_lfo_type_pitchbend= "PB";


  if (lfo_type  ==LFO_LFOPITCH)         return str_lfo_type_lfopitch;
  if (lfo_type  ==LFO_PITCHBEND)        return str_lfo_type_pitchbend;
  return str_lfo_type_unknown;
}


void PatternElement::setLfoType(int val)
{
  if (val>1) val=0;
  if (val<0) val=1;

  lfo_type=this->checkSevenBitBoundarie(val);
}

int PatternElement::getLfoType()
{
  return lfo_type;
}



const char * PatternElement::getFilterAlgoCharStar()
{
  static const char * str_filter_algo_unknown  = "UKNW";
  static const char * str_filter_algo_nofilter = "NOFL";
  static const char * str_filter_algo_biquad   = "BIQU";
  static const char * str_filter_algo_amsynth  = "AMST";

  if (filterAlgo==FILTER_ALGO_NOFILTER) return str_filter_algo_nofilter;
  if (filterAlgo==FILTER_ALGO_BIQUAD)   return str_filter_algo_biquad;
  if (filterAlgo==FILTER_ALGO_AMSYNTH)  return str_filter_algo_amsynth;
  return str_filter_algo_unknown;
}


const char * PatternElement::getMachineTypeCharStar()
{
  static const char * str_picosynth   = "PSYNTH ";
  static const char * str_picodrum    = "PDRUM  ";
  static const char * str_opl2        = "OPL2 ";

  switch (machine_type)
    {
    case SYNTH_PICOSYNTH:  return str_picosynth;   break;
    case SYNTH_PICODRUM:   return str_picodrum;    break;
    case SYNTH_OPL2:       return str_opl2;        break; 
    }
  return str_picosynth;
}


const char * PatternElement::getFMTypeCharStar()
{
  static const char * str_2_op_AM   = "2OP_AM ";
  static const char * str_2_op_FM   = "2OP_FM ";

  switch (fm_type)
    {
    case FMTYPE_2_OP_FM:  return str_2_op_FM;  break;
    case FMTYPE_2_OP_AM:  return str_2_op_AM;  break; 

    }
  return str_2_op_AM;
}



const char * PatternElement::getOscTypeCharStar(int oscType)
{
  static const char * str_sine       = "SINE ";
  static const char * str_saw        = "SAW  ";
  static const char * str_pulse      = "PULSE";
  static const char * str_trgl       = "TRGL";
  static const char * str_noise      = "NOISE";
  static const char * str_wtbl       = "WTBL";

  static const char * str_opl2_sine  = "  SIN";
  static const char * str_opl2_hsin  = " HSIN";
  static const char * str_opl2_absin = "ABSIN";
  static const char * str_opl2_qsin  = " QSIN";

  if 	  (
	   machine_type==SYNTH_PICOSYNTH ||
	   machine_type==SYNTH_PICODRUM
	   )
    switch (oscType)
      {
      case SINE:  return str_sine;  break;
      case SAW:   return str_saw;   break; 
      case PULSE: return str_pulse; break;      
      case TRGL:  return str_trgl;  break;            
      case NOISE: return str_noise; break;            
      }
  
  
  if (
      machine_type==SYNTH_OPL2
      )
    switch (oscType) 
      {
      case OPL2_SIN:           return str_opl2_sine;  break;
      case OPL2_HALF_SIN:      return str_opl2_hsin;  break; 
      case OPL2_ABS_SIN:       return str_opl2_absin; break;      
      case OPL2_QUART_SIN:     return str_opl2_qsin;  break;            
      }
  
  return str_sine;
}



const char * PatternElement::getOscOneTypeCharStar()
{
  return this->getOscTypeCharStar(oscOneType);
}


const char * PatternElement::getOscTwoTypeCharStar()
{
  return this->getOscTypeCharStar(oscTwoType);
}


void PatternElement::print()
{
  cout << getStr() << "\n";
}

string PatternElement::getStr()
{
  stringstream ss;
  string s;
  ss << "note:" << note << "\ttrig:" << note_on ;
  s=ss.str();
  return s;
}

