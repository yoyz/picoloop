#include "MonoMixer.h"
#define SAM 32
			
MonoMixer::MonoMixer()
{
  DPRINTF("MonoMixer::MonoMixer()");  
  amplitude=127;
  fx_depth=125;
  fx_speed=90;

  index=0;
  buffer16=NULL;
  buffer32=NULL;
  bufferfix=NULL;

  channel=-1; // not set
#if !defined(__PBSYNTHONLY__)
  machine_type=0;
#endif
#if defined(__PBSYNTHONLY__)
  machine_type=3;
#endif

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
  amplitude=127;
  fx_depth=125;
  fx_speed=90;

  index=0;
  buffer16=NULL;
  buffer32=NULL;
  bufferfix=NULL;

  channel=-1; // not set 
#if !defined(__PBSYNTHONLY__)
  machine_type=0;
#endif
#if defined(__PBSYNTHONLY__)
  machine_type=3;
#endif

#ifdef      ENABLE_SYNTH_PBSYNTH
  PBS   = new PBSynthMachine();
  PBS->init();
#endif

  
#ifdef ENABLE_SYNTH_PICOSYNTH
  PS   = new PicosynthMachine();
  PS->init();
#endif

#ifdef ENABLE_SYNTH_PICODRUM
  PD   = new PicodrumMachine();
  PD->init(); 
#endif

#ifdef ENABLE_SYNTH_OPL2
  OPLM = new dboplMachine();
  OPLM->init();
#endif

  
#ifdef ENABLE_SYNTH_LGPTSAMPLER
  LGPTSMPL = new LgptsamplerMachine();
  LGPTSMPL->init();
#endif

#ifdef ENABLE_SYNTH_CURSYNTH
  CS     = new CursynthMachine();
  CS->init();
#endif

#ifdef ENABLE_SYNTH_OPEN303
  O303   = new Open303Machine();
  O303->init();
#endif

#ifdef ENABLE_SYNTH_TWYTCHSYNTH
  TW     = new TwytchsynthMachine();
  TW->init();
#endif

#ifdef ENABLE_SYNTH_MDADRUM
  MD     = new MDADrumMachine();
  MD->init();
#endif

#ifdef ENABLE_SYNTH_SIDSYNTH
  SS     = new SIDSynthMachine();
  SS->init();
#endif




#ifdef ENABLE_SYNTH_MIDIOUT
  MIDIOUTM = new MidiOutMachine();
  MIDIOUTM->init();
#endif

  FXDelay= new EffectDelay();
  
  FX=FXDelay;

  FX->init();
  FX->setDepth(fx_depth);
  FX->setSpeed(fx_speed);
  
  FXDelay->init();
  FXDelay->reset();
  
  FXDelay->setDepth(fx_depth);
  FXDelay->setSpeed(fx_speed);

  this->setMachineType(machine_type);
  
  if (buffer16==NULL)  buffer16=(Sint16*)malloc(sizeof(Sint16)*SAM);
  if (buffer16==NULL)  { DPRINTF("OOM MM::init()"); CLOSE_DPRINTF(); exit(1); }
  if (buffer32==NULL)  buffer32=(Sint32*)malloc(sizeof(Sint32)*SAM);
  if (buffer32==NULL)  { DPRINTF("OOM MM::init()"); CLOSE_DPRINTF(); exit(1); }
  if (bufferfix==NULL) bufferfix=(Fixed*)malloc(sizeof(Fixed)*SAM);
  if (bufferfix==NULL) { DPRINTF("OOM MM::init()"); CLOSE_DPRINTF(); exit(1); }
    
  for (i=0;i<SAM;i++)
    {
      buffer16[i]=0;
      buffer32[i]=0;
      bufferfix[i]=0;
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

#ifdef ENABLE_SYNTH_PBSYNTH
    case      SYNTH_PBSYNTH:
      M=PBS;
      break;
#endif

#ifdef ENABLE_SYNTH_PICOSYNTH
    case      SYNTH_PICOSYNTH:
      M=PS;
      break;
#endif

#ifdef ENABLE_SYNTH_OPL2
    case      SYNTH_OPL2:
      M=OPLM;
      break;
#endif

#ifdef ENABLE_SYNTH_PICODRUM
    case      SYNTH_PICODRUM:
      M=PD;
      break;
#endif

#ifdef ENABLE_SYNTH_LGPTSAMPLER   
    case      SYNTH_LGPTSAMPLER:
      M=LGPTSMPL;
      break;
#endif      

#ifdef ENABLE_SYNTH_CURSYNTH
    case      SYNTH_CURSYNTH:
      M=CS;
      break;
#endif

#ifdef ENABLE_SYNTH_OPEN303
    case      SYNTH_OPEN303:
      M=O303;
      break;
#endif

#ifdef ENABLE_SYNTH_TWYTCHSYNTH
    case      SYNTH_TWYTCHSYNTH:
      M=TW;
      break;
#endif

#ifdef ENABLE_SYNTH_MDADRUM
    case      SYNTH_MDADRUM:
      M=MD;
      break;
#endif
#ifdef ENABLE_SYNTH_SIDSYNTH
    case      SYNTH_SIDSYNTH:
      M=SS;
      break;
#endif

#ifdef ENABLE_SYNTH_MIDIOUT
    case      SYNTH_MIDIOUT:
      M=MIDIOUTM;
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
  amplitudeFixed=Fixed(amplitude)/Fixed(127);
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
  for (i=0;i<SAM;i++)
    {
      buffer32[i]=M->tick();
    }

    for (i=0;i<SAM;i++)
    {
      //buffer32[i]=M->tick();
      buffer32[i]=FX->process_one_sample(buffer32[i]);
    }
  //FX->process(buffer32,SAM);

  // Amplify it we are a mixer 
  // Clip it it is a 32 bit, so no problem to clip it
  for (i=0;i<SAM;i++)
    {
      buffer32[i]=(buffer32[i]*amplitude) >> 4;
      if (buffer32[i]>32000) buffer32[i]=32000;
      if (buffer32[i]<-32000) buffer32[i]=-32000;
    }
  // convert it from 32 to 16 bit
  for (i=0;i<SAM;i++)
    buffer16[i]=(Sint16)buffer32[i];

  // GOTCHA : Here I set index to 0 and I return buffer16[0]
  index=0;  
}


void MonoMixer::process_fixed()
{
  int debug=0;
  Sint32 res32=0;
  Sint16 tick=0;
  Sint16 res16=0;
  int    i=0;

  // Fill with Machine audio
  for (i=0;i<SAM;i++)
    {
      bufferfix[i]=M->tick_fixed()*amplitudeFixed;
    }
  index=0;
  return;
}


Sint16 MonoMixer::tick()
{
  if (index+1<SAM)
    {
      index++;
      return buffer16[index];
    }
  this->process();
  return buffer16[index];
  
}

Fixed MonoMixer::tick_fixed()
{
  Fixed s_out;
  if (index+1<SAM)
    {
      index++;
      s_out=bufferfix[index];
      return s_out;
    }
  this->process_fixed();
  s_out=bufferfix[index];
  return s_out;
  
}

Fixed * MonoMixer::tick_fixed_buffer()
{
  this->process_fixed();
  return bufferfix;
}

/*
Fixed * MonoMixer::tick_fixed_buffer()
{  
  return M->tick_fixed_buffer();
}
*/
