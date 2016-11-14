#include "TwytchsynthUserInterface.h"




TwytchsynthUserInterface::TwytchsynthUserInterface()
{
}

TwytchsynthUserInterface::~TwytchsynthUserInterface()
{
}

void display_board_two_param(int machineParam1,int machineParam2);
void sub_handle_invert_trig();
int handle_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all);
void display_board_one_param_text(int machineParam1);
void display_board_two_param_text(int machineParam1,int machineParam2);
void display_board_two_param_number(int machineParam1,int machineParam2);
void display_board_trig();

void TwytchsynthUserInterface::handle_key(int menu)
{
  //DPRINTF("TwytchsynthUserInterface::handle_key(%d,%d)\n",menu);
  if (menu==GLOBALMENU_AD) 
    this->handle_key_amp_env();
  if (menu==GLOBALMENU_NOTE) 
    this->handle_key_note();
  if (menu==GLOBALMENU_OSC) 
    this->handle_key_osc();
  if (menu==GLOBALMENU_VCO) 
    this->handle_key_vco();
  if (menu==GLOBALMENU_LFO) 
    this->handle_key_lfo();
  if (menu==GLOBALMENU_FLTR) 
    this->handle_key_fltr();
}

void TwytchsynthUserInterface::handle_key_amp_env()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();
  
  
  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad       == MENU_AD_AMP_ATTACK_RELEASE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE, -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE,  1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,  -1, 0);

    }  


  // M_DS AMP
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_DECAY_SUSTAIN)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,    -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,     1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,  -1, 0);
    }  


  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_ATTACK_RELEASE)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE, -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE,  1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,  -1, 1);
    }  

  // GLOBALMENU_AD AMP
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_DECAY_SUSTAIN)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,  -1, 1);
    }  




  // GLOBALMENU_AD FLTR
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_ATTACK_RELEASE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE, -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE,  1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,  -1, 0);
    }  

  // GLOBALMENU_AD FLTR
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_DECAY_SUSTAIN)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,    -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,     1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,  -1, 0);
    }  



  // GLOBALMENU_AD FLTR
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_ATTACK_RELEASE)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE, -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE,  1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,  -1, 1);
    }  

  // GLOBALMENU_AD FLTR
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_DECAY_SUSTAIN)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,  -1, 1);
    }  




  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      if (lastKey   == BUTTON_A && 
	  lastEvent == KEYPRESSED)
	{
	  sub_handle_invert_trig();
	  // TK.invert_trig=1;
	  // DPRINTF("key lalt\n");      
	  // dirty_graphic=1;
	  // IE.clearLastKeyEvent();
	}
      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,  -1, 0);

    }  


  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,      1, 1);
      
      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,  -1, 1);
    }  



  // change GLOBALMENU_AD SUBMENU
  if (keyState[BUTTON_START]       &&
      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==127   &&
      menu_cursor ==  GLOBALMENU_AD)
    {
      menu_ad--;
      if (menu_ad<=-1)
	menu_ad=4;
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      menu_ad_dirty_keyboard=1;
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }

  // change GLOBALMENU_AD SUBMENU
  if (keyState[BUTTON_START] &&
      keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==127  &&
      menu_cursor ==  GLOBALMENU_AD)
    {
      menu_ad++;
      if (menu_ad>=4)
	menu_ad=0;
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      menu_ad_dirty_keyboard=1;
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }

  // change GLOBALMENU_AD SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_AD)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_ad==MENU_AD_AMP_ATTACK_RELEASE)       { menu_ad=MENU_AD_AMP_DECAY_SUSTAIN;        }
	  else if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)        { menu_ad=MENU_AD_FLTR_ATTACK_RELEASE;      }   
	  else if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE)      { menu_ad=MENU_AD_FLTR_DECAY_SUSTAIN;       }   
	  else if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN)       { menu_ad=MENU_AD_TRIGTIME_AMP;             }   
	  else if (menu_ad==MENU_AD_TRIGTIME_AMP)             { menu_ad=MENU_AD_AMP_ATTACK_RELEASE;       }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }




}

