#include "InputManager.h"

InputManager::InputManager()
{
  key_state=NULL;
  key_repeat=NULL;
  last_key=0;
  last_event=0;
  quit=false;
  escape=false;
  //max_key=0;
}

InputManager::~InputManager()
{
  if (key_state!=NULL)
    free(key_state);
  if (key_repeat!=NULL)
    free(key_repeat);
}

void InputManager::init()
{
  int i;

  printf("InputManager::init()\n");

  key_state=NULL;
  key_repeat=NULL;
  last_key=0;
  last_event=0;
  quit=false;
  escape=false;
  //max_key=0;

  if (key_state==NULL)
    key_state=(bool*)malloc(sizeof(bool)*MAX_KEY);

  if (key_repeat==NULL)
    key_repeat=(int*)malloc(sizeof(int)*MAX_KEY);

  for (i=0;i<MAX_KEY;i++)
    key_state[i]=false;

  for (i=0;i<MAX_KEY;i++)
    key_repeat[i]=0;
}

void InputManager::printState()
{
  int symbol;
  
  symbol=SDLK_ESCAPE;  printf("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_RETURN;  printf("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_LEFT;    printf("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_RIGHT;   printf("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_UP;      printf("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_DOWN;    printf("%d[%d %d]\n",symbol,key_state[symbol],key_repeat[symbol]);
  printf("\n");
}


int InputManager::updateState(int symbol,bool state)
{
  int i;
  key_state[symbol]=state;
  if (state==false)
    key_repeat[symbol]=0;

  if (state)
    for (i=0;i<MAX_KEY;i++)
      if (key_state[i])
	key_repeat[i]=key_repeat[i]+1;

}

int InputManager::shouldExit()
{
  if (key_state[SDLK_ESCAPE] && key_state[SDLK_RETURN])
    {
      this->printState();
      printf("SDLK_ESCAPE] + SDLK_RETURN\n");
      return(1);
    }
  if (quit)
    return(1);
  return(0);
}


bool * InputManager::keyState()
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
  SDL_Event event;
  if (SDL_PollEvent(&event))
    {     
      //printf("[%d %d %d %d]\n",SDL_KEYUP,SDL_KEYDOWN,event.type,event.key.keysym.sym);
      switch (event.type)
	{
	  case SDL_QUIT:
	    quit=true;
	    //printf("Exiting...\n");
	    //exit(0);
	    break; 	  
	  
	case SDL_KEYUP:
	  keypressrelease=1;
	  last_key=event.key.keysym.sym;
	  last_event=event.type;
	  //last_event=2;
	  this->updateState(event.key.keysym.sym,false);
	  break;

	  
	case SDL_KEYDOWN:
	  keypressrelease=1;
	  last_key=event.key.keysym.sym;
	  last_event=event.type;
	  //last_event=3;
	  this->updateState(event.key.keysym.sym,true);
	  break;
	  
	  //switch (event.key.keysym.sym)
	  //	    {
	      /*
	    case SDLK_ESCAPE:
	      escape=true;
	      printf("Exiting...\n");
	      exit(0);
 	      break;
	      */
	      
	  //default:
	  //  }	  
	  // break;
	  
	}
      printf("new event:%d %s\n",event.type,SDL_GetKeyName(event.key.keysym.sym));
    }
  if (keypressrelease==0) //need to update the state to increment keypress
    this->updateState(MAX_KEY,true); 
  return keypressrelease;
}
