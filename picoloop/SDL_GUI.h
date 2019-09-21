#include <string>


#ifdef   __SDL12__
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_audio.h>
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#endif // __SDL12__

#ifdef  __SDL20__
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
typedef struct SDL_Window SDL_Window;


#endif // __SDL20__


#include "Master.h"
#ifndef __SDL_GUI__
#define __SDL_GUI__

#define   FONT_CW         16
#define   FONT_CH         16

#define   OFFSET_X_MIDDLEBOX_UP   35
#define   OFFSET_Y_MIDDLEBOX_UP   30

#define   OFFSET_X_MIDDLEBOX_DOWN 35
#define   OFFSET_Y_MIDDLEBOX_DOWN 160


#define   OFFSET_X_PATTERN_CURSOR 70
#define   OFFSET_Y_PATTERN_CURSOR 220

#define   OFFSET_X_PATTERN_STEP   70
#define   OFFSET_Y_PATTERN_STEP   230


class SDL_GUI
{
 public:
  SDL_GUI();
  ~SDL_GUI();
  int  initVideo();

  int  closeVideo();

  void loadingScreen();

  void middleBox(int x, int y,Uint32 c);
  void middleBoxNumberUp(int x, int y,Uint32 c);
  void middleBoxNumberDown(int x, int y,Uint32 c);

  //void loadSaveBoxNumber(int x, int y,Uint32 c);
  void drawTTFTextLoadSaveBoxNumberUp(int x,int y,const char *txt);
  void drawTTFTextLoadSaveBoxNumberDown(int x,int y,const char *txt);

  void smallBox(int x, int y,Uint32 c);
  void smallBoxNumber(int n,int x,int y,Uint32 c);

  void fullBox(int x, int y, int w, int h, Uint32 c);
  void emptyBox(int x, int y, int w, int h, Uint32 c);
  //  void drawBox(int x,int y,Uint32 color);
  void drawBoxNumber(int n,Uint32 color);

  int  openBMPFont();
  void guiBMPText(int x, int y, const char *txt);
  void drawBMPText(int x,int y,std::string txt);
  void drawBMPTextNumber(int n,char *txt);

  int  openTTFFont();
  int  guiTTFText(int x,int y,const char *txt);
  void drawTTFTextNumberFirstLine(int n,const char *txt);
  void drawTTFTextNumberSecondLine(int n,const char *txt);
  //  void drawTTFTextNumber(int n,char *txt);

  void refresh();
  void clearScreen();

  void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

#ifdef __SDL20__
  SDL_Window * window;
  SDL_Joystick* gGameController = NULL;
  SDL_Haptic* gControllerHaptic = NULL;
#endif
  SDL_Surface * screen;


 private:  
  SDL_Surface * loadBMP(const char * fn);
  SDL_Surface * bmp_font;

  TTF_Font    * ttf_font;

  SDL_Surface * message;

  int boxSize;
  int boxOffset;


};

#endif
