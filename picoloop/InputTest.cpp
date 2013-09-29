using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <SDL_GUI.h>

#include "InputManager.h"

InputManager IE;
SDL_GUI SG;


int main()
{
  SG.initVideo();
  while (true)
    {
      //printf("%d %d ",SDLK_p, SDLK_m);
      //printf("launch InputManager::.handleKey()\n");
      IE.handleKey();
      IE.printState();
      if (IE.shouldExit())
	exit(0);
      //      sleep(1);
      
    }
  exit(0);
}
