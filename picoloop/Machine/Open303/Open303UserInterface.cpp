#include "Open303UserInterface.h"




Open303UserInterface::Open303UserInterface()
{
}

Open303UserInterface::~Open303UserInterface()
{
}

void display_board_two_param(int machineParam1,int machineParam2);
void sub_handle_invert_trig();
int handle_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all);
void helper_handle_key_two_button(int menu_cursor_tc, int menu_ad_tc, int key_repeat_interval, int param1, int param2, int inc_param1, int inc_param2 );
void helper_handle_key_one_button(int menu_cursor_tc, int menu_ad_tc, int key_repeat_interval, int param1, int inc_param1 );
void helper_change_sub_menu(int nb_menu);
void display_board_one_param_text(int machineParam1);
void display_board_two_param_text(int machineParam1,int machineParam2);
void display_board_trig();


void Open303UserInterface::handle_key(int menu)
{
  //DPRINTF("Open303UserInterface::handle_key(%d,%d)\n",menu);
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

void Open303UserInterface::handle_key_amp_env()
{
  // GLOBALMENU_AD AMP
  // Move Attack Release Decay Sustain
  // Insert/Remove Trig
  helper_handle_key_two_button(GLOBALMENU_AD, MENU_PAGE0_SUB0, KEY_REPEAT_INTERVAL_SMALLEST,
			       ADSR_AMP_RELEASE,
			       ADSR_AMP_ATTACK,
			       1,1);
  helper_handle_key_two_button(GLOBALMENU_AD, MENU_PAGE0_SUB1, KEY_REPEAT_INTERVAL_SMALLEST,
			       ADSR_AMP_DECAY,
			       ADSR_AMP_SUSTAIN,
			       1,1);


  helper_handle_key_two_button(GLOBALMENU_AD, MENU_PAGE0_SUB2, KEY_REPEAT_INTERVAL_SMALLEST,
			       AMP,
			       TRIG_TIME_DURATION,
			       1,1);


  helper_change_sub_menu(MENU_PAGE0_SUB2);


}

void Open303UserInterface::handle_key_note()
{
  helper_handle_key_two_button(GLOBALMENU_NOTE, MENU_PAGE0_SUB0, KEY_REPEAT_INTERVAL_LONG,
			       NOTE1,
			       NOTE1,
			       1,12);

  helper_change_sub_menu(MENU_PAGE0_SUB0);


}

void Open303UserInterface::handle_key_osc()
{
}


void Open303UserInterface::handle_key_vco()
{

  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value

  helper_handle_key_two_button(GLOBALMENU_VCO, MENU_PAGE0_SUB0, KEY_REPEAT_INTERVAL_SMALLEST,
			       VCO_MIX,
			       OSC1_PHASE,
			       1,1);

  helper_handle_key_two_button(GLOBALMENU_VCO, MENU_PAGE0_SUB1, KEY_REPEAT_INTERVAL_SMALLEST,
			       OSC1_AMP,
			       OSC2_AMP,
			       127,1);

  helper_change_sub_menu(MENU_PAGE0_SUB1); 
}


void Open303UserInterface::handle_key_lfo()
{
}



void Open303UserInterface::handle_key_fltr()
{

  
  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
    helper_handle_key_two_button(GLOBALMENU_FLTR, MENU_PAGE0_SUB0, KEY_REPEAT_INTERVAL_SMALLEST,
			       FILTER1_RESONANCE,
			       FILTER1_CUTOFF,
			       1,1);

  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  // Insert/Remove Trig
  helper_handle_key_one_button(GLOBALMENU_FLTR, MENU_PAGE0_SUB1, KEY_REPEAT_INTERVAL_LONG,
			       FILTER1_TYPE,
			       1);
  // change GLOBALMENU_FLTR SUBMENU
  helper_change_sub_menu(MENU_PAGE0_SUB1); 

}


void Open303UserInterface::display_board_text()
{
  int  i=0;
  char str_line1[64]="";
  char str_line2[64]="";
  char str_line3[64]="";
  char str_line4[64]="";
  char str_line5[64]="";
  char str_menu[64]="";

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

  if (menu_cursor==GLOBALMENU_AD)
    {
      if (menu_sub==MENU_PAGE0_SUB0)     sprintf(str_line2,"AccentDecay/AmpRelease");
      if (menu_sub==MENU_PAGE0_SUB1)     sprintf(str_line2,"SquarePhase/SlideTime");
      if (menu_sub==MENU_PAGE0_SUB2)     sprintf(str_line2,"AMP  T/N");	    
    }
  if (menu_cursor==GLOBALMENU_FLTR)
    {
      if (menu_sub==MENU_PAGE0_SUB0)     sprintf(str_line2,"CUTOFF/RES"); 
      if (menu_sub==MENU_PAGE0_SUB1)     sprintf(str_line2,"ALGO/TYPE"); 
    }
    if (menu_cursor==GLOBALMENU_VCO)
    {
      if (menu_sub==MENU_PAGE0_SUB0)      sprintf(str_line2,"Velocity/Accent");
      if (menu_sub==MENU_PAGE0_SUB1)      sprintf(str_line2,"envAmount/VCOMix");
      //if (menu_sub==MENU_PAGE0_SUB2)      sprintf(str_line2,"Detune/CrossMod");
    }
  if (menu_cursor==GLOBALMENU_LFO)
    {
      if (menu_sub==MENU_PAGE0_SUB0)      sprintf(str_line2,"NO LFO");
    }
  if (menu_cursor==GLOBALMENU_OSC)
    {
      if (menu_sub==MENU_PAGE0_SUB0)      sprintf(str_line2,"NO OSC");
    }
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_AD)    sprintf(str_menu,"[ENV] Note  OSC   VCO   LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_NOTE)  sprintf(str_menu," ENV [Note] OSC   VCO   LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_OSC)   sprintf(str_menu," ENV  Note [OSC]  VCO   LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_VCO)   sprintf(str_menu," ENV  Note  OSC  [VCO]  LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_LFO)   sprintf(str_menu," ENV  Note  OSC   VCO  [LFO]  FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_FLTR)  sprintf(str_menu," ENV  Note  OSC   VCO   LFO  [FLTR]  ");


  if (menu==0)                                              sprintf(str_menu,"                                     ");

  if (menu_cursor==GLOBALMENU_AD)               sprintf(str_line4,"ENV     ");
  if (menu_cursor==GLOBALMENU_NOTE)             sprintf(str_line4,"Note    ");
  if (menu_cursor==GLOBALMENU_OSC)              sprintf(str_line4,"OSC     ");
  if (menu_cursor==GLOBALMENU_VCO)              sprintf(str_line4,"VCO     ");
  if (menu_cursor==GLOBALMENU_LFO)              sprintf(str_line4,"LFO     ");
  if (menu_cursor==GLOBALMENU_FLTR)             sprintf(str_line4,"FLTR    ");


  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line2,str_line2);

  if (menu==MENU_ON_PAGE1)
    SG.guiTTFText(menu_x_display_offset,
		  menu_y_display_offset,str_menu);


}





