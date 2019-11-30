#include "SDL_GUI.h"
#include <assert.h>     /* assert */

SDL_GUI::SDL_GUI()
{
  DPRINTF("construction SDL_GUI::SDL_GUI()");
  screen=NULL;
  bmp_font=NULL;
  ttf_font=NULL;
  boxSize=31;
  boxOffset=20;

  message=NULL;
}

SDL_GUI::~SDL_GUI()
{
  DPRINTF("SDL_GUI::~SDL_GUI()");
  
  if (ttf_font!=NULL)
    TTF_CloseFont(ttf_font);
  if (message!=NULL)
    SDL_FreeSurface( message );
  //Quit SDL_ttf
  TTF_Quit();
}


#ifdef __SDL12__
int SDL_GUI::initVideo()
{

#ifdef __SDL_AUDIO__
  //  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)
  //if (SDL_Init(SDL_INIT_EVERYTHING)
  //if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK|SDL_INIT_TIMER)
  //if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK)
  //if (SDL_Init(SDL_INIT_VIDEO)
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK)
       
       //|SDL_INIT_TIMER|SDL_INIT_AUDIO) 
       
  //if ( SDL_Init(SDL_INIT_VIDEO) 
       //sleep(1);
       //|
       //		SDL_INIT_AUDIO|
       //SDL_INIT_TIMER
       //) < 0 )
       <0)
    {
      DPRINTF("Couldn't initialize SDL: %s", SDL_GetError());
      return -1;
    }
#endif   

#ifdef __RTAUDIO__
  if ( SDL_Init(SDL_INIT_VIDEO)<0)
    {
      DPRINTF("Couldn't initialize SDL: %s", SDL_GetError());
      return -1;
    }
#endif   



  
  DPRINTF("SDL initialized.");
  #ifdef OPENDINGUX
  screen = SDL_SetVideoMode(SCREEN_WIDTH, 
			    SCREEN_HEIGHT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
  #endif
  #ifdef PC_DESKTOP
  screen = SDL_SetVideoMode(SCREEN_WIDTH*SCREEN_MULT, 
			    SCREEN_HEIGHT*SCREEN_MULT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF);
  #endif
  #ifdef RASPI3_ILI9486
  //SDL_ShowCursor(SDL_DISABLE);
  screen = SDL_SetVideoMode(SCREEN_WIDTH*SCREEN_MULT, 
			    SCREEN_HEIGHT*SCREEN_MULT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF);
  #endif


  #ifdef LINUX_RASPI1
  screen = SDL_SetVideoMode(SCREEN_WIDTH*SCREEN_MULT, 
			    SCREEN_HEIGHT*SCREEN_MULT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF);
  #endif
  #ifdef LINUX_RASPIBOY
  screen = SDL_SetVideoMode(SCREEN_WIDTH*SCREEN_MULT, 
			    SCREEN_HEIGHT*SCREEN_MULT, 
			    SCREEN_DEPTH,			
			    SDL_SWSURFACE|SDL_FULLSCREEN);
  SDL_Joystick *joystick = NULL;
  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);
  #endif

  #ifdef LINUX_PBSYNTHONLY
  screen = SDL_SetVideoMode(SCREEN_WIDTH*SCREEN_MULT, 
			    SCREEN_HEIGHT*SCREEN_MULT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF);
  #endif

  #ifdef LINUX_POCKETCHIP
  screen = SDL_SetVideoMode(SCREEN_WIDTH*SCREEN_MULT, 
			    SCREEN_HEIGHT*SCREEN_MULT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF);
  #endif


  #ifdef GP2X
  screen = SDL_SetVideoMode(SCREEN_WIDTH, 
			    SCREEN_HEIGHT, 
			    SCREEN_DEPTH,			
			    SDL_SWSURFACE);
  SDL_Joystick *joystick = NULL;
  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);
  #endif
  #ifdef PSP
  screen = SDL_SetVideoMode(480,
			    272,
			    16,
			    SDL_HWSURFACE|SDL_DOUBLEBUF);
			    //SDL_SWSURFACE);
  SDL_Joystick *joystick = NULL;
  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);

  #endif
  


  if (screen == NULL)
    {
      DPRINTF("Couldn't set video mode: %s", SDL_GetError());
      SDL_Quit();
      return -1;
    }

  SDL_WM_SetCaption("Picoloop", NULL);
  
  return 0;
}
#endif // __SDL12__

