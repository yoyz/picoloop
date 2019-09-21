#include "Machine.h"
#include "fixed.h"

#ifdef ENABLE_SYNTH_PBSYNTH
#include "Machine/PBSynth/PBSynthMachine.h"
#endif

#ifdef ENABLE_SYNTH_PICOSYNTH
#include "Machine/Picosynth/PicosynthMachine.h"
#endif

#ifdef ENABLE_SYNTH_PICODRUM
#include "Machine/Picodrum/PicodrumMachine.h"
#endif

#ifdef ENABLE_SYNTH_OPL2
#include "Machine/Dbopl/DboplMachine.h"
#endif

#ifdef ENABLE_SYNTH_LGPTSAMPLER
#include "Machine/Lgptsampler/LgptsamplerMachine.h"
#endif


#ifdef ENABLE_SYNTH_CURSYNTH
#include "Machine/Cursynth/CursynthMachine.h"
#endif

#ifdef ENABLE_SYNTH_OPEN303
#include "Machine/Open303/Open303Machine.h"
#endif

#ifdef ENABLE_SYNTH_TWYTCHSYNTH
#include "Machine/Twytch/TwytchsynthMachine.h"
#endif

#ifdef ENABLE_SYNTH_MDADRUM
#include "Machine/MDADrum/MDADrumMachine.h"
#endif

#ifdef ENABLE_SYNTH_SIDSYNTH
#include "Machine/SIDSynth/SIDSynthMachine.h"
#endif

#ifdef ENABLE_SYNTH_MIDIOUT
#include "Machine/MidiOutSystem/MidiOutMachine.h"
#endif

//#include "Effect.h"
#include "EffectDelay.h"
#include "EffectDisabled.h"
//#include <SDL/SDL_types.h>

#ifndef __MONOMIXER____
#define __MONOMIXER____

class MonoMixer
{
 public:
  MonoMixer();
  ~MonoMixer();
  void        init();
  void        setMachineType(int type);
  void        setAmplitude(int amp);
  //  void        setInput(Machine * M);
  void        setMonoMixerChannelNumber(int c);
  int         getMonoMixerChannelNumber();
  Machine *   getInput();
  Effect  *   getEffect();
  void        process();
  void        process_fixed(); 
  Sint16      tick();
  Fixed       tick_fixed();
  Fixed *     tick_fixed_buffer();
 private:
  int                   channel;
  
  Machine              *M;

#if defined ENABLE_SYNTH_PBSYNTH
  PBSynthMachine       * PBS;
#endif

#if defined ENABLE_SYNTH_PICOSYNTH
  PicosynthMachine     * PS;
#endif
#if defined ENABLE_SYNTH_PICODRUM
  PicodrumMachine      * PD;
#endif
#if defined ENABLE_SYNTH_OPL2
  dboplMachine         * OPLM;
#endif
  
#if defined ENABLE_SYNTH_LGPTSAMPLER
  LgptsamplerMachine   * LGPTSMPL;
#endif

#if defined ENABLE_SYNTH_CURSYNTH
  CursynthMachine      * CS;
#endif

#if defined ENABLE_SYNTH_OPEN303
  Open303Machine       * O303;
#endif

#if defined ENABLE_SYNTH_TWYTCHSYNTH
  TwytchsynthMachine   * TW;
#endif

#if defined ENABLE_SYNTH_MDADRUM
  MDADrumMachine       * MD;
#endif

#if defined ENABLE_SYNTH_SIDSYNTH
  SIDSynthMachine      * SS;
#endif


#if defined ENABLE_SYNTH_MIDIOUT
  MidiOutMachine       * MIDIOUTM;
#endif

  Effect                * FX;
  EffectDelay           * FXDelay;
  EffectDisabled        * FXDisabled;

  Sint16                amplitude;
  Fixed                 amplitudeFixed;
  int                   machine_type;

  int                   fx_depth;
  int                   fx_speed;

  int                   index;
  Sint16                * buffer16;
  Sint32                * buffer32;
  Fixed                 * bufferfix;
};


#endif
