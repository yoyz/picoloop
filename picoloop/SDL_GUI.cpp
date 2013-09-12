#include "SDL_GUI.h"




SDL_GUI::SDL_GUI()
{
  printf("construction SDL_GUI::SDL_GUI()\n");
  screen=NULL;
  font=NULL;
  boxSize=30;
  boxOffset=20;
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
/*
void SDL_GUI::drawBox(int x, int y, int color)
{
    printf("SDL_GUI::drawBox\n");
}
*/



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

void SDL_GUI::drawTextNumber(int n,char * str)
{
  switch(n)
    {
    case 0:  this->guiText(boxOffset+(0*boxSize),boxOffset,str); break;
    case 1:  this->guiText(boxOffset+(1*boxSize)+10,boxOffset,str); break;
    case 2:  this->guiText(boxOffset+(2*boxSize)+20,boxOffset,str); break;
    case 3:  this->guiText(boxOffset+(3*boxSize)+30,boxOffset,str); break;

    case 4:  this->guiText(boxOffset+(0*boxSize),   boxOffset+(1*boxSize+10),str); break;
    case 5:  this->guiText(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),str); break;
    case 6:  this->guiText(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),str); break;
    case 7:  this->guiText(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),str); break;

    case 8:  this->guiText(boxOffset+(0*boxSize),   boxOffset+(2*boxSize+20),str); break;
    case 9:  this->guiText(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),str); break;
    case 10: this->guiText(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),str); break;
    case 11: this->guiText(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),str); break;
      
    case 12: this->guiText(boxOffset+(0*boxSize),   boxOffset+(3*boxSize+30),str); break;
    case 13: this->guiText(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),str); break;
    case 14: this->guiText(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),str); break;
    case 15: this->guiText(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),str); break;

    }
}


void SDL_GUI::drawBoxNumber(int n, Uint32 c)
{
  //sg.box(boxOffset+(0*boxSize),   boxOffset,boxSize,boxSize,0xAECD15);

  switch (n)
    {
    case 0:  this->box(boxOffset+(0*boxSize),   boxOffset,boxSize,boxSize,c); break;
    case 1:  this->box(boxOffset+(1*boxSize)+10,boxOffset,boxSize,boxSize,c); break;
    case 2:  this->box(boxOffset+(2*boxSize)+20,boxOffset,boxSize,boxSize,c); break;
    case 3:  this->box(boxOffset+(3*boxSize)+30,boxOffset,boxSize,boxSize,c); break;

    case 4:  this->box(boxOffset+(0*boxSize),   boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;
    case 5:  this->box(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;
    case 6:  this->box(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;
    case 7:  this->box(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;

    case 8:  this->box(boxOffset+(0*boxSize),   boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
    case 9:  this->box(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
    case 10: this->box(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
    case 11: this->box(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
      
    case 12: this->box(boxOffset+(0*boxSize),   boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    case 13: this->box(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    case 14: this->box(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    case 15: this->box(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    }               
}



SDL_Surface * SDL_GUI::gui_load_image(const char *fn)
{
  SDL_Surface *cvt;
  SDL_Surface *img = SDL_LoadBMP(fn);
  if(!img)
    return NULL;
  cvt = SDL_DisplayFormat(img);
  SDL_FreeSurface(img);
  return cvt;
}



int SDL_GUI::openFont()
{
  font = gui_load_image("font.bmp");
  if(!font)
    {
      fprintf(stderr, "Couldn't load font!\n");
      return -1;
    } 
  return 0;
}


void SDL_GUI::guiText(int x, int y, const char *txt)
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
	    sr.x = (c % (font->w / FONT_CW)) * FONT_CW;
	    sr.y = (c / (font->w / FONT_CW)) * FONT_CH;
	    dr.x = x;
	    dr.y = y;
	    SDL_BlitSurface(font, &sr, dst, &dr);
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



void SDL_GUI::refresh()
{
  SDL_Flip(screen);
}