/*

int SDL_GUI::initVideo()
{
  DPRINTF("before SDL_Init");

#if defined(PSVITA)
  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0)
    return -1;
  
  if ((window = SDL_CreateWindow( "GreenRectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) == NULL)
      exit(1);  
#endif
#if !defined(PSVITA)  
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)<0)
    {
      DPRINTF("Couldn't initialize SDL: %s", SDL_GetError());
      return -1;
    }
  DPRINTF("After SDL_Init");
  DPRINTF("Before SDL_CreateWindow");
#if defined(RASPI3_ILI9486)
  SDL_ShowCursor(SDL_DISABLE);
  window = SDL_CreateWindow("Picoloop",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            480, 320,
                            SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN|SDL_WINDOW_MAXIMIZED);
#else // all platform debian for example
  window = SDL_CreateWindow("Picoloop",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            640, 480,
                            SDL_WINDOW_SHOWN);
#endif
//SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
  DPRINTF("After SDL_CreateWindow");
  if (window == NULL) {
    // In the case that the window could not be made...
    DPRINTF("After SDL_CreateWindow %s",SDL_GetError());
    return -1;
  }
#endif // not defined PSVITA
  screen=SDL_GetWindowSurface( window );
  return 0;
}

#endif

 */

#if defined(__SDL20__) && !defined(PSVITA)
int SDL_GUI::initVideo()
{
  DPRINTF("before SDL_Init");

  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)<0)
    {
      DPRINTF("Couldn't initialize SDL: %s", SDL_GetError());
      return -1;
    }
  DPRINTF("After SDL_Init");
  DPRINTF("Before SDL_CreateWindow");
#if defined(RASPI3_ILI9486)
  SDL_ShowCursor(SDL_DISABLE);
  window = SDL_CreateWindow("Picoloop",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            480, 320,
                            SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN|SDL_WINDOW_MAXIMIZED);
#else // all platform debian for example
  window = SDL_CreateWindow("Picoloop",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            640, 480,
                            SDL_WINDOW_SHOWN);
#endif

//SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
  DPRINTF("After SDL_CreateWindow");
  if (window == NULL) {
    // In the case that the window could not be made...
    DPRINTF("After SDL_CreateWindow %s",SDL_GetError());
    return -1;
  }
  screen=SDL_GetWindowSurface( window );

  if( SDL_NumJoysticks() < 1 )
    { 
      printf( "joystick not connected!\n" );
    }
  else
    {
      printf( "joystick connected!\n" );
      SDL_JoystickEventState(SDL_ENABLE);
      gGameController = SDL_JoystickOpen( 0 );
      gControllerHaptic = SDL_HapticOpenFromJoystick( gGameController );
    }
  


  return 0;
}

#endif



