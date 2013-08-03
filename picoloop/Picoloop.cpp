#include "SDL_GUI.h"
#include <unistd.h>

SDL_GUI sg;
int boxSize=30;
int boxOffset=20;

int main()
{
  sg.initVideo();
  
  sg.box(boxOffset+(0*boxSize),   boxOffset,boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(1*boxSize)+10,boxOffset,boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(2*boxSize)+20,boxOffset,boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(3*boxSize)+30,boxOffset,boxSize,boxSize,0xAECD15);

  sg.box(boxOffset+(0*boxSize),   boxOffset+(1*boxSize+10),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),boxSize,boxSize,0xAECD15);

  sg.box(boxOffset+(0*boxSize),   boxOffset+(2*boxSize+20),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),boxSize,boxSize,0xAECD15);

  sg.box(boxOffset+(0*boxSize),   boxOffset+(3*boxSize+30),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),boxSize,boxSize,0xAECD15);
  sg.box(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),boxSize,boxSize,0xAECD15);

  sg.refresh();

  sleep(3);
  sg.closeVideo();
  return(0);


}
