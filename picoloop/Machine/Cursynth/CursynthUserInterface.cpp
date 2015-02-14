#include "CursynthUserInterface.h"




CursynthUserInterface::CursynthUserInterface()
{
}

CursynthUserInterface::~CursynthUserInterface()
{
}

void display_board_two_param(int machineParam1,int machineParam2);
void sub_handle_invert_trig();
int handle_tweakable_knob_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all);
void display_board_one_param_text(int machineParam1);
void display_board_two_param_text(int machineParam1,int machineParam2);


void CursynthUserInterface::handle_key(int menu)
{
  //printf("CursynthUserInterface::handle_key(%d,%d)\n",menu);
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

void CursynthUserInterface::handle_key_amp_env()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;
  
  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();
  
  
  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad       == MENU_AD_AMP_ATTACK_RELEASE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE, -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE,  1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,  -1, 0);

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

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,    -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,     1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,  -1, 0);
    }  


  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_ATTACK_RELEASE)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE, -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE,  1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,  -1, 1);
    }  

  // GLOBALMENU_AD AMP
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_DECAY_SUSTAIN)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,    -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,     1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,  -1, 1);
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

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE, -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE,  1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,  -1, 0);
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

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,    -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,     1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,  -1, 0);
    }  



  // GLOBALMENU_AD FLTR
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_ATTACK_RELEASE)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE, -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_RELEASE,  1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_ATTACK,  -1, 1);
    }  

  // GLOBALMENU_AD FLTR
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_FLTR_DECAY_SUSTAIN)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,    -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_DECAY,     1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_FLTR_SUSTAIN,  -1, 1);
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
	  // printf("key lalt\n");      
	  // dirty_graphic=1;
	  // IE.clearLastKeyEvent();
	}
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,  -1, 0);

    }  


  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,      1, 1);
      
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,  -1, 1);
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
      printf("[sub menu env : %d]\n",menu_ad);
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
      printf("[sub menu env : %d]\n",menu_ad);
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
	  else if (menu_ad==MENU_AD_TRIGTIME_AMP)             { menu_ad=MENU_AD_AMP_ATTACK_RELEASE;      }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu env : %d]\n",menu_ad);
    }




}

void CursynthUserInterface::handle_key_note()
{

  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

    // GLOBALMENU_NOTE
  // change note
  // copy/paste
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {
      // copy/paste/insert/delete trig 
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , NOTE,             -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , NOTE,              1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , NOTE           ,  12, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , NOTE           , -12, 0);

    }  

    // GLOBALMENU_NOTE
  // change note
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , NOTE,             -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , NOTE,              1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , NOTE           ,  12, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , NOTE           , -12, 1);

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
      printf("[sub menu note : %d]\n",menu_note);
    }


}

void CursynthUserInterface::handle_key_osc()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,        -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,         1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,        -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,         1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,  -1, 1);

    }

}


void CursynthUserInterface::handle_key_vco()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();




  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_PHASE        ,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_PHASE        ,  -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_PHASE     ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_PHASE     ,     -1, 1);
    }


  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSCAMP
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_AMP        ,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC1_AMP        ,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP        ,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP        ,     -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCAMP
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_AMP        ,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC1_AMP        ,      1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP        ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP        ,     -1, 1);
    }


  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_FMTYPE
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     FM_TYPE        ,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     FM_TYPE        ,     -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_FMTYPE
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     FM_TYPE        ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     FM_TYPE        ,     -1, 1);
    }



  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_VCO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_vco==MENU_VCO_FMTYPE)              { menu_vco=MENU_VCO_OSCAMP;            }
	  else if (menu_vco==MENU_VCO_OSCAMP)              { menu_vco=MENU_VCO_OSCMIX_PHASE;      }   
	  else if (menu_vco==MENU_VCO_OSCMIX_PHASE)        { menu_vco=MENU_VCO_FMTYPE;            }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu env : %d]\n",menu_ad);
    }



}


void CursynthUserInterface::handle_key_lfo()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

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

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,     -1, 0);
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,      1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,     -1, 1);
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

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,     -1, 0);
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,      1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,     -1, 1);
    }


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,     -1, 0);
    }



  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,     -1, 1);
    }



  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_LFO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_lfo==MENU_LFO_LFOPITCH)               { menu_lfo=MENU_LFO_PITCHBEND;       }
	  else if (menu_lfo==MENU_LFO_PITCHBEND)              { menu_lfo=MENU_LFO_TYPE;            } 
	  else if (menu_lfo==MENU_LFO_TYPE)                   { menu_lfo=MENU_LFO_LFOPITCH;        }  
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu lfo : %d]\n",menu_lfo);
    }



}



