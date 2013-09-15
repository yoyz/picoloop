#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <string>

#include "Master.h"

#ifndef __SDL_GUI__
#define __SDL_GUI__

#define   FONT_CW         16
#define   FONT_CH         16

using namespace std;

class SDL_GUI
{
 public:
  SDL_GUI();
  ~SDL_GUI();
  int  initVideo();
  int  closeVideo();
  void drawText(int x,int y,string txt);

  void smallBoxNumber(int n,int x,int y,Uint32 c);
  void smallBox(int x, int y,Uint32 c);
  void box(int x, int y, int w, int h, Uint32 c);
  //  void drawBox(int x,int y,Uint32 color);
  void drawBoxNumber(int n,Uint32 color);
  void drawTextNumber(int n,char *txt);
  int  openFont();
  void guiText(int x, int y, const char *txt);

  void refresh();
 private:
  SDL_Surface * gui_load_image(const char * fn);
  SDL_Surface * screen;
  SDL_Surface * font;
  int boxSize;
  int boxOffset;

};

#endif
