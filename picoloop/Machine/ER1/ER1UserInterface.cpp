#include "ER1UserInterface.h"

#include <cstdio>

// Defined in PatternPlayer.cpp, shared by every machine UI.
void display_board_two_param(int machineParam1,int machineParam2);
void display_board_one_param_dot(int machineParam1);
void display_board_one_param_text(int machineParam1);
void display_board_two_param_text(int machineParam1,int machineParam2);
void display_board_two_param_number(int machineParam1,int machineParam2);
void display_board_one_param_number(int machineParam1);
void helper_handle_key_two_button(int menu_cursor_tc,int menu_ad_tc,
                                  int key_repeat_interval,
                                  int param1,int param2,
                                  int inc_param1,int inc_param2);
void helper_handle_key_one_button(int menu_cursor_tc,int menu_ad_tc,
                                  int key_repeat_interval,
                                  int param1,int inc_param1);
void helper_change_sub_menu(int nb_menu);

#define ER1_KEY_REPEAT KEY_REPEAT_INTERVAL_SMALLEST

ER1UserInterface::ER1UserInterface()
{
}

ER1UserInterface::~ER1UserInterface()
{
}

void ER1UserInterface::handle_key(int menu)
{
  if (menu==GLOBALMENU_AD)   this->handle_key_env();
  if (menu==GLOBALMENU_NOTE) this->handle_key_note();
  if (menu==GLOBALMENU_OSC)  this->handle_key_osc();
  if (menu==GLOBALMENU_VCO)  this->handle_key_vco();
  if (menu==GLOBALMENU_LFO)  this->handle_key_lfo();
  if (menu==GLOBALMENU_FLTR) this->handle_key_fltr();
}

void ER1UserInterface::handle_key_env()
{
  // Decay / Level
  helper_handle_key_two_button(GLOBALMENU_AD,MENU_PAGE0_SUB0,ER1_KEY_REPEAT,
                               ADSR_AMP_DECAY,OSC1_AMP,1,1);

  // Noise decay / Noise level
  helper_handle_key_two_button(GLOBALMENU_AD,MENU_PAGE0_SUB1,ER1_KEY_REPEAT,
                               OSC4_AMP,OSC3_AMP,1,1);

  helper_change_sub_menu(MENU_PAGE0_SUB1);
}

void ER1UserInterface::handle_key_note()
{
  // LEFT/RIGHT: one semitone; UP/DOWN: one octave (like the other engines)
  helper_handle_key_two_button(GLOBALMENU_NOTE,MENU_PAGE0_SUB0,
                               KEY_REPEAT_INTERVAL_LONG,
                               NOTE1,NOTE1,1,12);

  helper_change_sub_menu(MENU_PAGE0_SUB0);
}

void ER1UserInterface::handle_key_osc()
{
  // Wave (discrete: slow repeat like the other machine type selectors)
  helper_handle_key_one_button(GLOBALMENU_OSC,MENU_PAGE0_SUB0,
                               KEY_REPEAT_INTERVAL_LONG,
                               OSC1_TYPE,1);

  // Mod span (X) / Wave warp (Y)
  helper_handle_key_two_button(GLOBALMENU_OSC,MENU_PAGE0_SUB1,ER1_KEY_REPEAT,
                               OSC2_PHASE,ENV1_DEPTH,1,1);

  // Detune
  helper_handle_key_one_button(GLOBALMENU_OSC,MENU_PAGE0_SUB2,ER1_KEY_REPEAT,
                               OSC1_DETUNE,1);

  helper_change_sub_menu(MENU_PAGE0_SUB2);
}

void ER1UserInterface::handle_key_vco()
{
  // Detune / Velocity
  helper_handle_key_two_button(GLOBALMENU_VCO,MENU_PAGE0_SUB0,ER1_KEY_REPEAT,
                               OSC1_DETUNE,VELOCITY,1,1);

  helper_change_sub_menu(MENU_PAGE0_SUB0);
}

