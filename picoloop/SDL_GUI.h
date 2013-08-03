#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <string>

#include "Master.h"

#ifndef __SDL_GUI__
#define __SDL_GUI__

using namespace std;

class SDL_GUI
{
 public:
  SDL_GUI();
  ~SDL_GUI();
  int initVideo();
  int closeVideo();
  void drawText(int x,int y,string txt);
  void drawBox(int x,int y,int color);
  void box(int x, int y, int w, int h, Uint32 c);
  void refresh();
 private:
  SDL_Surface *screen;
  
  
};

#endif
