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

	sg.drawTTFTextNumberFirstLine(0,"C3");
	sg.drawTTFTextNumberFirstLine(1,"C3");
	sg.drawTTFTextNumberFirstLine(2,"C3");
	sg.drawTTFTextNumberFirstLine(3,"C3");
	sg.drawTTFTextNumberFirstLine(4,"C3");
	sg.drawTTFTextNumberFirstLine(5,"C3");
	sg.drawTTFTextNumberFirstLine(6,"C3");
	sg.drawTTFTextNumberFirstLine(7,"C3");
	sg.drawTTFTextNumberFirstLine(8,"C3");
	sg.drawTTFTextNumberFirstLine(9,"C3");
	sg.drawTTFTextNumberFirstLine(10,"C3");
	sg.drawTTFTextNumberFirstLine(11,"C3");
	sg.drawTTFTextNumberFirstLine(12,"C3");
	sg.drawTTFTextNumberFirstLine(13,"C3");
	sg.drawTTFTextNumberFirstLine(14,"C3");
	sg.drawTTFTextNumberFirstLine(15,"C3");

	for (l=0;l<16;l++) 
	  sg.smallBoxNumber(l ,k,0,0x443322);

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

  //clear all and redisplay
  sg.clearScreen();
  sg.refresh();

  int x,y;
  const char * txt="0";
  const char * tmp_txt;
  tmp_txt="0";

  for (x=0;x<16;x++)
    for (y=0;y<8;y++)
      sg.middleBoxNumberUp(x,y,0x46DC65);

  // for (x=0;x<16;x++)
  //   for (y=0;y<8;y++)
  //     sg.drawTTFTextNumberFirstLine(x,tmp_txt);


  sg.refresh();
  sleep(4);
  sg.closeVideo();

  return(0);


}