void CursynthUserInterface::handle_key_fltr()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,    -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,     1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,        1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,       -1, 0);
    }  

  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,    -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,     1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,        1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,       -1, 1);
    }  

  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,             -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,              1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,  -1, 0);
    }  


  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      //sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,             -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,              1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,  -1, 1);
    }  


  // change GLOBALMENU_FLTR SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_FLTR)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)   { menu_fltr=MENU_FLTR_ALGO_TYPE;            }
	  else if (menu_fltr==MENU_FLTR_ALGO_TYPE)          { menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;     }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      printf("[sub menu_fltr : %d]\n",menu_fltr);
    }


}




void CursynthUserInterface::display_board(int menu)
{
  printf("UserInterface::display_board(%d,)\n",menu);
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


void CursynthUserInterface::display_board_amp_env()
{
  printf("UserInterface::display_board_amp_env()\n");
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




void CursynthUserInterface::display_board_note()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  NoteFreq & NF = NoteFreq::getInstance();
  // Note
  if (menu_cursor==GLOBALMENU_NOTE)
    {

      if (menu_note==DISABLE)
	{

	  SG.drawBoxNumber(cursor,CURSOR_COLOR);	  
	  SG.drawBoxNumber(step,STEP_COLOR);  
    
	  /*
	  SG.smallBoxNumber(cursor,
			    (P[cty].getPatternElement(cursor).getNote()%12)*10,
			    (P[cty].getPatternElement(cursor).getNote()/12)*10,
			    SMALLBOX_COLOR);
	  SG.smallBoxNumber(step,  
			    (P[cty].getPatternElement(step).getNote()%12)*10,
			    (P[cty].getPatternElement(step).getNote()/12)*10,
			    SMALLBOX_COLOR);
	  */
	}
      // Note C3 
      if (menu_note==ENABLE)
	{	  
	  SG.drawBoxNumber(step,STEP_COLOR);  
	  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  
	  SG.drawBoxNumber(cursor,CURSOR_COLOR);

	  for (i=0;i<16;i++)
	    {
	      // Draw trig note color
	      if (P[cty].getPatternElement(i).get(NOTE_ON))
		{
		  SG.drawBoxNumber(i,NOTE_COLOR);
		  
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  
		    //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);
		  //  SG.drawTTFTextNumberFirstLine(i,P[cty].getPatternElement(i).getNoteCharStar());
		  SG.drawTTFTextNumberFirstLine(i,NF.getNoteCharStar(P[cty].getPatternElement(i).get(NOTE)));
		}
	      //      if (P[cty].getPatternElement(i).getTrig())
		
	    }
	}
      // Note Cursor
      if (menu_note==DISABLE)      
	{
	  for (i=0;i<16;i++)
	    {
	      // Draw trig note color
	      if (P[cty].getPatternElement(i).get(NOTE_ON))
		  {
		  SG.drawBoxNumber(i,NOTE_COLOR);	    
		  if (i==cursor)
		    SG.drawBoxNumber(cursor,CURSOR_COLOR);
		  if (i==step)
		    SG.drawBoxNumber(step,STEP_COLOR);  

		  //		  if (i==SEQ.getPatternSequencer(cty).getStep())
		  //SG.drawBoxNumber(SEQ.getPatternSequencer(cty).getStep(),STEP_COLOR);  

		  SG.smallBoxNumber(i,
				    (P[cty].getPatternElement(i).get(NOTE)%12)*10,
				    (128-(P[cty].getPatternElement(i).get(NOTE)/12)*10),
				    SMALLBOX_COLOR);

		  
		}
	    }
	}
    }
  
}


void CursynthUserInterface::display_board_vco()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCMIX_PHASE)
    {
      display_board_two_param(VCO_MIX,OSC1_PHASE);
    }

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCAMP)
    {
       display_board_two_param(OSC1_AMP,OSC2_AMP);
    }

  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_FMTYPE)
    {	  
      display_board_one_param_text(FM_TYPE); 
    }
  
}

void CursynthUserInterface::display_board_osc()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_OSC)
    {
      display_board_two_param_text(OSC1_TYPE,OSC2_TYPE);
    }
}


void CursynthUserInterface::display_board_lfo()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // LFOPITCH

  if (menu_cursor==GLOBALMENU_LFO &&
      menu_lfo   ==MENU_LFO_LFOPITCH)
    {

      display_board_two_param(LFO1_DEPTH,LFO1_FREQ);
    }

  // PITCHBEND

  if (menu_cursor==GLOBALMENU_LFO &&
      menu_lfo   ==MENU_LFO_PITCHBEND)
    {
      display_board_two_param(PITCHBEND_DEPTH,PITCHBEND_SPEED);
    }

  if (menu_lfo==MENU_LFO_TYPE)
    {
      display_board_one_param_text(LFO_TYPE);
    }
}


void CursynthUserInterface::display_board_fltr()
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
	  display_board_two_param_text(FILTER1_ALGO,FILTER1_TYPE);
	}
    }
}
