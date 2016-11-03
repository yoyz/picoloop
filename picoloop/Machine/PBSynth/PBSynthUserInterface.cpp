#include "PBSynthUserInterface.h"




PBSynthUserInterface::PBSynthUserInterface()
{
}

PBSynthUserInterface::~PBSynthUserInterface()
{
}

void display_board_two_param(int machineParam1,int machineParam2);
void sub_handle_invert_trig();
int handle_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all);
void display_board_one_param_text(int machineParam1);
void display_board_two_param_text(int machineParam1,int machineParam2);
void display_board_two_param_number(int machineParam1,int machineParam2);
void display_board_trig();

void PBSynthUserInterface::handle_key(int menu)
{
  DPRINTF("PBSynthUserInterface::handle_key(%d,%d)\n",menu);
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

void PBSynthUserInterface::handle_key_amp_env()
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
	  else if (menu_ad==MENU_AD_TRIGTIME_AMP)             { menu_ad=MENU_AD_AMP_ATTACK_RELEASE;      }   
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }




}

void PBSynthUserInterface::handle_key_note()
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

void PBSynthUserInterface::handle_key_osc()
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
  // change oscilltor one and two scale
  if (menu        == MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC1SCALE_OSC2SCALE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,        -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,         1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE      ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE      ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two scale
  if (menu        != MENU_OFF       && 
      menu_cursor == GLOBALMENU_OSC &&
      menu_osc    == MENU_OSC_OSC1SCALE_OSC2SCALE)
    {

      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,        -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_SCALE,         1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE      ,   1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_SCALE      ,  -1, 1);

    }

  // change GLOBALMENU_OSC SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_OSC)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_osc==MENU_OSC_OSC1OSC2)              { menu_osc=MENU_OSC_OSC1SCALE_OSC2SCALE;            }
	  else if (menu_osc==MENU_OSC_OSC1SCALE_OSC2SCALE)   { menu_osc=MENU_OSC_OSC1OSC2;                       }
	  else                                               { menu_osc=MENU_OSC_OSC1OSC2;                       }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }
}


void PBSynthUserInterface::handle_key_vco()
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
      menu_vco    == MENU_PAGE0_SUB0
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE    ,   1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE    ,  -1, 0);
    }





  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_PAGE0_SUB0
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE    ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE    ,     -1, 1);
    }

  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_PAGE0_SUB1
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP      ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP      ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH        ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH        ,     -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_PAGE0_SUB1
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP        ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, OSC2_AMP        ,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH          ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ENV1_DEPTH          ,     -1, 1);
    }




  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_VCO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  // if      (menu_vco==MENU_VCO_OSCMIX_PHASE)              { menu_vco=MENU_VCO_OSCMIX_PHASE;    }
	  // else                                                   { menu_vco=MENU_VCO_OSCMIX_PHASE;    }

	  if      (menu_vco==MENU_PAGE0_SUB0)              { menu_vco=MENU_PAGE0_SUB1;           }
	  else if (menu_vco==MENU_PAGE0_SUB1)              { menu_vco=MENU_PAGE0_SUB0;           }   
	  else                                             { menu_vco=MENU_PAGE0_SUB0;           }

	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }
}


void PBSynthUserInterface::handle_key_lfo()
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

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,     -1, 0);
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {

      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,      1, 1);

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

      handle_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_DEPTH     ,     -1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO2_DEPTH     ,      1, 0);

      handle_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ      ,      1, 0);
      handle_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ      ,     -1, 0);

    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      handle_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_DEPTH     ,     -1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO2_DEPTH     ,      1, 1);

      handle_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ      ,      1, 1);
      handle_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO2_FREQ      ,     -1, 1);

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
	  else                                                { menu_lfo=MENU_LFO_LFOPITCH;        }  
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu lfo : %d]\n",menu_lfo);
    }

}



