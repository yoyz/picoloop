using namespace std;

#include <SDL/SDL.h>

#ifndef __INPUTMANAGER__
#define __INPUTMANAGER__

#define MAXKEY 512

class InputManager
{
public:
  InputManager();
  int    handleKey();
  int    shouldExit();
  int    updateState(int symbol,bool state);
  void   printState();
 private:

  bool  * key_state;    //[ 0,1, 0, 1,0,0....] value of key
  int   * key_repeat;   //[ 0,12,0,32,0,0....] number of time handleKey is called 
  int     last_key;     // the SDL_KEY number with an event associated
  bool    last_event;   // the event : 0 = SDL_KEYUP ; 1 = SDL_KEYDOWN
  bool    quit;
  bool    escape;
  int     max_key;
};

#endif