#if defined(__SDL20__) && defined(PSVITA)
int SDL_GUI::initVideo()
{
  DPRINTF("before SDL_Init");

  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0)
    return -1;
  
  if ((window = SDL_CreateWindow( "GreenRectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) == NULL)
    exit(1);  
  
  window = SDL_CreateWindow("Picoloop",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            640, 480,
                            SDL_WINDOW_SHOWN);
  
  DPRINTF("After SDL_CreateWindow");
  if (window == NULL) {
    // In the case that the window could not be made...
    DPRINTF("After SDL_CreateWindow %s",SDL_GetError());
    return -1;
  }
  screen=SDL_GetWindowSurface( window );
  return 0;
}
#endif




#ifdef __SDL12__
void SDL_GUI::refresh()
{
  //SDL_LockSurface(screen);
  SDL_Flip(screen);
  //SDL_UnlockSurface(screen);
}
#endif

#ifdef __SDL20__
void SDL_GUI::refresh()
{
  DPRINTF("SDL_GUI::refresh()");
  SDL_UpdateWindowSurface( window );
}
#endif // __SDL20__


#ifdef __SDL12__
void SDL_GUI::clearScreen()
{
  extern int32_t * pal;
  DPRINTF("SDL_GUI::clearScreen()");
  //SDL_FillRect(screen,NULL, pal[8]);
  SDL_FillRect(screen,NULL, SDL_MapRGB(screen->format, (pal[8]&0xFF0000)>>16,(pal[8]&0x00FF00)>>8,(pal[8]&0x0000FF)>>0));

}
#endif // __SDL12__

#ifdef __SDL20__
void SDL_GUI::clearScreen()
{
  extern int32_t * pal;
  DPRINTF("SDL_GUI::clearScreen()");
  //SDL_FillRect(screen,NULL, 0x000000);
  SDL_FillRect(screen,NULL, SDL_MapRGB(screen->format, (pal[8]&0xFF0000)>>16,(pal[8]&0x00FF00)>>8,(pal[8]&0x0000FF)>>0));
	//SDL_FillRect(screen,NULL, pal[8]);
}
#endif



int SDL_GUI::closeVideo()
{
  return 0;
  //SDL_Quit();
}


void SDL_GUI::loadingScreen()
{
  char txt[128];
  sprintf(txt,"Loading picoloop %s",PICOLOOP_VERSION);
  this->guiTTFText((SCREEN_WIDTH/2)*SCREEN_MULT,(SCREEN_HEIGHT/2)*SCREEN_MULT,txt);
  this->refresh();
}



void SDL_GUI::middleBox(int x,int y,Uint32 c)
{
  this->fullBox(x,y,(boxSize/2)*SCREEN_MULT,(boxSize/2)*SCREEN_MULT,c);
}

void SDL_GUI::middleBoxNumberUp(int x,int y,Uint32 c)
{
  //this->middleBox(35+15*x,60+15*y,c);
  //this->middleBox(35+15*x,30+15*y,c);
  //this->middleBox(35+15*x,OFFSET_Y_MIDDLE_BOX+15*y,c);
  this->middleBox((OFFSET_X_MIDDLEBOX_UP+15*x)*SCREEN_MULT,(OFFSET_Y_MIDDLEBOX_UP+15*y)*SCREEN_MULT,c);
}

void SDL_GUI::middleBoxNumberDown(int x,int y,Uint32 c)
{
  //this->middleBox(35+15*x,60+15*y,c);
  //this->middleBox(35+15*x,30+15*y,c);
  //this->middleBox(35+15*x,OFFSET_Y_MIDDLE_BOX+15*y,c);
  this->middleBox((OFFSET_X_MIDDLEBOX_DOWN+15*x)*SCREEN_MULT,(OFFSET_Y_MIDDLEBOX_DOWN+15*y)*SCREEN_MULT,c);
}
/*
void SDL_GUI::middleBoxNumber_second(int x,int y,Uint32 c)
{
  this->middleBox(35+15*x,60+15*y,c);
}
*/

 /*
void SDL_GUI::loadSaveBoxNumber(int x,int y,Uint32 c)
{
  this->middleBoxNumber(x,y,c);
}
 */

void SDL_GUI::drawTTFTextLoadSaveBoxNumberUp(int x,int y,const char *txt)
{
  //this->guiTTFText(35+15*x,60+15*y,txt);
  //this->guiTTFText(35+15*x,30+15*y,txt);
  //this->guiTTFText(35+15*x,OFFSET_Y_MIDDLE_BOX+15*y,txt);
  this->guiTTFText((OFFSET_X_MIDDLEBOX_UP+15*x)*SCREEN_MULT,(OFFSET_Y_MIDDLEBOX_UP+15*y)*SCREEN_MULT,txt);
} 


void SDL_GUI::drawTTFTextLoadSaveBoxNumberDown(int x,int y,const char *txt)
{
  //this->guiTTFText(35+15*x,60+15*y,txt);
  //this->guiTTFText(35+15*x,30+15*y,txt);
  //this->guiTTFText(35+15*x,OFFSET_Y_MIDDLE_BOX+15*y,txt);
  this->guiTTFText((OFFSET_X_MIDDLEBOX_DOWN+15*x)*SCREEN_MULT,(OFFSET_Y_MIDDLEBOX_DOWN+(15*y))*SCREEN_MULT,txt);
} 



void SDL_GUI::smallBox(int x,int y,Uint32 c)
{
  this->fullBox(x*SCREEN_MULT,
		y*SCREEN_MULT,
		(boxSize>>2)*SCREEN_MULT,
		(boxSize>>2)*SCREEN_MULT,
		c);
}



void SDL_GUI::emptyBox(int x, int y, int w, int h, Uint32 c)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;
  SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, (c&0xFF0000)>>16,(c&0x00FF00)>>8,(c&0x0000FF)>>0));
  
  r.x = x;
  r.y = y + h - 1;
  r.w = w;
  r.h = 1;
  SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, (c&0xFF0000)>>16,(c&0x00FF00)>>8,(c&0x0000FF)>>0));
  
  r.x = x;
  r.y = y + 1;
  r.w = 1;
  r.h = h - 2;
  SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, (c&0xFF0000)>>16,(c&0x00FF00)>>8,(c&0x0000FF)>>0));
	
  r.x = x + w - 1;
  r.y = y + 1;
  r.w = 1;
  r.h = h - 2;
  SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, (c&0xFF0000)>>16,(c&0x00FF00)>>8,(c&0x0000FF)>>0));
}

