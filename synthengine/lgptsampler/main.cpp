#include "SampleInstrument.h"
#include "SamplePool.h"
#include "FileSystem.h"
#include "UnixFileSystem.h"
#include <libgen.h>
#include "DEBSystem.h"

int main(int argc, char **argv)
{
  int i;
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
  
  SampleInstrument SI;
  SamplePool       SP;
  int * buffer=(int*)malloc(sizeof(int)*1024);
  

  printf("\n",SP.GetNameListSize());
  printf("<<<<%d>>>>>\n",SP.GetNameListSize());
  SP.Load();
  printf("<<<<%d>>>>>\n",SP.GetNameListSize());
  //SP.loadSample(
  SI.Init();
  SI.AssignSample(1);
  
  printf("<<<<SP SOURCE:%d>>>>>\n",SP.GetSource(0));
  printf("<<<<SP SAMPLEINDEX:%d>>>>>\n",SI.GetSampleIndex());

  SI.Start(0,64,true);
  SI.Render(0,buffer,1024,true);
  for(i=0;i<128;i++)
    {
      printf("%d\t",buffer[i]);
    }

  
}
