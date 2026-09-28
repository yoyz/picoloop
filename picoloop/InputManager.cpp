#include "InputManager.h"
#include "Master.h"

#ifdef RG35XXSP
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/select.h>
static int g_evdev_fd = -1;
static int volHold = 0;
static int volTick = 0;
#endif

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
#ifdef RG35XXSP
  g_evdev_fd = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
  DPRINTF("evdev event1 fd:%d", g_evdev_fd);
#endif
#ifdef __SDL12__
  m_key_state=(mapii)malloc(sizeof(int)*MAX_KEY);
  m_key_repeat=(mapii)malloc(sizeof(int)*MAX_KEY);

  for (i=0;i<MAX_KEY;i++)
    m_key_state[i]=0;

  for (i=0;i<MAX_KEY;i++)
    m_key_repeat[i]=0;
#endif
}

void InputManager::printState()
{
  int symbol;
  
  symbol=BUTTON_SELECT;  DPRINTF("%d[%d %d]",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=BUTTON_START;   DPRINTF("%d[%d %d]",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=BUTTON_LEFT;    DPRINTF("%d[%d %d]",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=BUTTON_RIGHT;   DPRINTF("%d[%d %d]",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=BUTTON_UP;      DPRINTF("%d[%d %d]",symbol,m_key_state[symbol],m_key_repeat[symbol]);
  symbol=BUTTON_DOWN;    DPRINTF("%d[%d %d]",symbol,m_key_state[symbol],m_key_repeat[symbol]);

}

#ifdef __SDL20__
int InputManager::updateStateNoKeyPress()
{
  for(mapii::iterator it = m_key_state.begin(); it != m_key_state.end(); ++it) 
    {
	    //printf("%d\n",it->first);
      //if (it->first)
      if (m_key_state[it->first])
	{
	  m_key_repeat[it->first]=m_key_repeat[it->first]+1;
	}
    }  
  return 0;
}
#endif
#ifdef __SDL12__
int InputManager::updateStateNoKeyPress()
{
  int i;
  for (i=0;i<MAX_KEY;i++)
    if (m_key_state[i])
      m_key_repeat[i]=m_key_repeat[i]+1;
  return 0;
}
#endif

#ifdef __SDL20__
int InputManager::updateState(int symbol,int state)
{
  int i;

	    printf("updateState %d %d\n",symbol,state);
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
  return 0;
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
  return 0;
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
#ifdef __SDL12__
void InputManager::clearStateAndRepeat()
{
  int i;
  for (i=0;i<MAX_KEY;i++)
    {
      m_key_state[i]=0;  
      m_key_repeat[i]=0;
    }
}
#endif
#ifdef __SDL20__
void InputManager::clearStateAndRepeat()
{
    for(mapii::iterator it = m_key_state.begin(); it != m_key_state.end(); ++it) 
      m_key_state[it->first]=0;

    for(mapii::iterator it = m_key_repeat.begin(); it != m_key_repeat.end(); ++it) 
      m_key_repeat[it->first]=0;
}
#endif


/*
  int handleKey()
  return 
    0 if nothing happen   => most of the time ( non blocking function )
    1 if something happen => sometime
 */
#if defined(PSVITA)
int InputManager::handleKey()
{
  int keypressrelease=0;
  int key=0;
  int joy=0;
  SDL_Event event;
  SceCtrlData     pad;
  int keydown=-1;
  int keypadtotest=0;
  int keypadtotest_valid=0;
  sceCtrlPeekBufferPositive(0, &pad, 1);

  keypadtotest=SCE_CTRL_UP;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_DOWN;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_LEFT;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_RIGHT;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_SQUARE;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_CROSS;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_CIRCLE;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_TRIANGLE;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  
  keypadtotest=SCE_CTRL_START;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_SELECT;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  keypadtotest=SCE_CTRL_LTRIGGER;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

    keypadtotest=SCE_CTRL_RTRIGGER;
  if ((pad.buttons & keypadtotest) && (m_key_state[keypadtotest]==0))
    { m_key_state[keypadtotest]=1; keypressrelease=1; keydown=1; key=1; keypadtotest_valid=keypadtotest;  }
  if (!(pad.buttons & keypadtotest) && m_key_state[keypadtotest]==1)
    { m_key_state[keypadtotest]=0; keypressrelease=1; keydown=0; key=1; keypadtotest_valid=keypadtotest; }

  
  if (keypressrelease)
    {
      this->updateState(keypadtotest_valid,keydown);
      if (keypadtotest_valid == SCE_CTRL_TRIANGLE)
	this->updateState(SCE_CTRL_START,keydown);
      if (keypadtotest_valid == SCE_CTRL_SQUARE)
	this->updateState(SCE_CTRL_SELECT,keydown);

      last_key=keypadtotest_valid;
      if (keydown) last_event=KEYPRESSED;
      else         last_event=KEYRELEASED;
    }
  if (key)
    DPRINTF("event occur on : %d",keypadtotest_valid);

  if (keypressrelease==0) //need to update the state to increment keypress
    {
      this->updateStateNoKeyPress();
      this->clearLastKeyEvent();
    }
  return keypressrelease;
}
#else // Generic platform after here, we are not using PSVITA here, so debian and other

int InputManager::handleKey()
{
  int keypressrelease=0;
  int key=0;
  int joy=0;
  SDL_Event event;
  //printf("polling\n");

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
#ifdef RG35XXSP
	  last_key=event.jbutton.button;
	  last_event=event.type;
	  this->updateState(event.jbutton.button,0);
#else
	  last_key=event.jbutton.button;
	  last_event=event.type;
	  this->updateState(event.jbutton.button,0);
#endif
	  break;	 

	case SDL_JOYBUTTONDOWN:
	  keypressrelease=1;
	  joy=1;
	  last_key=event.jbutton.button;
	  last_event=event.type;
	  this->updateState(event.jbutton.button,1);
	  break;

#ifdef RG35XXSP
	case SDL_JOYHATMOTION:
	  keypressrelease=1;
	  joy=1;
	  {
	    const int hat_dir[4]    = { SDL_HAT_UP, SDL_HAT_DOWN, SDL_HAT_LEFT, SDL_HAT_RIGHT };
	    const int hat_button[4] = { BUTTON_UP,  BUTTON_DOWN,  BUTTON_LEFT,  BUTTON_RIGHT };
	    int hat = event.jhat.value;
	    for (int i=0;i<4;i++)
	      {
		int newstate = (hat & hat_dir[i]) ? 1 : 0;
		if (m_key_state[hat_button[i]] != newstate)
		  {
		    this->updateState(hat_button[i], newstate);
		    last_key   = hat_button[i];
		    last_event = newstate ? KEYPRESSED : KEYRELEASED;
		  }
	      }
	  }
	  break;
#endif

	case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
	  if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) ) 
	    {
	      if( event.jaxis.axis == 0) 
	        {
		  /* Left-right movement code goes here */
	          if ( event.jaxis.value < -3200) this->updateState(BUTTON_LEFT,1);
	          if ( event.jaxis.value >  3200) this->updateState(BUTTON_RIGHT,1);
		  keypressrelease=1;
	        }
              if( event.jaxis.axis == 1) 
	        {
		  /* Up-Down movement code goes here */
	          if ( event.jaxis.value < -3200) this->updateState(BUTTON_DOWN,1);
	          if ( event.jaxis.value >  3200) this->updateState(BUTTON_UP,1);
		  keypressrelease=1;
                }
	    }
	}
      
      if (key)
	DPRINTF("key new event:%d %d %s",event.type,event.key.keysym.sym,SDL_GetKeyName(event.key.keysym.sym));
      if (joy)
	DPRINTF("joy new event:%d %d   ",event.type,event.jbutton.button);

    }
#ifdef LINUX_RASPIBOY
  if (event.type!=SDL_JOYAXISMOTION && keypressrelease==0)
    {
      this->updateState(BUTTON_UP   ,0);
      this->updateState(BUTTON_DOWN ,0);
      this->updateState(BUTTON_LEFT ,0);
      this->updateState(BUTTON_RIGHT,0);
      keypressrelease=1;
    }
#endif // LINUX_RASPIBOY
  if (keypressrelease==0) //need to update the state to increment keypress
    {
      this->updateStateNoKeyPress();
      this->clearLastKeyEvent();
    }
  //this->printState();
  return keypressrelease;
}
#endif // Generic platform we are not using PSVITA here, so debian and other

#ifdef RG35XXSP
// Poll the physical volume rocker directly from evdev. SDL does not
// surface KEY_VOLUMEUP/KEY_VOLUMEDOWN on this device. Handles press
// (value=1) and kernel auto-repeat (value=2); held key keeps repeating
// via volRepeat/volHold even if the kernel repeat rate is disabled.
int InputManager::readVolumeKeys(int & volDelta)
{
  volDelta = 0;
  if (g_evdev_fd < 0) return 0;

  struct input_event ev;
  int pressed = 0;
  while (read(g_evdev_fd, &ev, sizeof(ev)) == (ssize_t)sizeof(ev)) {
    if (ev.type == EV_KEY) {
      if (ev.code == KEY_VOLUMEUP && ev.value == 1)
        { volHold = 1; volTick = 0; pressed = 1; }
      if (ev.code == KEY_VOLUMEDOWN && ev.value == 1)
        { volHold = -1; volTick = 0; pressed = 1; }
      if (ev.code == KEY_VOLUMEUP && ev.value == 0)
        { if (volHold == 1) volHold = 0; }
      if (ev.code == KEY_VOLUMEDOWN && ev.value == 0)
        { if (volHold == -1) volHold = 0; }
      // kernel auto-repeat (value=2)
      if (ev.code == KEY_VOLUMEUP && ev.value == 2)
        { volHold = 1; volTick = 0; pressed = 1; }
      if (ev.code == KEY_VOLUMEDOWN && ev.value == 2)
        { volHold = -1; volTick = 0; pressed = 1; }
    }
  }

  if (pressed) {
    volDelta = volHold;
    return volDelta;
  }

  // Held key: keep repeating every ~40ms (main loop runs ~1ms).
  if (volHold != 0) {
    volTick++;
    if (volTick >= 40) {
      volTick = 0;
      volDelta = volHold;
    }
  }
  return volDelta;
}
#endif