void TwytchsynthUserInterface::handle_key_note()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

    // GLOBALMENU_NOTE
  // change note
  // copy/paste
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {
      // copy/paste/insert/delete trig 
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , NOTE1,             -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , NOTE1,              1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , NOTE1           ,  12, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , NOTE1           , -12, 0);

    }  

    // GLOBALMENU_NOTE
  // change note
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {

      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , NOTE1,             -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , NOTE1,              1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , NOTE1           ,  12, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , NOTE1           , -12, 1);

    }  


  // change note from box to value e.g C3 D4...
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_NOTE)
    {
      if      (menu_note==0)        { menu_note=1;  }
      else if (menu_note==1)        { menu_note=0;  }   
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu note : %d]\n",menu_note);
    }


}

void TwytchsynthUserInterface::handle_key_osc()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        == MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC1OSC2)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,        -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,         1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC1OSC2)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,        -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,         1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,  -1, 1);
    }



  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        == MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC3)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC3_TYPE,        -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC3_TYPE,         1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC3)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC3_TYPE,        -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC3_TYPE,         1, 1);

    }





  // GLOBALMENU_OSC
  // change lfo one and two waveform
  if (menu        == MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_LFO1LFO2)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , LFO1_WAVEFORM,    -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , LFO1_WAVEFORM,     1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , LFO2_WAVEFORM  ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , LFO2_WAVEFORM  ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_LFO1LFO2)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , LFO1_WAVEFORM,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , LFO1_WAVEFORM,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , LFO2_WAVEFORM  ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , LFO2_WAVEFORM  ,  -1, 1);
    }


  // GLOBALMENU_OSC
  // change lfo one and two waveform
  if (menu        == MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_UNISON1UNISON2)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISON,    -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISON,     1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISON  ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISON  ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_UNISON1UNISON2)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISON,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISON,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISON  ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISON  ,  -1, 1);
    }



  // GLOBALMENU_OSC
  // change lfo one and two waveform
  if (menu        == MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_UNISONDT1UNISONDT2)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISONDT,    -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISONDT,     1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISONDT  ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISONDT  ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_UNISONDT1UNISONDT2)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISONDT,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST    , OSC1_UNISONDT,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISONDT  ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST    , OSC2_UNISONDT  ,  -1, 1);
    }

  // GLOBALMENU_OSC
  // change osc1 and osc2 scale
  if (menu        == MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC1SCALE_OSC2SCALE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,    -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,     1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE  ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE  ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC1SCALE_OSC2SCALE)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE  ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE  ,  -1, 1);
    }




  // change GLOBALMENU_OSC SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_OSC)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_osc==MENU_OSC_OSC1OSC2)            { menu_osc=MENU_OSC_OSC3;                }
	  else if (menu_osc==MENU_OSC_OSC3)                { menu_osc=MENU_OSC_LFO1LFO2;            }
	  else if (menu_osc==MENU_OSC_LFO1LFO2)            { menu_osc=MENU_OSC_UNISON1UNISON2;      }
	  else if (menu_osc==MENU_OSC_UNISON1UNISON2)      { menu_osc=MENU_OSC_UNISONDT1UNISONDT2;  }
	  else if (menu_osc==MENU_OSC_UNISONDT1UNISONDT2)  { menu_osc=MENU_OSC_OSC1SCALE_OSC2SCALE; }
	  else if (menu_osc==MENU_OSC_OSC1SCALE_OSC2SCALE) { menu_osc=MENU_OSC_OSC1OSC2;            }
	  else                                             { menu_osc=MENU_OSC_OSC1OSC2;            }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_osc);
    }
}


