#include "SDL_GUI.h"
#include <unistd.h>

SDL_GUI sg;
SDL_Surface * sdls;

int main()
{
  int i=0;
  int j=0;
  int k=0;
  int l=0;

  sdls=(SDL_Surface*)malloc(sizeof(int)*8*8);

  if (sg.initVideo()==0)
    exit(1);
  //  sg.initTTF();
  sg.openBMPFont();
  sg.openTTFFont();
  /*
  */
  for (l=0;l<16;l++)
    {
      sg.drawBoxNumber(l,0xAECD15);
    }
  sg.refresh();
  for (j=0;j<10;j++)
    for (i=0;i<16;i++)
      {
	for (l=0;l<16;l++) 
	  sg.drawBoxNumber(l,0xAECD15);


	if (i > 0)  sg.drawBoxNumber(i-1,0xAECD15);
	if (i == 0) sg.drawBoxNumber(15,0xAECD15);
	sg.drawBoxNumber(i,0x045c15);

	sg.drawTTFTextNumber(0,"C3");
	sg.drawTTFTextNumber(1,"C3");
	sg.drawTTFTextNumber(2,"C3");
	sg.drawTTFTextNumber(3,"C3");
	sg.drawTTFTextNumber(4,"C3");
	sg.drawTTFTextNumber(5,"C3");
	sg.drawTTFTextNumber(6,"C3");
	sg.drawTTFTextNumber(7,"C3");
	sg.drawTTFTextNumber(8,"C3");
	sg.drawTTFTextNumber(9,"C3");
	sg.drawTTFTextNumber(10,"C3");
	sg.drawTTFTextNumber(11,"C3");
	sg.drawTTFTextNumber(12,"C3");
	sg.drawTTFTextNumber(13,"C3");
	sg.drawTTFTextNumber(14,"C3");
	sg.drawTTFTextNumber(15,"C3");

	sg.smallBoxNumber(0 ,k,0,0x443322);
	sg.smallBoxNumber(1 ,k,0,0x443322);
	sg.smallBoxNumber(2 ,k,0,0x443322);
	sg.smallBoxNumber(3 ,k,0,0x443322);

	sg.smallBoxNumber(4 ,k,0,0x443322);
	sg.smallBoxNumber(5 ,k,0,0x443322);
	sg.smallBoxNumber(6 ,k,0,0x443322);
	sg.smallBoxNumber(7 ,k,0,0x443322);

	sg.smallBoxNumber(8 ,k,0,0x443322);
	sg.smallBoxNumber(9 ,k,0,0x443322);
	sg.smallBoxNumber(10,k,0,0x443322);
	sg.smallBoxNumber(11,k,0,0x443322);

	sg.smallBoxNumber(12,k,0,0x443322);
	sg.smallBoxNumber(13,k,0,0x443322);
	sg.smallBoxNumber(14,k,0,0x443322);
	sg.smallBoxNumber(15,k,0,0x443322);

	sg.guiTTFText(0,0,"AA");

	//sg.guiText(20,24,"C4");
	sg.refresh();
	//usleep(100);
	SDL_Delay(1);
	//usleep(400000);
	//sleep(1);o
	//printf("%d %d %d\n",i,j,k);
	k++;
      }
  
  sleep(1);
  sg.closeVideo();
  return(0);


}
