#include "SDL_GUI.h"




SDL_GUI::SDL_GUI()
{
  printf("construction SDL_GUI::SDL_GUI()\n");
  screen=NULL;
}

SDL_GUI::~SDL_GUI()
{
  printf("destructor SDL_GUI::SDL_GUI()\n");
}


int SDL_GUI::initVideo()
{
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      return 0;
    }
  
  printf("SDL initialized.\n");
  
  screen = SDL_SetVideoMode(SCREEN_WIDTH, 
			    SCREEN_HEIGHT, 
			    SCREEN_DEPTH,			
			    SDL_SWSURFACE|SDL_DOUBLEBUF);
  
  if (screen == NULL)
    {
      fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
      SDL_Quit();
      return 0;
    }  
  return 1;
}


int SDL_GUI::closeVideo()
{
  SDL_Quit();
}

void SDL_GUI::drawText(int x, int y, string txt)
{
  printf("SDL_GUI::drawText\n");
}

void SDL_GUI::drawBox(int x, int y, int color)
{
    printf("SDL_GUI::drawBox\n");
}



void SDL_GUI::box(int x, int y, int w, int h, Uint32 c)
{
	SDL_Rect r;
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

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	SDL_FillRect(screen, &r, c);

}

void SDL_GUI::refresh()
{
  SDL_Flip(screen);
}
