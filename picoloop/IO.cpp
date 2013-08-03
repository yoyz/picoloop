/*****************************************************************************************
/* File: IO.cpp
/* Desc: Class for handling inpung & drawing, it uses SDL for the rendering. Change the methods 
/* of this class in order to use a different renderer
/*       
/*
/* gametuto.com - Javier López López (javilop.com)
/*
/*****************************************************************************************
/*
/* Creative Commons - Attribution 3.0 Unported
/* You are free:
/*	to Share — to copy, distribute and transmit the work
/*	to Remix — to adapt the work
/*
/* Under the following conditions:
/* Attribution. You must attribute the work in the manner specified by the author or licensor 
/* (but not in any way that suggests that they endorse you or your use of the work).
/*
/*****************************************************************************************/

// ------ Includes -----

#include "IO.h"

static SDL_Surface *mScreen;										// Screen
static Uint32 mColors [COLOR_MAX] = {0x000000ff,					// Colors
                               0xff0000ff, 0x00ff00ff, 0x0000ffff,
                               0x00ffffff, 0xff00ffff, 0xffff00ff,
                               0xffffffff};
SDL_Color foregroundColor = { 255, 255, 255 };
SDL_Color backgroundColor = { 0, 0, 255 };


TTF_Font *font;
/* 
======================================									
Init
====================================== 
*/
IO::IO() 
{
  OpenVideoSDL();
  OpenTextSDL();
}




/* 
======================================									
Clear the screen to black
====================================== 
*/
void IO::ClearScreen() 
{
	boxColor (mScreen, 
		  0, 
		  0, 
		  mScreen->w - 1, 
		  mScreen->h - 1, 
		  mColors[BLACK]);
}


/* 
======================================									
Draw a rectangle of a given color

Parameters:
>> pX1, pY1: 		Upper left corner of the rectangle
>> pX2, pY2: 		Lower right corner of the rectangle
>> pC				Rectangle color
====================================== 
*/
void IO::DrawRectangle (int pX1, int pY1, int pX2, int pY2, enum color pC)
{
	boxColor (mScreen, pX1, pY1, pX2, pY2-1, mColors[pC]);
}


/* 
======================================									
Return the screen height
====================================== 
*/
int IO::GetScreenHeight()
{
	return mScreen->h;
}


/* 
======================================									
Update screen
====================================== 
*/
void IO::UpdateScreen()
{
  SDL_Flip(mScreen);
}

void IO::DrawText(string text,int x,int y)
{
  int length=text.length();
  SDL_Rect textLocation;  
  SDL_Surface* textSurface = TTF_RenderText_Shaded(font, text.c_str(),foregroundColor, backgroundColor);
  textLocation.x=x*16;
  textLocation.y=y*16;
  textLocation.w=length*16;
  textLocation.h=16+y*16;
  SDL_BlitSurface(textSurface, NULL, mScreen, &textLocation);
}


/* 
======================================									
Keyboard Input
====================================== 
*/
int IO::Pollkey()
{
	SDL_Event event;
	while ( SDL_PollEvent(&event) ) 
	{
		switch (event.type) {
			case SDL_KEYDOWN:
				return event.key.keysym.sym;
			case SDL_QUIT:
				exit(3);
		}
	}
	return -1;
}

/* 
======================================									
Keyboard Input
====================================== 
*/
int IO::Getkey()
{
	SDL_Event event;
	while (true)
	{
	  SDL_WaitEvent(&event);
	  if (event.type == SDL_KEYDOWN)
		  break;
      if (event.type == SDL_QUIT)
		  exit(3);
	};
	return event.key.keysym.sym;
}

/* 
======================================									
Keyboard Input
====================================== 
*/
int IO::IsKeyDown (int pKey)
{
	Uint8* mKeytable;
	int mNumkeys;
	SDL_PumpEvents();
	mKeytable = SDL_GetKeyState(&mNumkeys);
	return mKeytable[pKey];
}




/* 
======================================									
SDL Text Initialization
====================================== 
*/

int IO::OpenTextSDL()
{
  
  TTF_Init();
  font = TTF_OpenFont("ARIAL.TTF", 14);
}


/* 
======================================									
SDL Graphical Initialization
====================================== 
*/
int IO::OpenVideoSDL()
{
	const SDL_VideoInfo *info;
	Uint8  video_bpp;
	Uint32 videoflags;
        
	// Initialize SDL
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	  {
	    fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
	    return 1;
	  }
	atexit(SDL_Quit);
	
	// Alpha blending doesn't work well at 8-bit color
	info = SDL_GetVideoInfo();
	if ( info->vfmt->BitsPerPixel > 8 ) {
		video_bpp = info->vfmt->BitsPerPixel;
	} else {
		video_bpp = 16;
	}
	videoflags = SDL_SWSURFACE | SDL_DOUBLEBUF;
	
	// Set 640x480 video mode
	if ( (mScreen=SDL_SetVideoMode(640,480,video_bpp,videoflags)) == NULL ) {
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n",640,480,SDL_GetError());
		return 2;
	}
    return 0;
}


int IO::CloseTextSDL()
{
  TTF_CloseFont(font);
  TTF_Quit();
}

int IO::CloseVideoSDL()
{
  SDL_Quit();
}

