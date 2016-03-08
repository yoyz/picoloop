#include "SampleInstrument.h"
#include "SamplePool.h"
#include "FileSystem.h"
#include "UnixFileSystem.h"
#include <libgen.h>
#include "DEBSystem.h"

int main(int argc, char **argv)
{
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

  printf("\n",SP.GetNameListSize());
  printf("<<<<%d>>>>>\n",SP.GetNameListSize());
  SP.Load();
  printf("<<<<%d>>>>>\n",SP.GetNameListSize());
  //SP.loadSample(
  //SI.AssignSample(0);
  
}
