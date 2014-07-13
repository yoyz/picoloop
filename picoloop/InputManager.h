using namespace std;

#include <SDL/SDL.h>

#ifndef __INPUTMANAGER__
#define __INPUTMANAGER__

#define MAX_KEY 512

class InputManager
{
public:
  InputManager();
  ~InputManager();
  void   init();
  int    handleKey();
  int    shouldExit();
  int    updateState(int symbol,bool state);
  void   printState();
  int    lastKey();
  int    lastEvent(); 
  void   clearLastKeyEvent();  
  bool * keyState();
  int  * keyRepeat();

 private:

  bool  * key_state;            // [ 0,1, 0, 1,0,0....] value of key
  int   * key_repeat;           // [ 0,12,0,32,0,0....] number of time handleKey is called 
  int     last_key;             // the last key   : SDLK_XXX number          || 0 if event cleared
  int    last_event;           // the last event : SDL_KEYUP || SDL_KEYDOWN || 0 if event cleared 
  bool    quit;
  bool    escape;
  //int     max_key;
};

#endif
