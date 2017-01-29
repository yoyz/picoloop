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
  //SamplePool       SP;
  SamplePool * SP=SamplePool::GetInstance();
  int * buffer=(int*)malloc(sizeof(int)*1024);
  

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
  SI.AssignSample(0);
  
  printf("<<<<SP->GetSource(0)=%d>>>>>\n",SP->GetSource(0));
  printf("<<<<SP->GetSource(1)=%d>>>>>\n",SP->GetSource(1));
  printf("<<<<SI.GetSampleIndex()=%d>>>>>\n",SI.GetSampleIndex());

  SI.Start(0,64,true);
  SI.Render(0,buffer,1024,true);
  for(i=0;i<1024;i++)
    {
      printf("%d\t",buffer[i]);
    }
  printf("\n");

  
}