void TwytchsynthUserInterface::handle_key_vco()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();




  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSCMIX_VELOCITY
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, VELOCITY          ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, VELOCITY        ,  -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCMIX_VELOCITY
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, VELOCITY     ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, VELOCITY     ,     -1, 1);
    }


  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_KEYTRACKENVDEPTH
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, KEYTRACK        ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, KEYTRACK        ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH        ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH        ,     -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_KEYTRACKENVDEPTH
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, KEYTRACK        ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, KEYTRACK        ,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH        ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH        ,     -1, 1);
    }


  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSCDETUNECROSSMOD
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE        ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE        ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_MOD           ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_MOD           ,     -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCDETUNECROSSMOD
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE        ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE        ,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_MOD           ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_MOD           ,     -1, 1);
    }


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSC3AMPOSC4AMP
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC3_AMP        ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC3_AMP        ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC4_AMP        ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC4_AMP        ,     -1, 0);
    }


  // VCO Menu
  // Change Value
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSC3AMPOSC4AMP
      )
    {     
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC3_AMP           ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC3_AMP           ,      1, 1);
 
      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC4_AMP           ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC4_AMP           ,     -1, 1);
    }







  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_VCO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_vco==MENU_VCO_OSCMIX_VELOCITY)     { menu_vco=MENU_VCO_KEYTRACKENVDEPTH;  }
	  else if (menu_vco==MENU_VCO_KEYTRACKENVDEPTH)    { menu_vco=MENU_VCO_OSCDETUNECROSSMOD; }
	  else if (menu_vco==MENU_VCO_OSCDETUNECROSSMOD)   { menu_vco=MENU_VCO_OSC3AMPOSC4AMP;   }
	  else if (menu_vco==MENU_VCO_OSC3AMPOSC4AMP)      { menu_vco=MENU_VCO_OSCMIX_VELOCITY;   }
	  else                                             { menu_vco=MENU_VCO_OSCMIX_VELOCITY;   }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu vco : %d]\n",menu_vco);
    }



}


void TwytchsynthUserInterface::handle_key_lfo()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();

  // GLOBALMENU_LFO
  // LFO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_ENV_AMOUNT,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_ENV_AMOUNT,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,     -1, 0);
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {

      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_ENV_AMOUNT,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_ENV_AMOUNT,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,     -1, 1);
    }

  // GLOBALMENU_LFO
  // LFO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_ENV_AMOUNT,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO2_ENV_AMOUNT,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ,     -1, 0);
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_ENV_AMOUNT,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO2_ENV_AMOUNT,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ      ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ      ,     -1, 1);
    }


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {
      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,     -1, 0);
    }



  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,     -1, 1);
    }



  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_LFO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_lfo==MENU_LFO_LFOPITCH)               { menu_lfo=MENU_LFO_PITCHBEND;       }
	  else if (menu_lfo==MENU_LFO_PITCHBEND)              { menu_lfo=MENU_LFO_LFOPITCH;        }
	  else                                                  menu_lfo=MENU_LFO_LFOPITCH; 
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu lfo : %d]\n",menu_lfo);
    }



}



void TwytchsynthUserInterface::handle_key_fltr()
{
  mapii keyState=IE.keyState();
  mapii keyRepeat=IE.keyRepeat();
  int    lastEvent=IE.lastEvent();
  int    lastKey=IE.lastKey();


  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,    -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,     1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,        1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,       -1, 0);
    }  

  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,    -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,     1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,        1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,       -1, 1);
    }  

  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,             -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,              1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,  -1, 0);
    }  


  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      //sub_handle_invert_trig();

      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,             -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,              1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,  -1, 1);
    }  




  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_SATURATION_FEEDBACK)
    {
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_SATURATION,       -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_SATURATION,        1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_FEEDBACK       ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_FEEDBACK       ,  -1, 0);
    }  


  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_SATURATION_FEEDBACK)
    {
      //sub_handle_invert_trig();

      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_SATURATION,       -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_SATURATION,        1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_FEEDBACK       ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST    , FILTER1_FEEDBACK       ,  -1, 1);
    }  









  // change GLOBALMENU_FLTR SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_FLTR)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)   { menu_fltr=MENU_FLTR_ALGO_TYPE;            }
	  else if (menu_fltr==MENU_FLTR_ALGO_TYPE)          { menu_fltr=MENU_FLTR_SATURATION_FEEDBACK;  }   
	  else if (menu_fltr==MENU_FLTR_SATURATION_FEEDBACK){ menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;     }   
	  else                                              { menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;     }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu_fltr : %d]\n",menu_fltr);
    }


}


