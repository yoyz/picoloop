using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <SDL_GUI.h>

#include "InputManager.h"

InputManager IE;
SDL_GUI SG;


int main()
{
  printf("ESC+RETURN to exit\n");
  SG.initVideo();
  bool * key_state;
  int  * key_repeat;
  while (true)
    {

      //printf("%d %d ",SDLK_p, SDLK_m);
      //printf("launch InputManager::.handleKey()\n");
      IE.handleKey();
      key_state=IE.keyState();
      key_repeat=IE.keyRepeat();
      
      //if (key_state[SDLK_LEFT])
      if (IE.lastEvent()==0 && IE.lastKey()==SDLK_LEFT)
	{
	  printf("LEFT:%d\n",key_repeat[SDLK_LEFT]);	  
	  IE.clearLastKeyEvent();
	}
      //IE.printState();
      SDL_Delay(10);
      if (IE.shouldExit())
	exit(0);
      
      //      sleep(1);
      
    }
  exit(0);
}