void ER1UserInterface::handle_key_lfo()
{
  // Modulation type (discrete: slow repeat like the OSC wave selector)
  helper_handle_key_one_button(GLOBALMENU_LFO,MENU_PAGE0_SUB0,
                               KEY_REPEAT_INTERVAL_LONG,
                               LFO1_WAVEFORM,1);

  // Mod depth / Mod speed
  helper_handle_key_two_button(GLOBALMENU_LFO,MENU_PAGE0_SUB1,ER1_KEY_REPEAT,
                               PITCHBEND_DEPTH,LFO1_FREQ,1,1);

  // Sweeps / Phase
  helper_handle_key_two_button(GLOBALMENU_LFO,MENU_PAGE0_SUB2,ER1_KEY_REPEAT,
                               LFO1_ENV_AMOUNT,OSC1_PHASE,1,1);

  helper_change_sub_menu(MENU_PAGE0_SUB2);
}

void ER1UserInterface::handle_key_fltr()
{
  // Resonance / Cutoff (X / Y, same order as the other engines)
  helper_handle_key_two_button(GLOBALMENU_FLTR,MENU_PAGE0_SUB0,ER1_KEY_REPEAT,
                               FILTER1_RESONANCE,FILTER1_CUTOFF,1,1);

  // Filter type / filter position (both discrete: slow repeat)
  helper_handle_key_two_button(GLOBALMENU_FLTR,MENU_PAGE0_SUB1,
                               KEY_REPEAT_INTERVAL_LONG,
                               FILTER1_TYPE,FILTER1_ALGO,1,1);

  // Low Boost / Key Track
  helper_handle_key_two_button(GLOBALMENU_FLTR,MENU_PAGE0_SUB2,ER1_KEY_REPEAT,
                               VCO_MIX,FILTER1_SATURATION,1,1);

  // Bit depth (X only): far left = no crush, far right = maximum crush
  helper_handle_key_one_button(GLOBALMENU_FLTR,MENU_PAGE0_SUB3,ER1_KEY_REPEAT,
                               OSC1_UNISON,1);

  helper_change_sub_menu(MENU_PAGE0_SUB3);
}

void ER1UserInterface::display_board(int menu)
{
  if (menu==GLOBALMENU_AD)   this->display_board_env();
  if (menu==GLOBALMENU_NOTE) this->display_board_note();
  if (menu==GLOBALMENU_OSC)  this->display_board_osc();
  if (menu==GLOBALMENU_VCO)  this->display_board_vco();
  if (menu==GLOBALMENU_LFO)  this->display_board_lfo();
  if (menu==GLOBALMENU_FLTR) this->display_board_fltr();
}

void ER1UserInterface::display_board_env()
{
  if (menu_sub>MENU_PAGE0_SUB1) menu_sub=MENU_PAGE0_SUB0;

  if (menu_sub==MENU_PAGE0_SUB0) display_board_two_param(ADSR_AMP_DECAY,OSC1_AMP);
  if (menu_sub==MENU_PAGE0_SUB1) display_board_two_param(OSC4_AMP,OSC3_AMP);
}

void ER1UserInterface::display_board_note()
{
  if (menu_sub>MENU_PAGE0_SUB0) menu_sub=MENU_PAGE0_SUB0;

  if (menu_sub==MENU_PAGE0_SUB0) display_board_one_param_number(NOTE1);
}

void ER1UserInterface::display_board_osc()
{
  if (menu_sub>MENU_PAGE0_SUB2) menu_sub=MENU_PAGE0_SUB0;

  if (menu_sub==MENU_PAGE0_SUB0) display_board_one_param_text(OSC1_TYPE);
  if (menu_sub==MENU_PAGE0_SUB1) display_board_two_param(OSC2_PHASE,ENV1_DEPTH);
  if (menu_sub==MENU_PAGE0_SUB2) display_board_one_param_dot(OSC1_DETUNE);
}

void ER1UserInterface::display_board_vco()
{
  if (menu_sub>MENU_PAGE0_SUB0) menu_sub=MENU_PAGE0_SUB0;

  if (menu_sub==MENU_PAGE0_SUB0) display_board_two_param(OSC1_DETUNE,VELOCITY);
}

