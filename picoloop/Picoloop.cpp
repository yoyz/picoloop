#include "SDL_GUI.h"
#include <unistd.h>

SDL_GUI sg;

int main()
{
  int i;
  int j;
  sg.initVideo();
  sg.openFont();
  /*
  */
  for (i=0;i<16;i++)
    {
      sg.drawBoxNumber(i,0xAECD15);
    }
  sg.refresh();
  for (j=0;j<1000;j++)
    for (i=0;i<16;i++)
      {
	sg.drawBoxNumber(i,0x045c15);
	if (i > 0)  sg.drawBoxNumber(i-1,0xAECD15);
	if (i == 0) sg.drawBoxNumber(15,0xAECD15);
	sg.guiText(20,24,"C4");
	sg.refresh();
	//usleep(100);
	SDL_Delay(100);
	//usleep(400000);
	//sleep(1);o
	printf("%d\n",i);

      }
  
  sleep(1);
  sg.closeVideo();
  return(0);


}