void SDL_GUI::fullBox(int x, int y, int w, int h, Uint32 c)
{
  SDL_Rect r;
  /*
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = 1;
    SDL_FillRect(screen, &r, c);
    
    r.x = x;
    r.y = y + h - 1;
    r.w = w;
    r.h = 1;
    SDL_FillRect(screen, &r, c);
    
    r.x = x;
    r.y = y + 1;
    r.w = 1;
    r.h = h - 2;
    SDL_FillRect(screen, &r, c);
    
    r.x = x + w - 1;
    r.y = y + 1;
    r.w = 1;
    r.h = h - 2;
    SDL_FillRect(screen, &r, c);
  */
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, (c&0xFF0000)>>16,(c&0x00FF00)>>8,(c&0x0000FF)>>0));  
}




/*
void SDL_GUI::drawText(int x, int y, string txt)
{
  DPRINTF("SDL_GUI::drawText\n");
}
*/

/*
void SDL_GUI::drawBox(int x, int y, int color)
{
    DPRINTF("SDL_GUI::drawBox\n");
}
*/


int SDL_GUI::openTTFFont()
{
  DPRINTF("SDL_GUI::openTTFFont");
  if( TTF_Init() == -1 )
    {
      return 0;
    }

  //Open the font
  //ttf_font = TTF_OpenFont("umd0:/font.ttf", 8*SCREEN_MULT ); <= fix issue on PSP
#if defined(PSVITA)
  ttf_font = TTF_OpenFont("ux0:/app/PICOLOOP1/font.ttf", 12);
#else
  ttf_font = TTF_OpenFont("font.ttf", FONTSIZE*SCREEN_MULT );
#endif
  //If there was an error in loading the font
  //  DPRINTF("%d\n",ttf_font);
  // exit(0);
  if( ttf_font == NULL || ttf_font==0 )
    {      
      DPRINTF("font not found or is not a font file ttf_font:0x%08.8X",(TTF_Font*)ttf_font);
      return false;
      //exit(1);
    }
  else
    DPRINTF("SDL_GUI::openTTFFont font successfully open:0x%08.8X",ttf_font);
  return true;  
}


