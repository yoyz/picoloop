using namespace std;

//#include <SDL/SDL.h>
#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              
#include <map>

#ifndef __INPUTMANAGER__
#define __INPUTMANAGER__

//#define MAX_KEY 512
//#define MAX_KEY 512
//#define MAX_KEY 1<<32

typedef map<int,int> mapii;

class InputManager
{
public:
  InputManager();
  ~InputManager();
  void   init();
  int    handleKey();
  int    shouldExit();
  int    updateState(int symbol,int state);
  int    updateStateNoKeyPress();
  void   printState();
  int    lastKey();
  int    lastEvent(); 
  void   clearLastKeyEvent();  
  mapii  & keyState();
  mapii  & keyRepeat();
  
 private:

  int   * key_state;            // [ 0,1, 0, 1,0,0....] value of key
  int   * key_repeat;           // [ 0,12,0,32,0,0....] number of time handleKey is called 
  int     last_key;             // the last key   : SDLK_XXX number          || 0 if event cleared
  int    last_event;           // the last event : SDL_KEYUP || SDL_KEYDOWN || 0 if event cleared 
  int    quit;
  int    escape;

  mapii    m_key_state;
  mapii    m_key_repeat;
  //int   * keycode_list;
};

#endif
