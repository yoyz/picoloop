using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <SDL_GUI.h>

#include "InputManager.h"

InputManager IE;
SDL_GUI SG;

int32_t pal0[MAXCOLOR]={0x0,      0xC8A0A8, 0x59AADA, 0xFFA11C, 0x39CE90, 0x36B82B, 0x3E789A, 0xFFFFFF, 0x0 };
int32_t *pal=pal0;

int main()
{
  printf("ESC+RETURN to exit\n");
  IE.init();
  SG.initVideo();
  
  mapii key_state;
  mapii key_repeat;
  int    last_event;
  int    last_key;

  while (true)
    {
      IE.handleKey();
      key_state=IE.keyState();
      key_repeat=IE.keyRepeat();
      last_event=IE.lastEvent();
      last_key=IE.lastKey();

      if (last_key   == BUTTON_A && 
	  last_event == KEYPRESSED)
	{
	  printf("A:%d\n",key_repeat[BUTTON_A]);	  
	  //IE.clearLastKeyEvent();
	}
      if (key_state[BUTTON_B]      &&
	  key_state[BUTTON_START]  &&
	  last_event == KEYPRESSED)
	{
	  printf("B+START:%d\n",key_repeat[BUTTON_B]);	  
	  //IE.clearLastKeyEvent();
	}

      
      //IE.printState();
      SDL_Delay(10);
      if (IE.shouldExit())
	exit(0);
      
      //      sleep(1);
      
    }
  exit(0);
}