void SDL_GUI::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip
)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


int SDL_GUI::guiTTFText(int x,int y,const char *txt)
{
  //SDL_Color textColor = { 54, 25, 255 };
  extern int32_t * pal;
  //SDL_Color textColor = { pal[7]&&0x0000FF, pal[7]&&0x00FF00, pal[7]&&0xFF0000 };
  //SDL_Color textColor = { pal[7], pal[7], pal[7] };
  //SDL_Color textColor = { (pal[7]&&0x0000FF), (pal[7]&&0x00FF00)>>8, (pal[7]&&0xFF0000)>>16 };

  SDL_Color textColor;
  SDL_Rect * clip = NULL;
  //textColor.r=pal[7]&0x0000FF;
  //textColor.g=(pal[7]&0x00FF00)>>8;
  //textColor.b=(pal[7]&0xFF0000)>>16;
  SDL_Color colorme = {255, 255, 255};
  textColor.r=(pal[7]&0xFF0000)>>16;
  textColor.g=(pal[7]&0x00FF00)>>8;
  textColor.b=(pal[7]&0x0000FF);
	

  //printf("textcolor : %d %d %d\n",textColor.r, textColor.g, textColor.b);

  if (message!=NULL)
    SDL_FreeSurface(message);
	#if   !defined(__VECTORFPU__)
  message=TTF_RenderText_Solid( ttf_font, txt, colorme );
  //message=TTF_RenderText_Solid( ttf_font, txt, textColor );
	#endif

	#if   defined(__VECTORFPU__) 
		message = TTF_RenderText_Blended( ttf_font, txt, textColor );
	#endif
  //apply_surface( 0, 0, background, screen );
  this->apply_surface( x, y, message, screen,clip);
  return 0;
}


#ifdef __SDL12__
SDL_Surface * SDL_GUI::loadBMP(const char *fn)
{
  SDL_Surface *cvt;
  SDL_Surface *img = SDL_LoadBMP(fn);
  if(!img)
    return NULL;
  cvt = SDL_DisplayFormat(img);
  SDL_FreeSurface(img);
  return cvt;
}
#endif //__SDL12__

#ifdef __SDL20__
SDL_Surface * SDL_GUI::loadBMP(const char *fn)
{
  // To be implemented, today loadBMP is not used
  return NULL;
}
#endif