void PBSynthUserInterface::handle_key_fltr()
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


  // change GLOBALMENU_FLTR SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_FLTR)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_fltr!=MENU_FLTR_CUTOFF_RESONANCE)   { menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;     }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu_fltr : %d]\n",menu_fltr);
    }



}





void PBSynthUserInterface::display_board_text()
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


  // ENV
  if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"AMP  A/R");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"AMP  S/D");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  if (menu_ad==MENU_AD_FLTR_ATTACK_RELEASE &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"FLTR A/R");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_ad==MENU_AD_FLTR_DECAY_SUSTAIN &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"FLTR S/D");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  if (menu_ad==MENU_AD_TRIGTIME_AMP &&
      menu_cursor==GLOBALMENU_AD)
    {
      sprintf(str_submenu,"T/N AMP");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }


  //FLTR
  if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE &&
      menu_cursor==GLOBALMENU_FLTR)
    {
      sprintf(str_submenu,"CUTOFF/RES");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_fltr==MENU_FLTR_ALGO_TYPE &&
      menu_cursor==GLOBALMENU_FLTR
      )
    {
      sprintf(str_submenu,"ALGO/TYPE");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  //VCO
  if (menu_vco==MENU_PAGE0_SUB0 &&
      menu_cursor==GLOBALMENU_VCO
      )
    {
      sprintf(str_submenu,"DETUNE/VCOMIX");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_vco==MENU_PAGE0_SUB1 &&
      menu_cursor==GLOBALMENU_VCO
      )
    {
      sprintf(str_submenu,"ENVDEPTH/PWM");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }



  if (menu_lfo==MENU_LFO_LFOPITCH &&
      menu_cursor==GLOBALMENU_LFO
      )
    {
      sprintf(str_submenu,"LFO AMP AMNT/SPEED");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }

  if (menu_lfo==MENU_LFO_PITCHBEND &&
      menu_cursor==GLOBALMENU_LFO
      )
    {
      sprintf(str_submenu,"LFO FLTR AMNT/SPEED");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }



  if (menu_osc    == MENU_OSC_OSC1OSC2 &&
      menu_cursor == GLOBALMENU_OSC)
    {
      sprintf(str_submenu,"OSC WAVEFORM");
      SG.guiTTFText(right_x_display_offset,
		    right_y_display_offset_line2,str_submenu);
    }





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





void PBSynthUserInterface::display_board(int menu)
{
  DPRINTF("PBSynthUserInterface::display_board(%d,)\n",menu);
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


void PBSynthUserInterface::display_board_amp_env()
{
  DPRINTF("PBSynthUserInterface::display_board_amp_env()\n");
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




void PBSynthUserInterface::display_board_note()
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


void PBSynthUserInterface::display_board_vco()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_PAGE0_SUB0)
    {
      display_board_two_param(VCO_MIX,OSC1_DETUNE);
    }

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_PAGE0_SUB1)
    {
      display_board_two_param(OSC2_AMP,ENV1_DEPTH);
    }  
}

void PBSynthUserInterface::display_board_osc()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_OSC &&
      menu_osc   ==MENU_OSC_OSC1OSC2)
    {
      display_board_two_param_text(OSC1_TYPE,OSC2_TYPE);
    }
  if (menu_cursor==GLOBALMENU_OSC &&
      menu_osc   ==MENU_OSC_OSC1SCALE_OSC2SCALE)
    {
      display_board_two_param_number(OSC1_SCALE,OSC2_SCALE);
    }
  if (menu_cursor==GLOBALMENU_OSC &&
      menu_osc   ==MENU_OSC_OSC1DETUNE_OSC2DETUNE)
    {
      display_board_two_param(OSC1_DETUNE,OSC2_DETUNE);
    }
}


void PBSynthUserInterface::display_board_lfo()
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
      display_board_two_param(LFO2_DEPTH,LFO2_FREQ);
    }

}


void PBSynthUserInterface::display_board_fltr()
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

