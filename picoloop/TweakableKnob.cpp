#include "TweakableKnob.h"

TweakableKnob::TweakableKnob()
{
  // bpm=0;
  // divider=0;
  // swing=0;

  // invert_trig=0;

  // note=0;
  // note_all=0;
  
  // amp=0;                  // variation of the amp of monomixer
  // amp_all=0;              // variation of the amp of monomixer
  
  // attack_amp=0;
  // decay_amp=0;
  // sustain_amp=0;
  // release_amp=0;
  
  // attack_fltr=0;
  // decay_fltr=0;
  // sustain_fltr=0;
  // release_fltr=0;
  
  
  // attack_amp_all=0;
  // decay_amp_all=0;
  // sustain_amp_all=0;
  // release_amp_all=0;
  
  // attack_fltr_all=0;
  // decay_fltr_all=0;
  // sustain_fltr_all=0;
  // release_fltr_all=0;
  
  
  // pshift_all=0;
  // plength_all=0;
  
  // trig_time=0;
  // trig_time_all=0;
  
  
  // lfo_depth=0;
  // lfo_depth_all=0;
  // lfo_speed=0;
  // lfo_speed_all=0;
  
  // pb_depth=0;
  // pb_depth_all=0;
  // pb_speed=0;
  // pb_speed_all=0;
  
  // lfo_type=0;
  // lfo_type_all=0;
  
  // cutoff=0;
  // resonance=0;
  
  // cutoff_all=0;
  // resonance_all=0;
  
  // fx_depth=0;
  // fx_speed=0;
  
  // fx_depth_all=0;
  // fx_speed_all=0;
  
  // vcomix=0;
  // vcomix_all=0;
  
  // osconetype=0;
  // osctwotype=0;
  
  // osconetype_all=0;
  // osctwotype_all=0;
  
  // machine_type=0;
  // machine_type_all=0;
  
  // phase_osc1=0;
  // phase_osc1_all=0;
  
  // filter_algo=0;
  // filter_algo_all=0;
  
  // filter_type=0;
  // filter_type_all=0;
  
  // osc1_amp=0;
  // osc1_amp_all=0;
  
  // osc2_amp=0;
  // osc2_amp_all=0;
  
  // fmtype=0;
  // fmtype_all=0;  
  machineParamArray=0;
  machineParamArrayAll=0;
}

TweakableKnob::~TweakableKnob()
{
  if (machineParamArray==0)
    free(machineParamArray);
  if (machineParamArrayAll==0)
    free(machineParamArrayAll);
}
  
void TweakableKnob::reset()
{
  int i;
  for (i=0;i<NBELEM_TWEAKABLEKNOB;i++)
    {
      machineParamArray[i]=0;
      machineParamArrayAll[i]=0;
    }
}  


void TweakableKnob::init()
{
  if (machineParamArray==0)
    machineParamArray=(int*)malloc(sizeof(int)*NBELEM_TWEAKABLEKNOB);
  if (machineParamArrayAll==0)
    machineParamArrayAll=(int*)malloc(sizeof(int)*NBELEM_TWEAKABLEKNOB);
  this->reset();
}


void TweakableKnob::set(int machineParam,int value)
{
  machineParamArray[machineParam]=value;
}


void TweakableKnob::setAll(int machineParam,int value)
{
  machineParamArrayAll[machineParam]=value;
}

int TweakableKnob::get(int machineParam)
{
  return machineParamArray[machineParam];
}

int TweakableKnob::getAll(int machineParam)
{
  return machineParamArrayAll[machineParam];
}

