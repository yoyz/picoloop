#include <stdio.h>
#include "IO.h"

using namespace std ;



int main()
{
  IO mIO;
  int mScreenHeight = mIO.GetScreenHeight();
  int x=0;
  int y=0;
  int loop=0;
  printf("hello");
  while (!mIO.IsKeyDown (SDLK_ESCAPE))
    {
      // ----- Draw -----
      
      mIO.ClearScreen (); 		// Clear screen


      mIO.DrawText("Hi",x,y);
      int mKey = mIO.Pollkey();
      //sleep(1);
      mIO.UpdateScreen ();		// Put the graphic context in the screen
      printf("x : %d y : %d loop: %d\n",x,y,loop);
      
      
      switch (mKey)
	{
	case (SDLK_LEFT):
	  {	    x--; break;	  }

	case (SDLK_RIGHT): 
	  {	    x++; break;   }

	case (SDLK_UP): 
	  {	    y--; break;   }

	case (SDLK_DOWN): 
	  {	    y++; break;	  }
	  
	}
      //sleep(1);
      loop++;
    }
  return 0;
}
	


