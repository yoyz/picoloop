#include "InputManager.h"

InputManager::InputManager()
{
  int i;
  key_state=(bool*)malloc(sizeof(bool)*MAX_KEY);
  for (i=0;i<MAX_KEY;i++)
    key_state[i]=false;

  key_repeat=(int*)malloc(sizeof(int)*MAX_KEY);
  for (i=0;i<MAX_KEY;i++)
    key_repeat[i]=0;
}




void InputManager::printState()
{
  int symbol;
  
  symbol=SDLK_LEFT;  printf("%d[%d %d]",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_RIGHT; printf("%d[%d %d]",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_UP;    printf("%d[%d %d]",symbol,key_state[symbol],key_repeat[symbol]);
  symbol=SDLK_DOWN;  printf("%d[%d %d]",symbol,key_state[symbol],key_repeat[symbol]);
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
    return(1);
  if (quit)
    return(1);
  return(0);
}


bool * InputManager::returnKeyState()
{
  return key_state;
}

int * InputManager::returnKeyRepeat()
{
  return key_repeat;
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
	  last_event=false;
	  this->updateState(event.key.keysym.sym,false);
	  break;

	  
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	      /*
	    case SDLK_ESCAPE:
	      escape=true;
	      printf("Exiting...\n");
	      exit(0);
 	      break;
	      */
	      
	    default:
	      keypressrelease=1;
	      last_key=event.key.keysym.sym;
	      last_event=true;
	      this->updateState(event.key.keysym.sym,true);
	      break;
	    }	  
	  break;
	  
	}
      printf("new event:%d %s\n",event.type,SDL_GetKeyName(event.key.keysym.sym));
    }
  if (keypressrelease==0) //need to update the state to increment keypress
    this->updateState(MAX_KEY,true); 
  return keypressrelease;
}