void SDL_GUI::smallBoxNumber(int n,int x,int y,Uint32 c)
{
  switch(n)
    {
      //case 0: this->box(boxOffset+(0*boxSize)+((x*((boxSize*100)/(x+1)))/100),boxOffset,boxSize/4,boxSize/4,c); break;
      //case 0: this->box(boxOffset+(0*boxSize)+((4*x)/boxSize),boxOffset,boxSize/4,boxSize/4,c); break;
    case 0:  this->fullBox((boxOffset+(0*boxSize)+ 0+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 1:  this->fullBox((boxOffset+(1*boxSize)+10+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break; 
    case 2:  this->fullBox((boxOffset+(2*boxSize)+20+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 3:  this->fullBox((boxOffset+(3*boxSize)+30+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;

    case 4:  this->fullBox((boxOffset+(0*boxSize)+ 0+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(1*boxSize+10)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 5:  this->fullBox((boxOffset+(1*boxSize)+10+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(1*boxSize+10)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 6:  this->fullBox((boxOffset+(2*boxSize)+20+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(1*boxSize+10)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 7:  this->fullBox((boxOffset+(3*boxSize)+30+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(1*boxSize+10)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;

    case 8:  this->fullBox((boxOffset+(0*boxSize)+ 0+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(2*boxSize+20)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 9:  this->fullBox((boxOffset+(1*boxSize)+10+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(2*boxSize+20)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 10: this->fullBox((boxOffset+(2*boxSize)+20+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(2*boxSize+20)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 11: this->fullBox((boxOffset+(3*boxSize)+30+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(2*boxSize+20)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;

    case 12: this->fullBox((boxOffset+(0*boxSize)+ 0+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(3*boxSize+30)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 13: this->fullBox((boxOffset+(1*boxSize)+10+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(3*boxSize+30)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 14: this->fullBox((boxOffset+(2*boxSize)+20+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(3*boxSize+30)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;
    case 15: this->fullBox((boxOffset+(3*boxSize)+30+((6*x)/boxSize))*SCREEN_MULT,(boxOffset+(3*boxSize+30)+((6*y)/boxSize))*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,(boxSize/4)*SCREEN_MULT,c); break;

    }  
}


/*
  n: box number from 0 to 15
  c: color
 */
void SDL_GUI::drawBoxNumber(int n, Uint32 c)
{
  //sg.box(boxOffset+(0*boxSize),   boxOffset,boxSize,boxSize,0xAECD15);
  // DPRINTF("drawBoxNumber: %d %d",n,c);
  // assert(n>=0);
  // assert(n<=15);
  switch (n)
    {
    case 0:  this->fullBox((boxOffset+(0*boxSize) +0)*SCREEN_MULT,(boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 1:  this->fullBox((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 2:  this->fullBox((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 3:  this->fullBox((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;

    case 4:  this->fullBox((boxOffset+(0*boxSize) +0)*SCREEN_MULT,(boxOffset+(1*boxSize)+10)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 5:  this->fullBox((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(1*boxSize)+10)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 6:  this->fullBox((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(1*boxSize)+10)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 7:  this->fullBox((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(1*boxSize)+10)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;

    case 8:  this->fullBox((boxOffset+(0*boxSize) +0)*SCREEN_MULT,(boxOffset+(2*boxSize)+20)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 9:  this->fullBox((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(2*boxSize)+20)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 10: this->fullBox((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(2*boxSize)+20)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 11: this->fullBox((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(2*boxSize)+20)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
      
    case 12: this->fullBox((boxOffset+(0*boxSize) +0)*SCREEN_MULT,(boxOffset+(3*boxSize)+30)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 13: this->fullBox((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(3*boxSize)+30)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 14: this->fullBox((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(3*boxSize)+30)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    case 15: this->fullBox((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(3*boxSize)+30)*SCREEN_MULT,boxSize*SCREEN_MULT,boxSize*SCREEN_MULT,c); break;
    }               
}

/*
void SDL_GUI::drawTTFTextNumber(int n,char * str)
{
  switch(n)
    {
    case 0:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset,str); break;
    case 1:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset,str); break;
    case 2:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset,str); break;
    case 3:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset,str); break;

    case 4:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(1*boxSize+10),str); break;
    case 5:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),str); break;
    case 6:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),str); break;
    case 7:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),str); break;

    case 8:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(2*boxSize+20),str); break;
    case 9:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),str); break;
    case 10: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),str); break;
    case 11: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),str); break;
      
    case 12: this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(3*boxSize+30),str); break;
    case 13: this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),str); break;
    case 14: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),str); break;
    case 15: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),str); break;

    }
}
*/

void SDL_GUI::drawTTFTextNumberFirstLine(int n,const char * str)
{
  switch(n)
    {
    case 0:  this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0))*SCREEN_MULT,str); break;
    case 1:  this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0))*SCREEN_MULT,str); break;
    case 2:  this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0))*SCREEN_MULT,str); break;
    case 3:  this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0))*SCREEN_MULT,str); break;

    case 4:  this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(1*boxSize+10))*SCREEN_MULT,str); break;
    case 5:  this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(1*boxSize+10))*SCREEN_MULT,str); break;
    case 6:  this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(1*boxSize+10))*SCREEN_MULT,str); break;
    case 7:  this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(1*boxSize+10))*SCREEN_MULT,str); break;

    case 8:  this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(2*boxSize+20))*SCREEN_MULT,str); break;
    case 9:  this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(2*boxSize+20))*SCREEN_MULT,str); break;
    case 10: this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(2*boxSize+20))*SCREEN_MULT,str); break;
    case 11: this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(2*boxSize+20))*SCREEN_MULT,str); break;
      
    case 12: this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(3*boxSize+30))*SCREEN_MULT,str); break;
    case 13: this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(3*boxSize+30))*SCREEN_MULT,str); break;
    case 14: this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(3*boxSize+30))*SCREEN_MULT,str); break;
    case 15: this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(3*boxSize+30))*SCREEN_MULT,str); break;

    }
}

void SDL_GUI::drawTTFTextNumberSecondLine(int n,const char * str)
{
  switch(n)
    {
    case 0:  this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+10)*SCREEN_MULT,str); break;
    case 1:  this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+10)*SCREEN_MULT,str); break;
    case 2:  this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+10)*SCREEN_MULT,str); break;
    case 3:  this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(0*boxSize+ 0)+10)*SCREEN_MULT,str); break;

    case 4:  this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(1*boxSize+10)+10)*SCREEN_MULT,str); break;
    case 5:  this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(1*boxSize+10)+10)*SCREEN_MULT,str); break;
    case 6:  this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(1*boxSize+10)+10)*SCREEN_MULT,str); break;
    case 7:  this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(1*boxSize+10)+10)*SCREEN_MULT,str); break;

    case 8:  this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(2*boxSize+20)+10)*SCREEN_MULT,str); break;
    case 9:  this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(2*boxSize+20)+10)*SCREEN_MULT,str); break;
    case 10: this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(2*boxSize+20)+10)*SCREEN_MULT,str); break;
    case 11: this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(2*boxSize+20)+10)*SCREEN_MULT,str); break;
      
    case 12: this->guiTTFText((boxOffset+(0*boxSize)+ 0)*SCREEN_MULT,(boxOffset+(3*boxSize+30)+10)*SCREEN_MULT,str); break;
    case 13: this->guiTTFText((boxOffset+(1*boxSize)+10)*SCREEN_MULT,(boxOffset+(3*boxSize+30)+10)*SCREEN_MULT,str); break;
    case 14: this->guiTTFText((boxOffset+(2*boxSize)+20)*SCREEN_MULT,(boxOffset+(3*boxSize+30)+10)*SCREEN_MULT,str); break;
    case 15: this->guiTTFText((boxOffset+(3*boxSize)+30)*SCREEN_MULT,(boxOffset+(3*boxSize+30)+10)*SCREEN_MULT,str); break;

    }
}




