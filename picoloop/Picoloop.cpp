#include "SDL_GUI.h"
#include <unistd.h>

SDL_GUI sg;
SDL_Surface * sdls;
int32_t pal0[MAXCOLOR]={0x0,      0xC8A0A8, 0x59AADA, 0xFFA11C, 0x39CE90, 0x36B82B, 0x3E789A, 0xFFFFFF, 0x0 };
// blue:
int32_t pal1[MAXCOLOR]={0x3088B0, 0xB7E4F8, 0x236481, 0x184D63, 0xE8D100, 0x3DBFF4, 0xFF8000, 0xFFFFFF, 0x3088B0 };
// automn:
int32_t pal2[MAXCOLOR]={0x633A17, 0x55310E, 0x925518, 0xCF7B30, 0xE16700, 0xC00000, 0xFAA46A, 0xEEEEEE, 0x633A17 };
// grey:
int32_t pal3[MAXCOLOR]={0x777777, 0xDDDDDD, 0x444444, 0x222222, 0x555555, 0x777777, 0x888888, 0xEEEEEE, 0x777777 };
// grey nanoloop:
int32_t pal4[MAXCOLOR]={0xE3E7E3, 0xA8A8A8, 0x969696, 0xA4A4A4, 0xFFFFFF, 0x767676, 0x222222, 0x222222, 0xE3E7E3 };
//// old grey nanoloop:
//// int32_t pal4[MAXCOLOR]={0x882288, 0xA8A8A8, 0x969696, 0xA4A4A4, 0xC9C9C9, 0x767676, 0x222222, 0x222222, 0xE3E7E3 };
// yellow gameboy nanoloop:
int32_t pal5[MAXCOLOR]={0xF6F6C5, 0xABA75E, 0x969266, 0xAEAD70, 0xFFFFDD, 0x7E7D49, 0x282A20, 0x282A20, 0xF6F6C5 };

int32_t *pal=pal0;
int      menu_config_palette=0;


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
