#ifndef __PICOSYSMUTEX__
#define __PICOSYSMUTEX__


/*
 *  SysMutex.h
 *  lgpt
 *
 *  Created by Marc Nostromo on 10/03/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

//#include <SDL/SDL.h>

#ifdef   __SDL12__
#include <SDL/SDL.h>
#endif // __SDL12__

#ifdef  __SDL20__
#include <SDL2/SDL.h>
#endif // __SDL20__


class SysMutex {
public:
	SysMutex() ;
	~SysMutex() ;
	bool Lock() ;
	void Unlock() ;
private:
	SDL_mutex *mutex_ ;
} ;

class SysMutexLocker {
public:
	SysMutexLocker(SysMutex &mutex) ;
	~SysMutexLocker() ;
private:
	SysMutex *mutex_ ;
} ;

#endif