void ER1UserInterface::display_board_lfo()
{
  if (menu_sub>MENU_PAGE0_SUB2) menu_sub=MENU_PAGE0_SUB0;

  if (menu_sub==MENU_PAGE0_SUB0) display_board_one_param_text(LFO1_WAVEFORM);
  if (menu_sub==MENU_PAGE0_SUB1) display_board_two_param(PITCHBEND_DEPTH,LFO1_FREQ);
  if (menu_sub==MENU_PAGE0_SUB2) display_board_two_param(LFO1_ENV_AMOUNT,OSC1_PHASE);
}

void ER1UserInterface::display_board_fltr()
{
  if (menu_sub>MENU_PAGE0_SUB3) menu_sub=MENU_PAGE0_SUB0;

  if (menu_sub==MENU_PAGE0_SUB0) display_board_two_param(FILTER1_RESONANCE,FILTER1_CUTOFF);
  if (menu_sub==MENU_PAGE0_SUB1) display_board_two_param_text(FILTER1_TYPE,FILTER1_ALGO);
  if (menu_sub==MENU_PAGE0_SUB2) display_board_two_param(VCO_MIX,FILTER1_SATURATION);
  if (menu_sub==MENU_PAGE0_SUB3) display_board_one_param_dot(OSC1_UNISON);
}

void ER1UserInterface::display_board_text()
{
  char str_line2[64]="";
  char str_menu[64]="";

  int right_x_display_offset=       COLLUMN40;
  int right_y_display_offset_line2= LINE03;
  int menu_x_display_offset=        COLLUMN01;
  int menu_y_display_offset=        LINE25;

  if (menu_cursor==GLOBALMENU_AD)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,"LEVEL/DECAY");
      if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line2,"NOISE/NDECAY");
    }
  if (menu_cursor==GLOBALMENU_NOTE)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,"OCT/NOTE");
    }
  if (menu_cursor==GLOBALMENU_OSC)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,"WAVE");
      if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line2,"WARP/SPAN");
      if (menu_sub==MENU_PAGE0_SUB2) sprintf(str_line2,"DETUNE");
    }
  if (menu_cursor==GLOBALMENU_VCO)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,"VELOCITY/DETUNE");
    }
  if (menu_cursor==GLOBALMENU_LFO)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,"MOD TYPE");
      if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line2,"SPEED/DEPTH");
      if (menu_sub==MENU_PAGE0_SUB2) sprintf(str_line2,"PHASE/SWEEP");
    }
  if (menu_cursor==GLOBALMENU_FLTR)
    {
      if (menu_sub==MENU_PAGE0_SUB0) sprintf(str_line2,"CUTOFF/RES");
      if (menu_sub==MENU_PAGE0_SUB1) sprintf(str_line2,"POS/TYPE");
      if (menu_sub==MENU_PAGE0_SUB2) sprintf(str_line2,"KEYTRACK/BOOST");
      if (menu_sub==MENU_PAGE0_SUB3) sprintf(str_line2,"BIT");
    }

  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_AD)    sprintf(str_menu,"[ENV]  Note  OSC   VCO   LFO   FLTR");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_NOTE)  sprintf(str_menu," ENV  [Note] OSC   VCO   LFO   FLTR");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_OSC)   sprintf(str_menu," ENV   Note [OSC]  VCO   LFO   FLTR");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_VCO)   sprintf(str_menu," ENV   Note  OSC  [VCO]  LFO   FLTR");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_LFO)   sprintf(str_menu," ENV   Note  OSC   VCO  [LFO]  FLTR");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_FLTR)  sprintf(str_menu," ENV   Note  OSC   VCO   LFO  [FLTR]");

  if (menu==0) sprintf(str_menu,"                                    ");

  SG.guiTTFText(right_x_display_offset,right_y_display_offset_line2,str_line2);

  if (menu==MENU_ON_PAGE1)
    SG.guiTTFText(menu_x_display_offset,menu_y_display_offset,str_menu);
}
