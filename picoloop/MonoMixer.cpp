#include "MonoMixer.h"


#if   defined(__FPU__) && defined(__RTMIDI__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(), CS(), O303(), TW(), MD(), MIDIOUTM(), FXDelay(), FXDisabled()
#endif

#if   defined(__FPU__) && !defined(__RTMIDI__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(), CS(), O303(), TW(),                 FXDelay(), FXDisabled()
#endif

#if  !defined(__FPU__) && !defined(__RTMIDI__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(),                                     FXDelay(), FXDisabled()
#endif
{
  DPRINTF("MonoMixer::MonoMixer()");  
  amplitude=127;
  //  M=NULL;
  //M=&PM;
  //DPRINTF("MonoMixer::MonoMixer()  PM=0x%08.8X\n",PM);
  //DPRINTF("MonoMixer::MonoMixer()  M=0x%08.8X\n",M);
  //M=&OPLM;
  machine_type=0;
  M=&PS;


  //FX=&FXDelay;

  fx_depth=125;
  fx_speed=90;
}

MonoMixer::~MonoMixer()
{
  DPRINTF("MonoMixer::~MonoMixer()");  
}


void MonoMixer::init()
{
  PS.init();
  PD.init();
  OPLM.init();
  PBS.init();

#if defined(__FPU__)
  CS.init();
  O303.init();
  TW.init();
  MD.init();
#endif

  //FX=&FXDelay;
  FX=&FXDelay;
  FX=&FXDisabled;
  FX=&FXDelay;

  FX->init();
  FX->setDepth(fx_depth);
  FX->setSpeed(fx_speed);
  
  FXDelay.init();
  FXDelay.reset();
  
  FXDelay.setDepth(fx_depth);
  FXDelay.setSpeed(fx_speed);

  

  if (machine_type==SYNTH_PICOSYNTH)
    M=&PS;
  if (machine_type==SYNTH_OPL2)
    M=&OPLM;
  if (machine_type==SYNTH_PICODRUM)
    M=&PD;
  if (machine_type==SYNTH_PBSYNTH)
    M=&PBS;
#if defined(__FPU__)
  if (machine_type==SYNTH_CURSYNTH)
    M=&CS;
  if (machine_type==SYNTH_OPEN303)
    M=&O303;
  if (machine_type==SYNTH_TWYTCHSYNTH)
    M=&TW;
#endif
#if defined(__RTMIDI__)
  if (machine_type==SYNTH_MIDIOUT)
    M=&MIDIOUTM;
#endif
}


void MonoMixer::setMachineType(int type)
{
  DPRINTF("MonoMixer::setMachineType :%d",type);  
  machine_type=type;

  switch (type)
    {
    case SYNTH_PICOSYNTH:
      M=&PS;
      break;

    case SYNTH_OPL2:
      M=&OPLM;
      break;

    case SYNTH_PICODRUM:
      M=&PD;
      break;

    case SYNTH_PBSYNTH:
      M=&PBS;
      break;
#if defined(__FPU__)
    case SYNTH_CURSYNTH:
      M=&CS;
      break;
    case SYNTH_OPEN303:
      M=&O303;
      break;
    case SYNTH_TWYTCHSYNTH:
      M=&TW;
      break;
    case SYNTH_MDADRUM:
      M=&MD;
      break;
#endif

#if defined(__RTMIDI__)
    case SYNTH_MIDIOUT:
      M=&MIDIOUTM;
      break;      
#endif

    default:
      DPRINTF("void MonoMixer::setMachineType(%d)",type);
      DPRINTF("ERROR : This machine does not exist : %d",type);
      exit(1);
      break;
    }
}

void MonoMixer::setAmplitude(int amp)
{
  int debug=0;
  if (amp>=127)           { amplitude=127; }
  if (amp<=0)             { amplitude=0;   }
  if (amp>0   && amp<127) { amplitude=amp; }
  DPRINTF("MonoMixer::setAmplitude(amplitude=%d)",amplitude);

  //M->setAmplitude(amp);
  amplitude=amp;
}
/*
void MonoMixer::setInput(Machine * Mach)
{
  M=Mach;
}
*/
Machine * MonoMixer::getInput()
{
  //M->init();
  return M;
  
}

Effect * MonoMixer::getEffect()
{
  //M->init();
  return FX;
  
}


Sint16 MonoMixer::tick()
{
  int debug=0;
  //  /*
  Sint32 res32=0;
  Sint16 tick=0;
  Sint16 res16=0;
  //tick=M->tick();
  //tick=FX.process(M->tick());
  //tick=FXDelay.process(M->tick());

  tick=FX->process(M->tick());
  res32=tick*amplitude;
  //  res32=tick*127;
  //res32=res32/127;
  
  res32=res32>>4;

  //if (res32>32000)  res32=32000-(res32>>6);
  //if (res32<-32000) res32=-32000+(res32>>6);

  //if (res32>32000)  res32=32000;
  //if (res32<-32000) res32=-32000;

  if (res32>32000)  res32=32000-(res32-32000);
  if (res32<-32000) res32=-32000-(res32+32000);

  //  return M->tick();
  res16=res32;
  //if (0) DPRINTF("amplitude:%d tick:%d res32:%d res16:%d\n",amplitude,tick,res32,res16);
  return res16;
  //*/
  //return M->tick();
}