void TwytchsynthUserInterface::display_board_text()
{
  int  i;
  char str_up[64];
  char str_down[64];
  char str_divider[64];
  char str_submenu[64];

  int  right_x_display_offset=       COLLUMN40;
  int  right_y_display_offset_line1= LINE02;
  int  right_y_display_offset_line2= LINE03;
  int  right_y_display_offset_line3= LINE04;
  int  right_y_display_offset_line4= LINE05;
  int  right_y_display_offset_line5= LINE06;
  int  right_y_display_offset_line6= LINE07;

  int  menu_x_display_offset=       COLLUMN01;
  int  menu_y_display_offset=       LINE25;

  int  cty=SEQ.getCurrentTrackY();
  int  stepdiv=SEQ.getPatternSequencer(cty).getBPMDivider();


  if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE &&
      menu_cursor==GLOBALMENU_AD) 
    sprintf(str_submenu,"AMP  A/R");

  if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    sprintf(str_submenu,"AMP  S/D");



  if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE &&
      menu_cursor==GLOBALMENU_AD)
    sprintf(str_submenu,"FLTR A/R");

  if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    sprintf(str_submenu,"FLTR S/D");



  if (menu_ad==MENU_AD_TRIGTIME_AMP &&
      menu_cursor==GLOBALMENU_AD)    
    sprintf(str_submenu,"T/N AMP");
    

  if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE &&
      menu_cursor==GLOBALMENU_FLTR)
    sprintf(str_submenu,"CUTOFF/RES");

  if (menu_fltr==MENU_FLTR_SATURATION_FEEDBACK &&
      menu_cursor==GLOBALMENU_FLTR)
    sprintf(str_submenu,"FLTR SATURATION/FEEDBACK");



  if (menu_fltr==MENU_FLTR_ALGO_TYPE &&
      menu_cursor==GLOBALMENU_FLTR
      )
    sprintf(str_submenu,"ALGO/TYPE");


  if (menu_vco==MENU_VCO_OSCAMP &&
      menu_cursor==GLOBALMENU_VCO
      )
    sprintf(str_submenu,"envdepth/keytrack");

  if (menu_vco==MENU_VCO_OSCMIX_PHASE &&
      menu_cursor==GLOBALMENU_VCO
      )
    sprintf(str_submenu,"Velocity/VCOMIX");


  if (menu_vco==MENU_VCO_OSCDETUNECROSSMOD &&
      menu_cursor==GLOBALMENU_VCO
      )
    sprintf(str_submenu,"DETUNE/Crossmod");

  if (menu_vco==MENU_VCO_OSC3AMPOSC4AMP &&
      menu_cursor==GLOBALMENU_VCO
      )
    sprintf(str_submenu,"SUB/NOISE");



  if (menu_lfo==MENU_LFO_LFOPITCH &&
      menu_cursor==GLOBALMENU_LFO
      )
    sprintf(str_submenu,"LFO1 Amount/Speed");

  if (menu_lfo==MENU_LFO_PITCHBEND &&
      menu_cursor==GLOBALMENU_LFO
      )
    sprintf(str_submenu,"LFO2 Amount/Speed");


  if (menu_lfo==MENU_LFO_TYPE &&
      menu_cursor==GLOBALMENU_LFO
      )
    sprintf(str_submenu,"LFOType");

  if (menu_osc    == MENU_OSC_OSC1OSC2 &&
      menu_cursor == GLOBALMENU_OSC)
    sprintf(str_submenu,"OSC1&2 WAVEFORM");

  if (menu_osc    == MENU_OSC_OSC3 &&
      menu_cursor == GLOBALMENU_OSC)
    sprintf(str_submenu,"SUBOSC WAVEFORM");


  if (menu_osc    == MENU_OSC_LFO1LFO2 &&
      menu_cursor == GLOBALMENU_OSC)
    sprintf(str_submenu,"LFO WAVEFORM  ");


  if (menu_osc    == MENU_OSC_UNISON1UNISON2 &&
      menu_cursor == GLOBALMENU_OSC)
      sprintf(str_submenu,"UNISON OSC1OSC2  ");

  if (menu_osc    == MENU_OSC_UNISONDT1UNISONDT2 &&
      menu_cursor == GLOBALMENU_OSC)
      sprintf(str_submenu,"UNISONDT OSC1OSC2  ");


  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line2,str_submenu);
  


  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_AD)    sprintf(str_down,"[A/R] Note  OSC   VCO   LFO   FLTR   ",cty);  
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_NOTE)  sprintf(str_down," A/R [Note] OSC   VCO   LFO   FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_OSC)   sprintf(str_down," A/R  Note [OSC]  VCO   LFO   FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_VCO)   sprintf(str_down," A/R  Note  OSC  [VCO]  LFO   FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_LFO)   sprintf(str_down," A/R  Note  OSC   VCO  [LFO]  FLTR   ",cty);
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_FLTR)  sprintf(str_down," A/R  Note  OSC   VCO   LFO  [FLTR]  ",cty);


  if (menu==0)                         sprintf(str_down,"                     ",cty);

  if (menu_cursor==GLOBALMENU_AD)               sprintf(str_up,"A/R     ");
  if (menu_cursor==GLOBALMENU_NOTE)             sprintf(str_up,"Note    ");
  if (menu_cursor==GLOBALMENU_OSC)              sprintf(str_up,"OSC     ");
  if (menu_cursor==GLOBALMENU_VCO)              sprintf(str_up,"VCO     ");
  if (menu_cursor==GLOBALMENU_LFO)              sprintf(str_up,"LFO     ");
  if (menu_cursor==GLOBALMENU_FLTR)             sprintf(str_up,"FLTR    ");

  // if (menu==MENU_ON_PAGE1)
  //   SG.guiTTFText(right_x_display_offset,
  // 		  right_y_display_offset_line4,str_up);

  if (menu==MENU_ON_PAGE1)
    SG.guiTTFText(menu_x_display_offset,
		  menu_y_display_offset,str_down);

}





