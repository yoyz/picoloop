#include "InputManager.h"
#include "Master.h"
//InputManager::InputManager() : key_state(new  bool[MAX_KEY]),
//			       key_repeat(new int[MAX_KEY])
InputManager::InputManager()
{
  int i=0;
  last_key=0;
  last_event=0;
  quit=0;
  escape=0;

}

InputManager::~InputManager()
{
  //  if (key_state!=NULL)
  //    free(key_state);
  //  if (key_repeat!=NULL)
  //    free(key_repeat);
}

void InputManager::init()
{
  int i=0;
  DPRINTF("InputManager::init() %d",i);

  key_state=malloc(sizeof(int)*MAX_KEY);
  key_repeat=malloc(sizeof(int)*MAX_KEY);

  for (i=0;i<MAX_KEY;i++)
    key_state[i]=0;

  for (i=0;i<MAX_KEY;i++)
    key_repeat[i]=0;


  last_key=0;
  last_event=0;
  quit=0;
  escape=0;

}

void InputManager::printState()
{
  int symbol;
  
  symbol=SDLK_ESCAPE;  DPRINTF("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_RETURN;  DPRINTF("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_LEFT;    DPRINTF("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_RIGHT;   DPRINTF("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_UP;      DPRINTF("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_DOWN;    DPRINTF("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  //DPRINTF("\n");
}


int InputManager::updateState(int symbol,int state)
{
  int i;
  key_state[symbol]=state;
  if (state==0)
    key_repeat[symbol]=0;

  if (state)
    for (i=0;i<MAX_KEY;i++)
      if (key_state[i])
	key_repeat[i]=key_repeat[i]+1;

}

int InputManager::shouldExit()
{
  int a=0;
  //if (key_state[SDLK_ESCAPE] && key_state[SDLK_RETURN])
  if (key_state[BUTTON_SELECT] && key_state[BUTTON_START])
    {
      this->printState();
      DPRINTF("[SELECT] + [START] => END %d",a);      
      quit=1;
      return(1);
    }
  if (quit)
    return(1);
  return(0);
}


int * InputManager::keyState()
{
  return key_state;
}

int * InputManager::keyRepeat()
{
  return key_repeat;
}


int InputManager::lastKey()
{
  return last_key;
}

int InputManager::lastEvent()
{
  return last_event;
}

void InputManager::clearLastKeyEvent()
{
  last_event=0;
  last_key=0;
}

/*
  int handleKey()
  return 
    0 if nothing happen   => most of the time ( non blocking function )
    1 if something happen => sometime
 */
int InputManager::handleKey()
{
  int keypressrelease=0;
  int key=0;
  int joy=0;
  //int keypressrelease=0;
  SDL_Event event;
  if (SDL_PollEvent(&event))
    {     
      //DPRINTF("[%d %d %d %d]\n",SDL_KEYUP,SDL_KEYDOWN,event.type,event.key.keysym.sym);
      switch (event.type)
	{
	  case SDL_QUIT:
	    quit=true;
	    //DPRINTF("Exiting...\n");
	    //exit(0);
	    break; 	  
	  
	case SDL_KEYUP:
	  keypressrelease=1;
	  key=1;
	  last_key=event.key.keysym.sym;
	  last_event=event.type;
	  //last_event=2;
	  this->updateState(event.key.keysym.sym,0);
	  break;

	  
	case SDL_KEYDOWN:
	  keypressrelease=1;
	  key=1;
	  last_key=event.key.keysym.sym;
	  last_event=event.type;
	  //last_event=3;
	  this->updateState(event.key.keysym.sym,1);
	  break;

	case SDL_JOYBUTTONUP:
	  keypressrelease=1;
	  joy=1;
	  last_key=event.jbutton.button;
	  last_event=event.type;
	  //last_event=3;
	  this->updateState(event.jbutton.button,1);
	  break;	 

	case SDL_JOYBUTTONDOWN:
	  keypressrelease=1;
	  joy=1;
	  last_key=event.jbutton.button;
	  last_event=event.type;
	  //last_event=3;
	  this->updateState(event.jbutton.button,1);
	  break;



	  
	  //switch (event.key.keysym.sym)
	  //	    {
	      /*
	    case SDLK_ESCAPE:
	      escape=true;
	      DPRINTF("Exiting...\n");
	      exit(0);
 	      break;
	      */
	      
	  //default:
	  //  }	  
	  // break;
	  
	}
      if (key)
	DPRINTF("key new event:%d %d %s\n",event.type,event.key.keysym.sym,SDL_GetKeyName(event.key.keysym.sym));
      if (joy)
	DPRINTF("joy new event:%d %d   \n",event.type,event.jbutton.button);

    }
  if (keypressrelease==0) //need to update the state to increment keypress
    this->updateState(MAX_KEY,1); 
  return keypressrelease;
}