void Open303UserInterface::display_board(int menu)
{
  DPRINTF("UserInterface::display_board(%d,)\n",menu);
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


void Open303UserInterface::display_board_amp_env()
{
  DPRINTF("UserInterface::display_board_amp_env()\n");
  if (menu_cursor==GLOBALMENU_AD)
    {
      if (menu_sub>MENU_PAGE0_SUB4)
	{ menu_sub=MENU_PAGE0_SUB0; }
      
      if (menu_sub==MENU_PAGE0_SUB0) display_board_two_param(ADSR_AMP_RELEASE, ADSR_AMP_ATTACK);
      if (menu_sub==MENU_PAGE0_SUB1) display_board_two_param(ADSR_AMP_DECAY,   ADSR_AMP_SUSTAIN);
      if (menu_sub==MENU_PAGE0_SUB2) display_board_two_param(AMP,TRIG_TIME_DURATION);	
    }  
}




void Open303UserInterface::display_board_note()
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


void Open303UserInterface::display_board_vco()
{
  if (menu_cursor == GLOBALMENU_VCO)
    {
      if (menu_sub>MENU_PAGE0_SUB1)
	{ menu_sub=MENU_PAGE0_SUB0; }

      if (menu_sub    == MENU_PAGE0_SUB0) display_board_two_param(VCO_MIX,OSC1_PHASE);
      if (menu_sub    == MENU_PAGE0_SUB1) display_board_two_param(OSC1_AMP,OSC2_AMP);
    }  
}

void Open303UserInterface::display_board_osc()
{
  if (menu_cursor==GLOBALMENU_OSC)
    {
      if (menu_sub>MENU_PAGE0_SUB2)
	{ menu_sub=MENU_PAGE0_SUB0; }
      display_board_trig();
    }
}


void Open303UserInterface::display_board_lfo()
{
  if (menu_cursor==GLOBALMENU_LFO)
    {
      if (menu_sub>MENU_PAGE0_SUB2)
	{ menu_sub=MENU_PAGE0_SUB0; }
      display_board_trig();
    }
}


void Open303UserInterface::display_board_fltr()
{
  if (menu_cursor==GLOBALMENU_FLTR)
    {
      if (menu_sub>MENU_PAGE0_SUB1)
	{ menu_sub=MENU_PAGE0_SUB0; }
      
      if (menu_sub==MENU_PAGE0_SUB0) display_board_two_param(FILTER1_RESONANCE,FILTER1_CUTOFF);
      if (menu_sub==MENU_PAGE0_SUB1) display_board_one_param_text(FILTER1_TYPE);
    }
}