void SDL_GUI::drawBMPTextNumber(int n,char * str)
{
  switch(n)
    {
    case 0:  this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset,str); break;
    case 1:  this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset,str); break;
    case 2:  this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset,str); break;
    case 3:  this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset,str); break;

    case 4:  this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset+(1*boxSize+10),str); break;
    case 5:  this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),str); break;
    case 6:  this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),str); break;
    case 7:  this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),str); break;

    case 8:  this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset+(2*boxSize+20),str); break;
    case 9:  this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),str); break;
    case 10: this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),str); break;
    case 11: this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),str); break;
      
    case 12: this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset+(3*boxSize+30),str); break;
    case 13: this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),str); break;
    case 14: this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),str); break;
    case 15: this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),str); break;

    }
}




int SDL_GUI::openBMPFont()
{
  bmp_font = this->loadBMP("font.bmp");
  if(!bmp_font)
    {
      DPRINTF("Couldn't load font!");
      return -1;
    } 
  return 0;
}


void SDL_GUI::guiBMPText(int x, int y, const char *txt)
{
  SDL_Surface *dst=this->screen;
  int sx = x;
  int sy = y;
  const char *stxt = txt;
  int highlights = 0;
  SDL_Rect sr;
  sr.w = FONT_CW;
  sr.h = FONT_CH;
  while(*txt)
    {
      int c = *txt++;
      switch(c)
	{
	case 0:	/* terminator */
	  break;
	case '\n':	/* newline */
	  x = sx;
	  y += FONT_CH;
	  break;
	case '\t':	/* tab */
	  x -= sx;
	  x += 8 * FONT_CW;
	  x %= 8 * FONT_CW;
	  x += sx;
	  break;
	case '\001':	/* red highlight */
	case '\002':	/* green highlight */
	case '\003':	/* yellow highlight */
	case '\004':	/* blue highlight */
	case '\005':	/* purple highlight */
	case '\006':	/* cyan highlight */
	case '\007':	/* white highlight */
	  highlights = 1;
	  if(*txt == '\001')
	    txt += 2;
	  break;
	case '\021':	/* red bullet */
	case '\022':	/* green bullet */
	case '\023':	/* yellow bullet */
	case '\024':	/* blue bullet */
	case '\025':	/* purple bullet */
	case '\026':	/* cyan bullet */
	case '\027':	/* white bullet */
	  {
	    SDL_Rect r;
	    int hlr = c & 1 ? 255 : 0;
	    int hlg = c & 2 ? 255 : 0;
	    int hlb = c & 4 ? 255 : 0;
	    Uint32 hlc = SDL_MapRGB(dst->format, hlr, hlg, hlb);
	    r.x = x;
	    r.y = y;
	    r.w = FONT_CW;
	    r.h = FONT_CH;
	    SDL_FillRect(dst, &r,
			 SDL_MapRGB(dst->format, 0, 0, 0));
	    //gui_dirty(&r);
	    r.x = x + 2;
	    r.y = y + 2;
	    r.w = FONT_CW - 6;
	    r.h = FONT_CH - 6;
	    SDL_FillRect(dst, &r, hlc);
	    x += FONT_CW;
	    break;
	  }
	default:	/* printables */
	  {
	    SDL_Rect dr;
	    if(c < ' ' || c > 127)
	      c = 127;
	    c -= 32;
	    sr.x = (c % (bmp_font->w / FONT_CW)) * FONT_CW;
	    sr.y = (c / (bmp_font->w / FONT_CW)) * FONT_CH;
	    dr.x = x;
	    dr.y = y;
	    SDL_BlitSurface(bmp_font, &sr, dst, &dr);
	    //gui_dirty(&dr);
	    x += FONT_CW;
	    break;
	  }
	}
    }
  if(!highlights)
    return;
  x = sx;
  y = sy;
  txt = stxt;
  while(*txt)
    {
      int c = *txt++;
      switch(c)
	{
	case 0:	/* terminator */
	  break;
	case '\n':	/* newline */
	  x = sx;
	  y += FONT_CH;
	  break;
	case '\t':	/* tab */
	  x -= sx;
	  x += 8 * FONT_CW;
	  x %= 8 * FONT_CW;
	  x += sx;
	  break;
	case '\001':	/* red highlight */
	case '\002':	/* green highlight */
	case '\003':	/* yellow highlight */
	case '\004':	/* blue highlight */
	case '\005':	/* purple highlight */
	case '\006':	/* cyan highlight */
	case '\007':	/* white highlight */
	  {
	    int hlr = c & 1 ? 255 : 0;
	    int hlg = c & 2 ? 255 : 0;
	    int hlb = c & 4 ? 255 : 0;
	    Uint32 hlc = SDL_MapRGB(screen->format, hlr, hlg, hlb);
	    int hlw = 1;
	    if(*txt == '\001')
	      {
		hlw = txt[1];
		txt += 2;
	      }
	    /*
	    gui_box(x - 2, y - 2,
		    FONT_CW * hlw + 2, FONT_CH + 2,
		    hlc, dst);
	    */
	    break;
	  }
	default:	/* printables */
	  x += FONT_CW;
	  break;
	}
    }
}