void TwytchsynthUserInterface::display_board(int menu)
{
  DPRINTF("TwytchsynthUserInterface::display_board(%d,)\n",menu);
  if (menu==GLOBALMENU_AD) 
    this->display_board_amp_env();
  if (menu==GLOBALMENU_NOTE) 
    this->display_board_note();
  if (menu==GLOBALMENU_VCO) 
    this->display_board_vco();
  if (menu==GLOBALMENU_OSC) 
    this->display_board_osc();
  if (menu==GLOBALMENU_LFO) 
    this->display_board_lfo();
  if (menu==GLOBALMENU_FLTR) 
    this->display_board_fltr();

}


void TwytchsynthUserInterface::display_board_amp_env()
{
  DPRINTF("TwytchsynthUserInterface::display_board_amp_env()\n");
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  // Attack/Release
  if (menu_cursor==GLOBALMENU_AD)
    {

      if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE)
	display_board_two_param(ADSR_AMP_RELEASE,ADSR_AMP_ATTACK);

      if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)
	display_board_two_param(ADSR_AMP_DECAY,ADSR_AMP_SUSTAIN);

      if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE)
	display_board_two_param(ADSR_FLTR_RELEASE,ADSR_FLTR_ATTACK);

      if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN)
	display_board_two_param(ADSR_FLTR_DECAY,ADSR_FLTR_SUSTAIN);

      if (menu_ad==MENU_AD_TRIGTIME_AMP)
	display_board_two_param(AMP,TRIG_TIME_DURATION);	
    }  
}




