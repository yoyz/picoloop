#include "MonoMixer.h"

//PCDESKTOP
#if   defined(__VECTORFPU__) && defined(__RTMIDI__) && defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(), CS(), O303(), TW(), MD(), SS(), LGPTSMPL(), MIDIOUTM(), FXDelay(), FXDisabled()
#endif
			
#if   defined(__VECTORFPU__) && !defined(__RTMIDI__) && defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(), CS(), O303(), TW(), MD(), SS(), LGPTSMPL(),            FXDelay(), FXDisabled()
#endif

#if   !defined(__VECTORFPU__) && defined(__RTMIDI__) && !defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(),                          MIDIOUTM(), FXDelay(), FXDisabled()
#endif

//PSVITA, POCKETCHIP
#if  !defined(__VECTORFPU__) && !defined(__RTMIDI__) && defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(),                                  LGPTSMPL(),          FXDelay(), FXDisabled()
#endif

			
//  OPENDINGUX, GP2X, PSP
#if  !defined(__VECTORFPU__) && !defined(__RTMIDI__) && !defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
MonoMixer::MonoMixer(): PD(), PS(), OPLM(), PBS(),                         FXDelay(), FXDisabled()
#endif

//  LINUX TEST
#if  defined(__PBSYNTHONLY__)
MonoMixer::MonoMixer(): PBS(),                         FXDelay(), FXDisabled()
#endif


			
#define SAMMONOMIXER 32

{
  DPRINTF("MonoMixer::MonoMixer()");  
  amplitude=127;

#if !defined(__PBSYNTHONLY__)
  M=&PS;
  machine_type=0;
#endif
#if defined(__PBSYNTHONLY__)
  M=&PBS;
  machine_type=3;
#endif
  fx_depth=125;
  fx_speed=90;

  index=0;
  buffer16=NULL;
  buffer32=NULL;

  channel=-1; // not set 
}

MonoMixer::~MonoMixer()
{
  DPRINTF("MonoMixer::~MonoMixer()");
  if (buffer16)
    free(buffer16);
  if (buffer32)
    free(buffer32);
}


void MonoMixer::init()
{
  int i=0;
  PBS.init();
  
#if !defined(__PBSYNTHONLY__)
  PS.init();
  PD.init();
  OPLM.init();
#endif

#if defined(__RAM512MIB__)
  LGPTSMPL.init();
#endif

#if defined(__VECTORFPU__)
  CS.init();
  O303.init();
  TW.init();
  MD.init();
  SS.init();
#endif

  FX=&FXDelay;

  FX->init();
  FX->setDepth(fx_depth);
  FX->setSpeed(fx_speed);
  
  FXDelay.init();
  FXDelay.reset();
  
  FXDelay.setDepth(fx_depth);
  FXDelay.setSpeed(fx_speed);

  this->setMachineType(machine_type);
  
  if (buffer16==NULL)
    buffer16=(Sint16*)malloc(sizeof(Sint16)*SAMMONOMIXER);
  if (buffer32==NULL)
    buffer32=(Sint32*)malloc(sizeof(Sint32)*SAMMONOMIXER);
  for (i=0;i<SAMMONOMIXER;i++)
    {
      buffer16[i]=0;
      buffer32[i]=0;
    }

  channel=-1; // not set
}

void MonoMixer::setMonoMixerChannelNumber(int c)
{
  channel=c;
}
int MonoMixer::getMonoMixerChannelNumber()
{
  return channel;
}

void MonoMixer::setMachineType(int type)
{
  DPRINTF("MonoMixer::setMachineType :%d",type);  
  machine_type=type;

  switch (type)
    {      
    case SYNTH_PBSYNTH:
      M=&PBS;
      break;

#if !defined(__PBSYNTHONLY__)
    case SYNTH_PICOSYNTH:
      M=&PS;
      break;

    case SYNTH_OPL2:
      M=&OPLM;
      break;

    case SYNTH_PICODRUM:
      M=&PD;
      break;
#endif

#if defined(__RAM512MIB__)     
    case SYNTH_LGPTSAMPLER:
      M=&LGPTSMPL;
      break;
#endif      
#if defined(__VECTORFPU__)
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
    case SYNTH_SIDSYNTH:
      M=&SS;
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
      fprintf(stderr,"MonoMixer exit\n");
      exit(1);
      break;
    }
  M->setChannelNumber(channel);
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


void MonoMixer::process()
{
  int debug=0;
  Sint32 res32=0;
  Sint16 tick=0;
  Sint16 res16=0;
  int    i=0;

  // Fill with Machine audio
  for (i=0;i<SAMMONOMIXER;i++)
    {
      buffer32[i]=M->tick();
    }
  //  // Send To FX
  // for (i=0;i<SAMMONOMIXER;i++)
  //   buffer32[i]=FX->process_one_sample(buffer32[i]);

  FX->process(buffer32,SAMMONOMIXER);

  // Amplify it we are a mixer 
  // Clip it it is a 32 bit, so no problem to clip it
  for (i=0;i<SAMMONOMIXER;i++)
    {
      buffer32[i]=(buffer32[i]*amplitude) >> 4;
      if (buffer32[i]>32000) buffer32[i]=32000;
      if (buffer32[i]<-32000) buffer32[i]=-32000;
    }
  // convert it from 32 to 16 bit
  for (i=0;i<SAMMONOMIXER;i++)
    buffer16[i]=(Sint16)buffer32[i];

  // GOTCHA : Here I set index to 0 and I return buffer16[0]
  index=0;  
}

Sint16 MonoMixer::tick()
{

  if (index+1<SAMMONOMIXER)
    {
      index++;
      return buffer16[index];
    }
  this->process();
  return buffer16[index];
  
}
