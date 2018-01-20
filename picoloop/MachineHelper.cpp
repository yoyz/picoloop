#include "MachineHelper.h"

const char * getMachineTypeCharStar(int machine_type)
{
  static const char * str_null        = "NULL   ";
  static const char * str_picosynth   = "PSYNTH ";
  static const char * str_picodrum    = "PDRUM  ";
  static const char * str_opl2        = "OPL2   ";
  static const char * str_pbsynth     = "PBS    ";
  static const char * str_cursynth    = "CSYNTH ";
  static const char * str_open303     = "O303   ";
  static const char * str_twytchsynth = "TSYNTH ";
  static const char * str_midiout     = "MDIOUT";
  static const char * str_mdadrum     = "MDRUM";
  static const char * str_sidsynth    = "SID    ";
  static const char * str_lgptsampler = "LGPT   ";

  switch (machine_type)
    {
    case SYNTH_PICOSYNTH:  return str_picosynth;   break;
    case SYNTH_PICODRUM:   return str_picodrum;    break;
    case SYNTH_OPL2:       return str_opl2;        break; 
    case SYNTH_PBSYNTH:    return str_pbsynth;     break; 
    case SYNTH_CURSYNTH:   return str_cursynth;    break; 
    case SYNTH_OPEN303:    return str_open303;     break;
    case SYNTH_TWYTCHSYNTH:return str_twytchsynth; break; 
    case SYNTH_MIDIOUT:    return str_midiout;     break; 
    case SYNTH_MDADRUM:    return str_mdadrum;     break;
    case SYNTH_SIDSYNTH:   return str_sidsynth;    break;
    case SYNTH_LGPTSAMPLER: return str_lgptsampler; break; 
    }
  return str_null;
}