void TwytchsynthUserInterface::display_board_note()
{
  int  i;
  int  j;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  NoteFreq & NF = NoteFreq::getInstance();

  display_board_trig();

  if (menu_cursor==GLOBALMENU_NOTE)
    {
      // Note C3 
      if (menu_note==ENABLE)
	{	  
	  for (i=0;i<16;i++)
	    {
	      j=i+pattern_display_offset[cty];

	      if (P[cty].getPatternElement(j).get(NOTE_ON))
		SG.drawTTFTextNumberFirstLine(i,NF.getNoteCharStar(P[cty].getPatternElement(j).get(NOTE1)));
	    }
	}
      // Note Cursor
      if (menu_note==DISABLE)      
	{
	  for (i=0;i<16;i++)
	    {
	      j=i+pattern_display_offset[cty];

	      if (P[cty].getPatternElement(j).get(NOTE_ON))
		SG.smallBoxNumber(i,
				  (P[cty].getPatternElement(j).get(NOTE1)%12-1)*10,
				  (128-(P[cty].getPatternElement(j).get(NOTE1)/12)*10),
				  SMALLBOX_COLOR);
	    }
	}
    }  

}


void TwytchsynthUserInterface::display_board_vco()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCMIX_VELOCITY)
    {
      display_board_two_param(VCO_MIX,VELOCITY);
    }

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_KEYTRACKENVDEPTH)
    {
       display_board_two_param(KEYTRACK,ENV1_DEPTH);
    }


  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCDETUNECROSSMOD)
    {
       display_board_two_param(OSC1_DETUNE,OSC1_MOD);
    }

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSC3AMPOSC4AMP)
    {
       display_board_two_param(OSC3_AMP,OSC4_AMP);
    }


  
}

void TwytchsynthUserInterface::display_board_osc()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor == GLOBALMENU_OSC    &&
      menu_osc    == MENU_OSC_OSC1OSC2)
    {
      display_board_two_param_text(OSC1_TYPE,OSC2_TYPE);
    }
  if (menu_cursor == GLOBALMENU_OSC    &&
      menu_osc    == MENU_OSC_OSC3)
    {
      display_board_one_param_text(OSC3_TYPE);
    }
  if (menu_cursor == GLOBALMENU_OSC    &&
      menu_osc    == MENU_OSC_LFO1LFO2)
    {
      display_board_two_param_text(LFO1_WAVEFORM,LFO2_WAVEFORM);
    }

  if (menu_cursor == GLOBALMENU_OSC    &&
      menu_osc    == MENU_OSC_UNISON1UNISON2)
    {
      display_board_two_param(OSC1_UNISON,OSC2_UNISON);
    }

  if (menu_cursor == GLOBALMENU_OSC    &&
      menu_osc    == MENU_OSC_UNISONDT1UNISONDT2)
    {
      display_board_two_param(OSC1_UNISONDT,OSC2_UNISONDT);
    }

  if (menu_cursor == GLOBALMENU_OSC    &&
      menu_osc    == MENU_OSC_OSC1SCALE_OSC2SCALE)
    {
      display_board_two_param_number(OSC1_SCALE,OSC2_SCALE);
    }


}


void TwytchsynthUserInterface::display_board_lfo()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // LFOPITCH

  if (menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH)
    {
      display_board_two_param(LFO1_ENV_AMOUNT,LFO1_FREQ);
    }

  // PITCHBEND

  if (menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND)
    {
      display_board_two_param(LFO2_ENV_AMOUNT,LFO2_FREQ);
    }

  if (menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE)
    {
      display_board_one_param_text(LFO_TYPE);
    }
}


void TwytchsynthUserInterface::display_board_fltr()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_FLTR)
    {

      if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)
	{
	  display_board_two_param(FILTER1_RESONANCE,FILTER1_CUTOFF);
	}
      if (menu_fltr==MENU_FLTR_ALGO_TYPE)
	{
	  display_board_one_param_text(FILTER1_TYPE);
	}
      if (menu_fltr==MENU_FLTR_SATURATION_FEEDBACK)
	{
	  display_board_two_param(FILTER1_SATURATION,FILTER1_FEEDBACK);
	}

    }
}

