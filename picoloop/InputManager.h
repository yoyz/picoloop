#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              
#include <map>

#ifndef __INPUTMANAGER__
#define __INPUTMANAGER__



// for SDL12 I use a simple int * because it is fast and easy to use
// for SDL20 I use a map because SDL2 use a weird keyboard symbol with a huge gap in the middle
// so  SDL20 for me require this kind of "datatype"
#ifdef __SDL12__
#define MAX_KEY 512
typedef int * mapii;
#endif
#ifdef __SDL20__
typedef std::map<int32_t,int32_t> mapii;
#endif

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
  void   clearStateAndRepeat();
  void   printState();
  int    lastKey();
  int    lastEvent(); 
  void   clearLastKeyEvent();  
  mapii  & keyState();
  mapii  & keyRepeat();
  
 private:


  int     last_key;             // the last key   : SDLK_XXX number          || 0 if event cleared
  int    last_event;           // the last event : SDL_KEYUP || SDL_KEYDOWN  || 0 if event cleared 
  int    quit;
  int    escape;
                                 // With SDL1.2
  mapii    m_key_state;          // [ 0,1, 0, 1,0,0....] key is press or not
  mapii    m_key_repeat;         // [ 0,12,0,32,0,0....] number of time handleKey is called 
};

#endif
