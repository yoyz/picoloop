#include "InputManager.h"
#include "Master.h"

#ifdef __SDL12__
InputManager::InputManager() 
//: m_key_state(new  int[MAX_KEY]),
//			       m_key_repeat(new int[MAX_KEY])
#endif
#ifdef __SDL20__
InputManager::InputManager() 
#endif
{
  int i=0;
  last_key=0;
  last_event=0;
  quit=0;
  escape=0;
}

InputManager::~InputManager()
{

}

void InputManager::init()
{
  int i=0;
  DPRINTF("InputManager::init() %d",i);

  last_key=0;
  last_event=0;
  quit=0;
  escape=0;
#ifdef __SDL12__
  m_key_state=malloc(sizeof(int)*MAX_KEY);
  m_key_repeat=malloc(sizeof(int)*MAX_KEY);

  for (i=0;i<MAX_KEY;i++)
    m_key_state[i]=0;

  for (i=0;i<MAX_KEY;i++)
    m_key_repeat[i]=0;
#endif
}

void InputManager::printState()
{
  int symbol;
  
  symbol=SDLK_ESCAPE;  DPRINTF("%d[%d %d]\n",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=SDLK_RETURN;  DPRINTF("%d[%d %d]\n",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=SDLK_LEFT;    DPRINTF("%d[%d %d]\n",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=SDLK_RIGHT;   DPRINTF("%d[%d %d]\n",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=SDLK_UP;      DPRINTF("%d[%d %d]\n",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=SDLK_DOWN;    DPRINTF("%d[%d %d]\n",symbol,m_key_state[symbol],m_key_repeat[symbol]);

}

#ifdef __SDL20__
int InputManager::updateStateNoKeyPress()
{
  for(mapii::iterator it = m_key_state.begin(); it != m_key_state.end(); ++it) 
    {
      if (m_key_state[it->first])
	{
	  m_key_repeat[it->first]=m_key_repeat[it->first]+1;
	}
    }  
}
#endif
#ifdef __SDL12__
int InputManager::updateStateNoKeyPress()
{
  int i;
  for (i=0;i<MAX_KEY;i++)
    if (m_key_state[i])
      m_key_repeat[i]=m_key_repeat[i]+1;
}
#endif

#ifdef __SDL20__
int InputManager::updateState(int symbol,int state)
{
  int i;

  m_key_state[symbol]=state;

  if (state==0)
    {
      m_key_repeat[symbol]=0;
    }

  if (state)
    for(mapii::iterator it = m_key_state.begin(); it != m_key_state.end(); ++it) 
      {
	if (m_key_state[it->first])
	  {
	    m_key_repeat[it->first]=m_key_repeat[it->first]+1;
	  }
      }
}
#endif //__SDL20__

#ifdef __SDL12__
int InputManager::updateState(int symbol,int state)
{
  int i;
  m_key_state[symbol]=state;
  if (state==false)
    m_key_repeat[symbol]=0;
  
  if (state)
    for (i=0;i<MAX_KEY;i++)
      if (m_key_state[i])
	m_key_repeat[i]=m_key_repeat[i]+1;
}
#endif


int InputManager::shouldExit()
{
  int a=0;
  if (m_key_state[BUTTON_SELECT] && m_key_state[BUTTON_START])
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


mapii & InputManager::keyState()
{
#ifdef __SDL12__
  return m_key_state;
#endif
  return m_key_state;
}

mapii & InputManager::keyRepeat()
{
#ifdef __SDL12__
  return m_key_repeat;
#endif

  return m_key_repeat;
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
  SDL_Event event;
  if (SDL_PollEvent(&event))
    {     
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
	  this->updateState(event.key.keysym.sym,0);
	  break;

	  
	case SDL_KEYDOWN:
	  keypressrelease=1;
	  key=1;
	  last_key=event.key.keysym.sym;
	  last_event=event.type;
	  this->updateState(event.key.keysym.sym,1);
	  break;

	case SDL_JOYBUTTONUP:
	  keypressrelease=1;
	  joy=1;
	  last_key=event.jbutton.button;
	  last_event=event.type;
	  this->updateState(event.jbutton.button,0);
	  break;	 

	case SDL_JOYBUTTONDOWN:
	  keypressrelease=1;
	  joy=1;
	  last_key=event.jbutton.button;
	  last_event=event.type;
	  this->updateState(event.jbutton.button,1);
	  break;

	}
      if (key)
	DPRINTF("key new event:%d %d %s\n",event.type,event.key.keysym.sym,SDL_GetKeyName(event.key.keysym.sym));
      if (joy)
	DPRINTF("joy new event:%d %d   \n",event.type,event.jbutton.button);

    }
  if (keypressrelease==0) //need to update the state to increment keypress
    this->updateStateNoKeyPress();
  return keypressrelease;
}
