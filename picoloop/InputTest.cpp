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
  sleep(1);
  exit(0);
}
