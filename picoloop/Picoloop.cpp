#include "SDL_GUI.h"
#include <unistd.h>

SDL_GUI sg;

int main()
{
  int i=0;
  int j=0;
  int k=0;
  int l=0;
  sg.initVideo();
  sg.openFont();
  /*
  */
  for (l=0;l<16;l++)
    {
      sg.drawBoxNumber(l,0xAECD15);
    }
  sg.refresh();
  for (j=0;j<1000;j++)
    for (i=0;i<16;i++)
      {
	for (l=0;l<16;l++) 
	  sg.drawBoxNumber(l,0xAECD15);

	if (i > 0)  sg.drawBoxNumber(i-1,0xAECD15);
	if (i == 0) sg.drawBoxNumber(15,0xAECD15);
	sg.drawBoxNumber(i,0x045c15);


	sg.smallBoxNumber(0,k,k,0x443322);
	sg.smallBoxNumber(1,k,k,0x443322);
	sg.smallBoxNumber(2,k,k,0x443322);
	sg.smallBoxNumber(3,k,k,0x443322);

	sg.smallBoxNumber(4,k,k,0x443322);
	sg.smallBoxNumber(5,k,k,0x443322);
	sg.smallBoxNumber(6,k,k,0x443322);
	sg.smallBoxNumber(7,k,k,0x443322);

	sg.smallBoxNumber(8,k,k,0x443322);
	sg.smallBoxNumber(9,k,k,0x443322);
	sg.smallBoxNumber(10,k,k,0x443322);
	sg.smallBoxNumber(11,k,k,0x443322);

	sg.smallBoxNumber(12,k,k,0x443322);
	sg.smallBoxNumber(13,k,k,0x443322);
	sg.smallBoxNumber(14,k,k,0x443322);
	sg.smallBoxNumber(15,k,k,0x443322);

	//sg.guiText(20,24,"C4");
	sg.refresh();
	//usleep(100);
	SDL_Delay(100);
	//usleep(400000);
	//sleep(1);o
	printf("%d %d %d\n",i,j,k);
	k++;
      }
  
  sleep(1);
  sg.closeVideo();
  return(0);


}
