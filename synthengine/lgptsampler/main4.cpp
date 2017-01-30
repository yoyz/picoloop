#include "SampleInstrument.h"
#include "SamplePool.h"
//#include "FileSystem.h"
//#include "UnixFileSystem.h"
#include <libgen.h>
#include "DEBSystem.h"

int main(int argc, char **argv)
{
  int i,j;
  FILE    * FD; 
  DEBSystem DEB;
  DEB.Boot(argc,argv);
  //DEBSystem::Boot
  //char buff[1024];
  //strcpy(buff,".");
  //Path::SetAlias("bin",dirname(buff)) ;
  //Path::SetAlias("root","bin:..") ;
  Path::SetAlias("samples","samples") ;
  //FileSystem::Install(new UnixFileSystem()) ;
  //Path::SetAlias("bin",dirname(buff)) ;
  //Path::SetAlias("root","bin:..") ;
  //Path logPath("bin:lgpt.log");
  
  //SampleInstrument SI;
  
  //SamplePool       SP;
  //SampleInstrument * SI=SampleInstrument::GetInstance();
  SampleInstrument SI;
  SamplePool * SP=SamplePool::GetInstance();
  //SP->Init();
  //int * buffer=(int*)malloc(sizeof(int)*1024);
  fixed * buffer=(fixed*)malloc(sizeof(fixed)*1024);
  int16_t * buffer16=(int16_t*)malloc(sizeof(int16_t)*1024);
  //typedef char MY_TYPE;
  //MY_TYPE *buffer8 = (MY_TYPE *) buffer;
  
  printf("<<<<SP->GetNameListSize()=%d>>>>>\n",SP->GetNameListSize());
  SP->Load();
  printf("SP->Load()\n");
  printf("<<<<SP.GetNameListSize()=%d>>>>>\n",SP->GetNameListSize());
  for (i=0;i<SP->GetNameListSize();i++)
    {
      char ** tab_str=SP->GetNameList();
      printf("%s\n",tab_str[i]);
    }
  SI.Init();
  SI.AssignSample(1);
  
  printf("<<<<SP->GetSource(0)=%d>>>>>\n",SP->GetSource(0));
  printf("<<<<SP->GetSource(1)=%d>>>>>\n",SP->GetSource(1));
  printf("<<<<SI.GetSampleIndex()=%d>>>>>\n",SI.GetSampleIndex());

  FD=fopen("audioout","w+");
      SI.ProcessCommand(0,MAKE_FOURCC('T','R','S','P'),j);

      SI.ProcessCommand(0,MAKE_FOURCC('L','S','T','A'),0x10); 
      SI.ProcessCommand(0,MAKE_FOURCC('L','L','E','N'),0x11);

      SI.ProcessCommand(0,MAKE_FOURCC('L','M','O','D'),0x2);
      SI.ProcessCommand(0,MAKE_FOURCC('S','T','R','T'),0x1);
      SI.ProcessCommand(0,MAKE_FOURCC('E','N','D','_'),0x2);
    
  SI.Start(0,48,true);
  //SI.Render(0,buffer,1024,true);
  for (j=0;j<1024;j++)
    {
      if (j==32)
	SI.Start(0,72,true);
      //SI.ProcessCommand(0,MAKE_FOURCC('F','C','U','T'),0x7F);
      SI.ProcessCommand(0,MAKE_FOURCC('F','C','U','T'),0xFF);
      SI.ProcessCommand(0,MAKE_FOURCC('F','R','E','S'),0);


      SI.ProcessCommand(0,MAKE_FOURCC('D','S','P','L'),0);

      SI.ProcessCommand(0,MAKE_FOURCC('V','O','L','M'),0x7F);
      SI.ProcessCommand(0,MAKE_FOURCC('C','R','S','H'),0); 

      SI.Render(0,buffer,512,true);
      
      for (i=0;i<1024;i++)
	buffer[i]=fp2i(buffer[i]);
      
      for (i=0;i<1024;i++)
	buffer16[i]=buffer[i];

      fwrite(buffer16,sizeof(int16_t),1024,FD);
      /*
      for(i=0;i<1024;i++)
	{
	  printf("0x%08.8X\t",buffer16[i]);
	}
      */

    }
    free(buffer);
    SP->Reset();
    fclose(FD);
  
}
